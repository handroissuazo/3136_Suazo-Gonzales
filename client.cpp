#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <exception>
#include "threadManager.h"
#include "semaphore.h"

/*Global Options*/
int RequestsPerPerson;
int SizeOfBuffer;
int NumberOfWorkers;
string ServerName = "";
int ServerPort;

void getOption(int argc, char *argv[ ])
{
    char option;

    //Specifying the expected options
    while ((option = getopt(argc, argv, "n:b:w:h:p:")) != -1)
    {
        switch (option) {
              case 'n' :
                  RequestsPerPerson = atoi(optarg);
                  break;
			  case 'b' :
                  SizeOfBuffer = atoi(optarg);
                  break;
			  case 'w' :
                  NumberOfWorkers = atoi(optarg);
                  break;
              case 'h':
                  ServerName = optarg;
                  break;
              case 'p':
                  ServerPort = atoi(optarg);
                  break;
              case '?':
                  if (optarg == "n" || optarg == "b" || optarg == "w" || optarg == "h" || optarg == "p")
                    fprintf (stderr, "Option -%c requires an argument.\n", optarg);
                  else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n",  optarg);
              default:
                  abort ();
          }
    }

    if(RequestsPerPerson == 0) RequestsPerPerson = 10;
    if(SizeOfBuffer == 0) SizeOfBuffer = 100;
    if(NumberOfWorkers == 0) NumberOfWorkers = 40;

    bool bIsStartFail = false;
    if(ServerPort == 0) {
        fprintf (stderr, "Option character for port (-p <port number>) missing.\n Please start the server with a port number.\n");
        bIsStartFail = true;
    }

    if(ServerName == ""){
        fprintf (stderr, "Option character for server name (-h <server name/IP address>) missing.\n Please start the server with a server name.\n");
        bIsStartFail = true;
    }

    if(bIsStartFail) abort(); //We don't want this program to start if there is no server name or port.

}

int main(int argc, char **argv)
{

	getOption(argc, argv);
    pid_t pid = fork();
    try
    {
        int retVal = 0;
        if (pid == 0)// child process
        {
            // Data server moved out
        }
        else if (pid > 0) // parent process
        {
    		ThreadManager threadManager(RequestsPerPerson, SizeOfBuffer, NumberOfWorkers, ServerName, ServerPort);
            threadManager.StartClient();
        }
        else
        {
            // fork failed
            printf("fork() failed!\n");
            return 1;
        }
    }
    //TODO figure out how to printf exceptions
    // catch (exception& e)
    // {
    //     printf("%s\n",e.what().c_str());
    // }
    catch(char const* e)
    {
        printf("%s\n", e);
    }
    catch(...)
    {
        printf("Catched an unknown error\n");
    }


	return 0;
}
