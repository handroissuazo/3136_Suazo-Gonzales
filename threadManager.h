/*
File: threadManager.h

class ThreadManager
 This object will serve as the manager for request, worker and statistics threads
*/
#ifndef _threadManager_H_                   // include file only once
#define _threadManager_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <ctime>
#include <vector>
#include "reqchannel.h"
#include "semaphore.h"

class ThreadManager
{
	//Declare ThreadManager's data members and helper functions
	public:
		//The constructor innitializes the control RequestChannel and whatever
		//other things that need to get done in the beginning
		ThreadManager(int req, int buf, int wok);
		~ThreadManager();

		void StartClient();

	private:
		//std::vector<RequestThread> v_requestThreads;
		//std::vector<WorkerThread> v_workerThreads;
		//std::vector<StatisticThread> v_staticticsThreads;
		RequestChannel* m_controlChannel;

		Semaphore* v_requestBuffer;
		Semaphore* v_responseBuffer1;
		Semaphore* v_responseBuffer2;
		Semaphore* v_responseBuffer3;

		int m_requestsPerPerson;
		int m_sizeOfBuffer;
		int m_numberOfWorkers;

		void enqueueRequestBuffer(string personRequested);
		void dequeueRequestBuffer(RequestPackage rqstPackg);

		void initRequestThreads();
		void initWorkerThreads();
		void initStatisticsThreads();

		void joinRequestThreads();
		void joinWorkerThreads();
		void joinStatisticsThreads();
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
