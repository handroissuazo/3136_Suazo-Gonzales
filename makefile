# makefile

all: dataserver client #simpleclient

reqchannel.o: reqchannel.h reqchannel.cpp
	g++-4.7 -c -g reqchannel.cpp

threadManager.o: threadManager.h threadManager.cpp reqchannel.o
	g++-4.7 -c -g threadManager.cpp reqchannel.o

dataserver: dataserver.cpp reqchannel.o
	g++-4.7 -g -o dataserver dataserver.cpp reqchannel.o -lpthread

# simpleclient: simpleclient.cpp reqchannel.o
# 	g++-4.7 -g -o simpleclient simpleclient.cpp reqchannel.o

client: client.cpp threadManager.o reqchannel.o
	g++-4.7 -g -o client client.cpp threadManager.o reqchannel.o -lpthread

clean:
	rm *o dataserver client #simpleclient
