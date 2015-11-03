#include "threadManager.h"

ThreadManager::ThreadManager()
{
	printf("Establishing control channel... ");
	this->m_controlChannel = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
    printf("done.\n");


    // string reply1 = this->m_controlChannel->send_request("hello");
	// printf(reply1.c_str());
}

ThreadManager::ThreadManager(int RequestsPerPerson, int SizeOfBuffer, int NumberOfWorkers)
{
	m_requestsPerPerson = RequestsPerPerson;
	m_sizeOfBuffer = SizeOfBuffer;
	m_numberOfWorkers = NumberOfWorkers;
	printf("This application supports:\n\tRequests Per Person: %d\n\tBuffer Size: %d\n\tTotal Worker Threads: %d\n",m_requestsPerPerson, m_sizeOfBuffer, m_numberOfWorkers);

	printf("Establishing control channel... ");
	this->m_controlChannel = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
    printf("done.\n");
}

ThreadManager::~ThreadManager()
{
	string reply4 = this->m_controlChannel->send_request("quit");
	cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;
}

void ThreadManager::StartClient()
{
	printf("You have started the client!\n");
}
