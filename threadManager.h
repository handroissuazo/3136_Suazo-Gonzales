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

struct RequestPackage
{
	string personRequested;
	int requestNumber;
	string serverResponse;
	clock_t requestEnqued;
	clock_t requestDequed;
	clock_t requestSent;
	clock_t requestStored;

};

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
		std::vector<RequestPackage> v_requestBuffer;
		int m_requestsPerPerson;
		int m_sizeOfBuffer;
		int m_numberOfWorkers;

		void enqueueRequestBuffer(string personRequested);
		void dequeueRequestBuffer(RequestPackage rqstPackg);
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
