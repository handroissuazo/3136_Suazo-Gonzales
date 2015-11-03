#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "threadManager.h"

/*Global Options*/
int RequestsPerPerson;
int SizeOfBuffer;
int NumberOfWorkers;
void getOption(int argc, char *argv[ ])
{
    char option;

    //Specifying the expected options
    while ((option = getopt(argc, argv, "n:b:w:")) != -1)
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
              case '?':
                  if (optarg == "n" || optarg == "b" || optarg == "w")
                    fprintf (stderr, "Option -%c requires an argument.\n", optarg);
                  else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n",  optarg);
              default:
                  abort ();
          }
    }
}

int main(int argc, char **argv)
{

	getOption(argc, argv);
	// printf("Requests per person: %d\tSize of Buffers: %d\tNumber of Workers: %d\n", RequestsPerPerson, SizeOfBuffer, NumberOfWorkers );
    pid_t pid = fork();

    if (pid == 0)// child process
    {
		ThreadManager threadManager(RequestsPerPerson, SizeOfBuffer, NumberOfWorkers);
        threadManager.StartClient();
    }
    else if (pid > 0) // parent process
    {
		execl("./dataserver","./dataserver");
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }

	return 0;
}
