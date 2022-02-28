//***************************************************************************
// File name:		OneThread.c
// Author:			chadd williams
// Date:				March 16, 2018
// Class:				CS 460
// Assignment:	Thread examples
// Purpose:			Example of starting one thread
// 							compile and link with -pthread
//							gcc -o app -g app.o -pthread 
//***************************************************************************

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// global data shared by all threads
int gSum;

// the function to run in the thread
void *runner(void *pParam);

/****************************************************************************
 Function:		main
 
 Description: launch one thread
 
 Parameters:	none
 
 Returned:		0 for success, 1 for failure
****************************************************************************/
int main()
{
	pthread_t tid; 				
	int *pResult;
	int threadParam = 2;
	
  gSum = 0;

	/* create the thread */
	pthread_create (&tid, NULL, runner, &threadParam);

	/* now wait for the thread to exit */
	pthread_join (tid,  (void**)&pResult); // why a handle?

	printf("sum = %d OR %d\n",gSum, *pResult);

	return 0;
}


/****************************************************************************
 Function:		runner
 
 Description: entry point for the thread.  Sum 1 to *param
							set gSum to the sum calculated.  Return gSum via
							pthread_exit
 
 Parameters:	pParam - void* that is an int*
 
 Returned:		address of gSum via pthread_exit()
****************************************************************************/
void *runner(void *pParam) 
{
	int localSum = 0;
	int i, upper = atoi(pParam);


	for (i = 1; i <= upper; i++)
	{
		localSum += i;
	}

	// set global variable
	gSum = localSum;
	
	pthread_exit((void*) &gSum);
}
