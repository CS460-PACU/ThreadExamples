/* This code works on your VM! 
 * link and compile with -pthread
 * gcc -o app -g app.o -pthread 
 * gcc -o app.o -c -g app.c
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>

#define THREAD_COUNT 2

uint8_t gSum; /* this data is shared by the thread(s) */


/**
 * The thread will begin control 
 * in the function runner()
 */

/****************************************************************************
 Function:		runner
 
 Description: entry point for the thread.  Add 1 to the shared data multiple
              times. No synchronization.
 
 Parameters:	pParam - void* that is an char*
 
 Returned:		NONE
****************************************************************************/
void *runner(void *pParam) 
{
  uint8_t i, upper = *(int*) pParam;

  if (upper > 0) 
  {
    for (i = 1; i <= upper; i++)
    {
      gSum += i;
    }
  }

  pthread_exit(NULL);
}



/****************************************************************************
 Function:		main
 
 Description: launch two threads, wait for both to finish
 
 Parameters:	none
 
 Returned:		0 for success, 1 for failure
****************************************************************************/
int main(int argc, char *argv[])
{
  pthread_t tid[THREAD_COUNT]; /* the thread identifier */
  pthread_attr_t sAttr[THREAD_COUNT]; /* set of attributes for the thread */
  uint8_t valueT1 = 10; // add a zero

  uint8_t correctSum = (valueT1*(valueT1+1)/2) * THREAD_COUNT;

/* adapted from page 133 of Silberschatz */
  gSum = 0;

  /* get the default attributes */
  for (int i = 0; i < THREAD_COUNT; ++i)
  {
    pthread_attr_init(&sAttr[i]);
  }

  /* create the threads */
  for (int i = 0; i < THREAD_COUNT; ++i)
  {
    pthread_create(&tid[i],&sAttr[i],runner, &valueT1);
  }
  

  /* now wait for the thread to exit */
  for (int i = 0; i < THREAD_COUNT; ++i)
  {
    pthread_join(tid[i], NULL);
  }

  printf("sum = %d\n",gSum);

  printf("Correct answer: %d\n", correctSum);

  // clean up
  for (int i = 0; i < THREAD_COUNT; ++i)
  {
	  pthread_attr_destroy (&sAttr[i]);
  }

}
/* adapted from page 133 of Silberschatz */

