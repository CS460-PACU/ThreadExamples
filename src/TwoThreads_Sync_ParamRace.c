//***************************************************************************
// File name:		TwoThread.c
// Author:			chadd williams
// Date:				March 16, 2018
// Class:				CS 460
// Assignment:	Thread examples
// Purpose:			Example of starting two threads and synchronization
// 							compile and link with -pthread
//							gcc -o app -g app.o -pthread 
//***************************************************************************
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>


// global data shared by all threads
int gSum = 0;
pthread_mutex_t gsMutex;

// the function to run in the thread
void *runner(void *pParam);

/****************************************************************************
 Function:		main
 
 Description: launch two threads, wait for both to finish
 
 Parameters:	none
 
 Returned:		0 for success, 1 for failure
****************************************************************************/
int main(int argc, char **argv)
{
	pthread_t tid1, tid2; 	// thread ids
	pthread_attr_t sAttr; 		// thread attributes
  
	int value = 5;
	
	
	pthread_mutex_init (&gsMutex, NULL); // initialize the mutex
	
	pthread_attr_init (&sAttr);					// get default thread attributes
	
	// create each thread
	pthread_create (&tid1, &sAttr, runner, &value);

		
  if (argc > 1)
  {
    // such a small sleep, does not always
    // prevent race!
    nanosleep( (const struct timespec[]){{0, 5000}}, NULL);
  }

  value += 1;

	pthread_create (&tid2, &sAttr, runner, &value);

	// wait for each thread to finish
	// don't capture any data from pthread_exit()
	pthread_join (tid1, NULL);
	pthread_join (tid2, NULL);
	
	// cleanup data
	pthread_mutex_destroy (&gsMutex);
	pthread_attr_destroy (&sAttr);
		
	// no other threads are active, so no mutex is necessary
	printf("sum = %d\n",gSum);
	
	return 0;
}

/****************************************************************************
 Function:		runner
 
 Description: entry point for the thread.  Sum 1 to *param
							add the sum calculated to gSum. Use mutex for synchonization
 
 Parameters:	pParam - void* that is an int*
 
 Returned:		NONE
****************************************************************************/
void *runner(void *pParam) 
{
	int i;
	int localSum = 0;

  printf("%d : %ld : %d\n", 
	  getpid(),
	  //gettid(),
    // libc does NOT provide a library call for gettid()
    // so make a system call directly!
    syscall(SYS_gettid),
	  *(int*) pParam);

	for (i = 1; i <= *(int*) pParam; i++)
	{
		localSum += i;
	}

	// lock the mutex before you update shared data!
	pthread_mutex_lock (&gsMutex);
	gSum += localSum;
	pthread_mutex_unlock (&gsMutex);
	// unlock the mutex so the other threads can run!
	
	pthread_exit (NULL);
}
