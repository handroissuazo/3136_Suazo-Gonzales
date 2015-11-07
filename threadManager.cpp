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
}

ThreadManager::~ThreadManager()
{
	string reply = m_controlChannel->send_request("quit");
	cout << "Reply to request 'quit' is '" << reply << "'" << endl;
}

void ThreadManager::StartClient()
{
	printf("Started Client\n");
	enqueueRequestBuffer("Dimas Gonzales");

	// std::thread requestThread1 (enqueueRequestBuffer, "Dimas Gonzales");
	// std::thread requestThread2 (enqueueRequestBuffer, "Alejandro Suazo");
	// std::thread requestThread3 (enqueueRequestBuffer, "John Jacob");

	while(!v_requestBuffer.empty()){
		dequeueRequestBuffer(v_requestBuffer.back());
		v_requestBuffer.pop_back();
	}

	// requestThread1.join();
	// requestThread2.join();
	// requestThread3.join();

}

void ThreadManager::enqueueRequestBuffer(string personRequested)
{
	for(int requestNum = 1; requestNum < m_requestsPerPerson + 1; requestNum++)
	{
		RequestPackage rqstPckg;
		rqstPckg.personRequested = personRequested;
		rqstPckg.requestNumber = requestNum;
		rqstPckg.requestEnqued = clock();

		v_requestBuffer.push_back(rqstPckg);
	}
}

void ThreadManager::dequeueRequestBuffer(RequestPackage rqstPackg)
{
	string threadRequest = m_controlChannel->send_request("newthread");
	cout << "Reply to request 'newthread' is " << threadRequest << "'" << endl;
	RequestChannel chan2(threadRequest, RequestChannel::CLIENT_SIDE);

	string reply6 = chan2.send_request("data "+rqstPackg.personRequested);
	cout << "Reply to request 'data " + rqstPackg.personRequested + "' is '" << reply6 << "'" << endl;

	string reply7 = chan2.send_request("quit");
	cout << "Reply to request 'quit' is '" << reply7 << "'" << endl;

}
