/*
    File: dataserver.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/16

    Dataserver main program for MP3 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "NetworkRequestChannel.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* VARIABLES */
/*--------------------------------------------------------------------------*/

int backlogSize = 0;
int currentUsedPorts = 0;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

void handle_process_loop(NetworkRequestChannel & _channel);

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THREAD FUNCTIONS */
/*--------------------------------------------------------------------------*/

void * handle_data_requests(void * args) {

    NetworkRequestChannel * data_channel =  (NetworkRequestChannel*)args;

    data_channel->WaitForClientInitConnection();
  // -- Handle client requests on this channel.

    handle_process_loop(*data_channel);

  // -- Client has quit. We remove channel.

    delete data_channel;
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- INDIVIDUAL REQUESTS */
/*--------------------------------------------------------------------------*/

void process_hello(NetworkRequestChannel & _channel, const string & _request) {
  _channel.cwrite("hello to you too");
}

void process_data(NetworkRequestChannel & _channel, const string &  _request) {
  usleep(1000 + (rand() % 5000));
  //_channel.cwrite("here comes data about " + _request.substr(4) + ": " + int2string(random() % 100));
  _channel.cwrite(int2string(rand() % 100));
}

void process_newthread(NetworkRequestChannel & _channel, const string & _request, NetworkRequestChannel::ReqChannelType channelType) {
    int error;

    if (currentUsedPorts < backlogSize) {
        // -- Construct new data channel (pointer to be passed to thread function)

        NetworkRequestChannel *data_channel = new NetworkRequestChannel(0, NULL, channelType);
        string new_channel_port = data_channel->port();

        // Send back port
        _channel.cwrite(new_channel_port);

        // -- Create new thread to handle request channel
        pthread_t thread_id;

        if (error = pthread_create(&thread_id, NULL, handle_data_requests, data_channel)) {
            fprintf(stderr, "p_create failed: %s\n", strerror(error));
        }

        currentUsedPorts++;
    }
    else {  // Fail gracefully if no new threads can be created/no new ports can be opened.
        _channel.cwrite("-1");
    }
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

void process_request(NetworkRequestChannel & _channel, const string & _request) {

  if (_request.compare(0, 5, "hello") == 0) {
    process_hello(_channel, _request);
  }
  else if (_request.compare(0, 4, "data") == 0) {
    process_data(_channel, _request);
  }
  else if (_request.compare(0, 9, "newthread") == 0) {
    process_newthread(_channel, _request, NetworkRequestChannel::THREAD_CHANNEL);
  }
  else {
    _channel.cwrite("unknown request");
  }

}

void handle_process_loop(NetworkRequestChannel & _channel) {

  for(;;) {

    string request = _channel.cread();

    if (request.compare("quit") == 0) {
      _channel.cwrite("bye");
      usleep(10000);          // give the other end a bit of time.
        if(_channel.my_type == NetworkRequestChannel::THREAD_CHANNEL) {
            --currentUsedPorts;
            break;                  // break out of the loop;
        }
    }

    process_request(_channel, request);
  }
}

void handle_control_loop(NetworkRequestChannel & _channel){

    while(true) {
        _channel.WaitForClientInitConnection();

        for (; ;) {

            string request = _channel.cread();

            if (request.compare("quit") == 0) {
                _channel.cwrite("bye");
                usleep(10000);          // give the other end a bit of time.
                break;                  // break out of the loop;
            }

            process_request(_channel, request);
        }
    }
}
/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    char option;
    int desiredPortNumber, backlog = 0;

    //Specifying the expected options
    while ((option = getopt(argc, argv, "p:b:")) != -1)
    {
        switch (option) {
            case 'p' :
                desiredPortNumber = atoi(optarg);
                break;
            case 'b' :
                backlogSize = atoi(optarg);
                break;
            case '?':
                if (optarg == "n" || optarg == "b" || optarg == "w")
                    fprintf (stderr, "Option -%c requires an argument.\n", optarg);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n",  optarg);
            default:
                abort ();
        }
    }

    if(desiredPortNumber == 0) desiredPortNumber = 25526;
    if(backlogSize == 0) backlogSize = 100;

   // cout << "Establishing control channel... " << flush;
    NetworkRequestChannel control_channel(desiredPortNumber, NULL, NetworkRequestChannel::CONTROL_CHANNEL);
   // cout << "done.\n" << flush;

    handle_control_loop(control_channel);

}
