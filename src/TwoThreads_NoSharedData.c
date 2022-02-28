/* This code works on your VM! 
 * link and compile with -pthread
 * gcc -o app -g app.o -pthread 
 * gcc -o app.o -c -g app.c
 */
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <signal.h>

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>



int gSum; /* this data is shared by the thread(s) */


/**
 * The thread will begin control 
 * in the function runner()
 */

/****************************************************************************
 Function:		runner
 
 Description: entry point for the thread.  Print some data and return gSum.
 
 Parameters:	pParam - void* that is an int*
 
 Returned:		NONE
****************************************************************************/
void *runner(void *param) 
{
  printf("%d : %ld : %d\n", 
	getpid(),
	//gettid(),
  // libc does NOT provide a library call for gettid()
  // so make a system call directly!
  syscall(SYS_gettid),
	*(int*) param);

  pthread_exit((void*) &gSum);
}

/****************************************************************************
 Function:		main
 
 Description: launch two threads, wait for both to finish
 
 Parameters:	none
 
 Returned:		0 for success, 1 for failure
****************************************************************************/
int main(int argc, char *argv[])
{
  pthread_t tid[2]; /* the thread identifier */
  pthread_attr_t attr[2]; /* set of attributes for the thread */
  int *pResult;
  int value = 9;

/* adapted from page 133 of Silberschatz */
  gSum = 0;

  /* get the default attributes */
  pthread_attr_init(&attr[0]);
  pthread_attr_init(&attr[1]);

  /* create the thread */
  pthread_create(&tid[0],&attr[0],runner, &value);
  pthread_create(&tid[1],&attr[1],runner, &value);

  /* now wait for the thread to exit */
  pthread_join(tid[0], (void**) &pResult); // why a handle?
  pthread_join(tid[1], (void**) &pResult); // why a handle?

  printf("sum = %d OR %d\n",gSum, *pResult);
}
/* adapted from page 133 of Silberschatz */

