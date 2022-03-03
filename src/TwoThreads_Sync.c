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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


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
int main()
{
  pthread_t tid[2]; /* the thread identifier */
  pthread_attr_t sAttr[2]; /* set of attributes for the thread */
  int valueT1 = 10000;
  int valueT2 = 10000;
  int correctSum = (valueT1*(valueT1+1)/2) + (valueT2*(valueT2+1)/2);

/* adapted from page 133 of Silberschatz */
  gSum = 0;

  /* get the default attributes */
  pthread_attr_init(&sAttr[0]);
  pthread_attr_init(&sAttr[1]);

	
	pthread_mutex_init (&gsMutex, NULL); // initialize the mutex
	
	  /* create the thread */
  pthread_create(&tid[0],&sAttr[0],runner, &valueT1);
  pthread_create(&tid[1],&sAttr[1],runner, &valueT2);

  /* now wait for the thread to exit */
  pthread_join(tid[0], NULL); // why a handle?
  pthread_join(tid[1], NULL); // why a handle?

	// no other threads are active, so no mutex is necessary
	printf("sum = %d\n",gSum);
	printf("Correct answer: %d\n", correctSum);
  

	// cleanup data
	pthread_mutex_destroy (&gsMutex);
	pthread_attr_destroy (&sAttr[0]);
	pthread_attr_destroy (&sAttr[1]);
		

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
	 // int localSum = 0; // A FASTER METHOD
		
	for (i = 1; i <= *(int*) pParam; i++)
	{

    // A SLOWER METHOD
  	// lock the mutex before you update shared data!
  	pthread_mutex_lock (&gsMutex);
	  gSum += i;
	  pthread_mutex_unlock (&gsMutex);
	  // unlock the mutex so the other threads can run!


    /* A FASTER METHOD
		localSum += i;
    */
	}

  /* A FASTER METHOD
	// lock the mutex before you update shared data!
	pthread_mutex_lock (&gsMutex);
	gSum += localSum;
	pthread_mutex_unlock (&gsMutex);
	// unlock the mutex so the other threads can run!
	*/

	pthread_exit (NULL);
}
