/*
File: threadManager.h

class ThreadManager
 This object will serve as the manager for request, worker and statistics threads
*/
#ifndef _threadManager_H_                   // include file only once
#define _threadManager_H_

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <ctime>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <unistd.h>
#include "NetworkRequestChannel.h"
#include "semaphore.h"

using namespace std;
class ThreadManager
{
	//Declare ThreadManager's data members and helper functions
	public:
		//The constructor innitializes the control RequestChannel and whatever
		//other things that need to get done in the beginning
		ThreadManager(int req, int buf, int wok, string ServerName, int ServerPort);
		~ThreadManager();

		void StartClient();

	//private:
		//std::vector<RequestThread> v_requestThreads;
		std::vector<RequestPackage> v_requestBuffer1Results;
		std::vector<RequestPackage> v_requestBuffer2Results;
		std::vector<RequestPackage> v_requestBuffer3Results;

		std::vector<std::thread> v_workerThreads;
		std::vector<std::thread> v_staticticsThreads;
		std::vector<NetworkRequestChannel> v_workerChannels;
		NetworkRequestChannel* m_controlChannel;

		Semaphore* v_requestBuffer;
		Semaphore* v_responseBuffer1;
		Semaphore* v_responseBuffer2;
		Semaphore* v_responseBuffer3;

		int m_requestsPerPerson;
		int m_sizeOfBuffer;
		int m_numberOfWorkers;
		string m_serverName;
		int m_serverControlPort;

		void enqueueRequestBuffer(string personRequested);
		void dequeueRequestBufferEnqueueResponseBuffer(string strPort);
		void dequeueResponseBuffer1();
		void dequeueResponseBuffer2();
		void dequeueResponseBuffer3();

		void initRequestThreads();
		void initWorkerThreads();
		void initStatisticsThreads();

		void checkClose();
		void clientCloser();

		void joinRequestThreads();
		void joinWorkerThreads();
		void joinStatisticsThreads();

		void processResults(std::vector<RequestPackage> reqPacks);

		bool finish1;
		bool finish2;
		bool finish3;
};

// class RequestThread
// {
// public:
// 	RequestThread();
// 	~RequestThread();
//
// private:
//
// }


#endif
