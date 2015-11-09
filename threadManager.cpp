#include "threadManager.h"

ThreadManager::ThreadManager(int RequestsPerPerson, int SizeOfBuffer, int NumberOfWorkers)
{

	m_requestsPerPerson = RequestsPerPerson;
	m_sizeOfBuffer = SizeOfBuffer;
	m_numberOfWorkers = NumberOfWorkers;
	printf("This application supports:\n\tRequests Per Person: %d\n\tBuffer Size: %d\n\tTotal Worker Threads: %d\n",m_requestsPerPerson, m_sizeOfBuffer, m_numberOfWorkers);

	printf("Establishing control channel... ");
	m_controlChannel = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
  printf("done.\n");

	v_requestBuffer = new Semaphore(m_sizeOfBuffer);
	v_responseBuffer1 = new Semaphore(m_sizeOfBuffer);
	v_responseBuffer2 = new Semaphore(m_sizeOfBuffer);
	v_responseBuffer3 = new Semaphore(m_sizeOfBuffer);
}

ThreadManager::~ThreadManager()
{
	delete m_controlChannel;
	delete v_requestBuffer;
	delete v_responseBuffer1;
	delete v_responseBuffer2;
	delete v_responseBuffer3;
}

void ThreadManager::StartClient()
{
	printf("Started Client\n");

	initWorkerThreads();

	initStatisticsThreads();

	initRequestThreads();

	clientRunner();
}

void ThreadManager::enqueueRequestBuffer(string personRequested)
{
	for(int requestNum = 1; requestNum < m_requestsPerPerson + 1; requestNum++)
	{
		RequestPackage rqstPckg;
		rqstPckg.personRequested = personRequested;
		rqstPckg.requestNumber = requestNum;
		rqstPckg.requestEnqued = clock();

		v_requestBuffer->P(rqstPckg);
	}
}

void ThreadManager::dequeueRequestBufferEnqueueResponseBuffer(string strRequestChannel)
{
	RequestChannel chan(strRequestChannel, RequestChannel::CLIENT_SIDE);

	while(!v_requestBuffer->isDone()){
		RequestPackage newPackage = v_requestBuffer->V();

		if (v_requestBuffer->isDone()) break;
		string strReply = chan.send_request("data " + newPackage.personRequested);
		newPackage.serverResponse = strReply;

		if (newPackage.personRequested == "Joe Smith"){
			v_responseBuffer1->P(newPackage);
		}
		else if (newPackage.personRequested == "Jane Smith"){
			v_responseBuffer2->P(newPackage);
		}
		else if (newPackage.personRequested == "John Doe"){
			v_responseBuffer3->P(newPackage);
		}
	}

	string reply7 = chan.send_request("quit");
	cout << "Reply to request 'quit' is '" << reply7 << "'" << endl;
}

void ThreadManager::dequeueResponseBuffer1(){


	while(!v_responseBuffer1->isDone()){
		//printf("\n >>>> Size of buffer on dequeue 1 before dequeue: %d\n", v_responseBuffer1->returnNumberOfElems());
		RequestPackage newPackage = v_responseBuffer1->V();
		//printf("\n >>>> Size of buffer on dequeue 1 after dequeue: %d\n", v_responseBuffer1->returnNumberOfElems());

		if (v_responseBuffer1->isDone()) break;
		string strReply = newPackage.serverResponse;
		v_requestBuffer1Results.push_back(std::stoi(newPackage.serverResponse));
	}
}

void ThreadManager::dequeueResponseBuffer2(){
	while(!v_responseBuffer2->isDone()){
		RequestPackage newPackage = v_responseBuffer2->V();

		if (v_responseBuffer2->isDone()) break;
		string strReply = newPackage.serverResponse;
		v_requestBuffer2Results.push_back(std::stoi(newPackage.serverResponse));
	}
}

void ThreadManager::dequeueResponseBuffer3(){
	while(!v_responseBuffer3->isDone()){
		RequestPackage newPackage = v_responseBuffer3->V();

		if (v_responseBuffer3->isDone()) break;
		string strReply = newPackage.serverResponse;
		v_requestBuffer3Results.push_back(std::stoi(newPackage.serverResponse));
	}
}

void ThreadManager::initRequestThreads(){
	// enqueueRequestBuffer("Dimas Gonzales");
	std::thread requestThread1 (&ThreadManager::enqueueRequestBuffer, this, "Joe Smith");
	std::thread requestThread2 (&ThreadManager::enqueueRequestBuffer, this, "Jane Smith");
	std::thread requestThread3 (&ThreadManager::enqueueRequestBuffer, this, "John Doe");
	requestThread1.join();
	requestThread2.join();
	requestThread3.join();
}



void ThreadManager::initWorkerThreads(){
	for (int i = 0; i < m_numberOfWorkers; ++i){
		string strServerThreadRequest = m_controlChannel->send_request("newthread");
		cout << "Reply to request 'newthread' is " << strServerThreadRequest << "'" << endl;

		v_workerThreads.push_back(std::thread(&ThreadManager::dequeueRequestBufferEnqueueResponseBuffer, this, strServerThreadRequest));
	}
}

void ThreadManager::initStatisticsThreads(){
	v_staticticsThreads.push_back(std::thread (&ThreadManager::dequeueResponseBuffer1, this));
	v_staticticsThreads.push_back(std::thread (&ThreadManager::dequeueResponseBuffer2, this));
	v_staticticsThreads.push_back(std::thread (&ThreadManager::dequeueResponseBuffer3, this));
}

void ThreadManager::clientRunner(){
	std::string strUserInput;
	while (cin>>strUserInput){
		if (strUserInput == "quit"){
			break;
		}
	}

	v_requestBuffer->setDone(true);
	v_responseBuffer1->setDone(true);
	v_responseBuffer2->setDone(true);
	v_responseBuffer3->setDone(true);

	joinRequestThreads();
	printf("NOTICE: Request threads complete!\n");
	joinWorkerThreads();
	printf("NOTICE: Worker threads complete!\n");
	joinStatisticsThreads();
	printf("NOTICE: Statistics threads complete!\n");

	string reply = m_controlChannel->send_request("quit");
	cout << "Reply to request 'quit' is '" << reply << "'" << endl;

	usleep(100000); //Just for print formatting.

	processResults();

	printf("\n ---------------------------------- \n Thanks for using our program!\n ---------------------------------- \n");
}


void ThreadManager::joinRequestThreads(){

}

void ThreadManager::joinWorkerThreads(){
	// for (int i = 0; i < v_workerThreads.size(); ++i)
	// {
	// 	v_workerThreads[i].join();
	// }
	for (auto& t: v_workerThreads) t.join();
}

void ThreadManager::joinStatisticsThreads(){
	for (auto& t: v_staticticsThreads) t.join();
}

void ThreadManager::processResults()
{
 	cout<<v_requestBuffer1Results.size()<<endl;
	cout<<v_requestBuffer2Results.size()<<endl;
	cout<<v_requestBuffer3Results.size()<<endl;

	//for (auto& t: v_staticticsThreads) cout<<t<<endl;
}
