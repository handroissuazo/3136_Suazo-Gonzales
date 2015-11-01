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
#include "reqchannel.h"


class ThreadManager
{
	//Declare ThreadManager's data members and helper functions
	public:
		//The constructor innitializes the control RequestChannel and whatever
		//other things that need to get done in the beginning
		ThreadManager();
		ThreadManager(int RequestsPerPerson, int SizeOfBuffer, int NumberOfWorkers);
		~ThreadManager();

	private:
		//std::vector<RequestThread> v_requestThreads;
		//std::vector<WorkerThread> v_workerThreads;
		//std::vector<StatisticThread> v_staticticsThreads;
		RequestChannel* m_controlChannel;
		int m_requestsPerPerson;
		int m_sizeOfBuffer;
		int m_numberOfWorkers;

};


#endif
