//***************************************************************************
// File name:		CondWaitThread.c
// Author:			chadd williams
// Date:				March 16, 2018
// Class:				CS 460
// Assignment:	Thread examples
// Purpose:			Example of starting two threads, cond wait, signal,
//							and synchronization
// 							link with -lpthread
//							gcc -o app -g app.o -lpthread 
//***************************************************************************

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>


// global data shared by all threads
typedef struct ThreadArgs
{
	// data, mutex, and cond to signal the threads to start
	int startFlag ;
	pthread_mutex_t sStartMutex;
	pthread_cond_t sStartWaitCond;

	// data, mutex, and cond to signal that all threads have finished
	int finishedFlag ;
	pthread_mutex_t sFinishedMutex;
	pthread_cond_t sFinishedWaitCond;
} ThreadArgs;

// the functions to run in the threads
void *runner(void *pParam);
void *runnerNoWait(void *pParam);

/****************************************************************************
 Function:		main
 
 Description: launch two threads, wait for both to finish
 
 Parameters:	none
 
 Returned:		0 for success, 1 for failure
****************************************************************************/
int main()
{
	pthread_t tid1, tid2; 	// thread ids
	pthread_attr_t sAttr; 		// thread attributes
	
	ThreadArgs sThreadArg;
	int tid = syscall(SYS_gettid);

		
	srand(time(NULL)); 			// seed random number generator
	
	sThreadArg.startFlag = 0;
	sThreadArg.finishedFlag = 0;
	



	// create mutex and cond
	pthread_mutex_init (&sThreadArg.sStartMutex, NULL);
	pthread_cond_init (&sThreadArg.sStartWaitCond, NULL);




	pthread_mutex_init (&sThreadArg.sFinishedMutex, NULL);
	pthread_cond_init (&sThreadArg.sFinishedWaitCond, NULL);

	
	pthread_attr_init (&sAttr);		// get default thread attributes
	
	// create each thread
	pthread_create (&tid1, &sAttr, runner, &sThreadArg);
	pthread_create (&tid2, &sAttr, runnerNoWait, &sThreadArg);
	
	// sleep for a random amount of time
	sleep(rand() % 10);
	
	
	
	
	// signal the thread to start
	// lock the mutex to access shared data
	pthread_mutex_lock(&sThreadArg.sStartMutex);
	
	
	
	// change the condition flag
	sThreadArg.startFlag = 1;
	
	
	
	// unlock the mutex so other threads can access the data
	pthread_mutex_unlock(&sThreadArg.sStartMutex);
	
	
	
	
	// signal the waiting thread to check the condition
	pthread_cond_signal(&sThreadArg.sStartWaitCond);











	// many threads can call pthread_cond_signal()
	// with the same cond variable
	// only one thread can call pthread_cond_wait()
	// with the same cond variable


	// wait for all threads to finish!
	fprintf(stderr,"\nmain waiting %d\n\n", tid);


	pthread_mutex_lock(&sThreadArg.sFinishedMutex);




	while(sThreadArg.finishedFlag != 2)
	{



		pthread_cond_wait(&sThreadArg.sFinishedWaitCond, 
			&sThreadArg.sFinishedMutex);
			
		fprintf(stderr,"\nmain wakeup\n\n");
	}



	sThreadArg.finishedFlag = 0;



	pthread_mutex_unlock(&sThreadArg.sFinishedMutex);



	fprintf(stderr,"\nmain finishing\n\n");



	
	// wait for each thread to call pthread_exit()
	pthread_join (tid1, NULL);
	pthread_join (tid2, NULL);
	


	
	// cleanup allocated data
	pthread_mutex_destroy (&sThreadArg.sStartMutex);
	pthread_cond_destroy (&sThreadArg.sStartWaitCond);

	pthread_mutex_destroy (&sThreadArg.sFinishedMutex);
	pthread_cond_destroy (&sThreadArg.sFinishedWaitCond);

	pthread_attr_destroy (&sAttr);
	
	fprintf(stderr,"\nmain DONE\n\n");
}

/****************************************************************************
 Function:		runnerNoWait
 
 Description: entry point for the thread. 
							sleep randomly
							increment sThreadArg.finishedFlag to indicate the thread is done
							signal main() that the sThreadArg.finishedFlag state has changed
 
 Parameters:	pParam - void* that is an int*
 
 Returned:		NONE
****************************************************************************/
void *runnerNoWait(void *pParam) 
{
	
	
	ThreadArgs *psThreadArg = (ThreadArgs*) pParam;

	int localSeed = (int) time(NULL);
	
	
	int tid = syscall(SYS_gettid);
	
	
	fprintf(stderr,"\nThread running %d!\n\n", tid);
	
	// do work
	
	//rand() is not reentrant
	sleep(rand_r(&localSeed) % 20);
	
	
	
	
	// signal main() that psThreadArg->finishedFlag
	// has changed
	pthread_mutex_lock(&psThreadArg->sFinishedMutex);
	
	
	psThreadArg->finishedFlag ++;
	
	
	pthread_mutex_unlock(&psThreadArg->sFinishedMutex);
	

	
	
	pthread_cond_signal(&psThreadArg->sFinishedWaitCond);



	fprintf(stderr,"\nThread DONE %d!\n\n", tid);


	pthread_exit (NULL);
}


/****************************************************************************
 Function:		runner
 
 Description: entry point for the thread. 
							wait for main() to set sThreadArg.startFlag before continuing
							sleep randomly
							increment sThreadArg.finishedFlag to indicate the thread is done
							signal main() that the sThreadArg.finishedFlag state has changed
 
 Parameters:	pParam - void* that is an int*
 
 Returned:		NONE
****************************************************************************/
void *runner(void *pParam) 
{
	ThreadArgs *psThreadArg = (ThreadArgs*) pParam;
	
	int localSeed = (int) time(NULL);
	
	int tid = syscall(SYS_gettid);

	
	fprintf(stderr,"\nThread waiting %d!\n\n", tid);
	
	
	// wait until the sThreadArg.startFlag is set
	pthread_mutex_lock(&psThreadArg->sStartMutex);
	
	while(psThreadArg->startFlag != 1)
	{
	
		pthread_cond_wait(&psThreadArg->sStartWaitCond, &psThreadArg->sStartMutex);
	}
	
	psThreadArg->startFlag = 0;
	
	pthread_mutex_unlock(&psThreadArg->sStartMutex);
	
	fprintf(stderr,"\nThread running %d!\n\n", tid);

	// do work
	
	//rand() is not reentrant
	sleep(rand_r(&localSeed) % 10);
	
	
	
	// signal main() that psThreadArg->finishedFlag
	// has changed
	pthread_mutex_lock(&psThreadArg->sFinishedMutex);
	psThreadArg->finishedFlag ++;
	
	pthread_mutex_unlock(&psThreadArg->sFinishedMutex);
	pthread_cond_signal(&psThreadArg->sFinishedWaitCond);
	
	fprintf(stderr,"\nThread DONE %d!\n\n", tid);

	pthread_exit (NULL);
}
