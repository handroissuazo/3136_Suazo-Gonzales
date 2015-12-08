# makefile
all: dataserver client

semaphore.o: semaphore.h semaphore.cpp
	g++ -std=c++11 -w -c -g semaphore.cpp -pthread

NetworkRequestChannel.o: NetworkRequestChannel.h NetworkRequestChannel.cpp
	g++ -std=c++11 -w -c -g NetworkRequestChannel.cpp

threadManager.o: threadManager.h threadManager.cpp NetworkRequestChannel.o semaphore.o
	g++ -std=c++11 -c -g threadManager.cpp NetworkRequestChannel.o semaphore.o -pthread

dataserver: dataserver.cpp NetworkRequestChannel.o
	g++ -std=c++11 -w -g -o dataserver dataserver.cpp NetworkRequestChannel.o -lpthread

client: client.cpp threadManager.o NetworkRequestChannel.o
	g++ -std=c++11 -w -g -o client client.cpp threadManager.o NetworkRequestChannel.o semaphore.o -pthread

clean:
	rm *o dataserver client
