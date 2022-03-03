/* This code works on your VM! 
 * link and compile with -pthread
 * gcc -o app -g app.o -pthread 
 * gcc -o app.o -c -g app.c
 */
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
 
 Description: entry point for the thread.  Add 1 to the shared data multiple
              times. No synchronization.
 
 Parameters:	pParam - void* that is an int*
 
 Returned:		NONE
****************************************************************************/
void *runner(void *pParam) 
{
  int i, upper = *(int*) pParam;

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

  /* create the thread */
  pthread_create(&tid[0],&sAttr[0],runner, &valueT1);
  pthread_create(&tid[1],&sAttr[1],runner, &valueT2);

  /* now wait for the thread to exit */
  pthread_join(tid[0], NULL); // why a handle?
  pthread_join(tid[1], NULL); // why a handle?

  printf("sum = %d\n",gSum);

  printf("Correct answer: %d\n", correctSum);
	pthread_attr_destroy (&sAttr[0]);
	pthread_attr_destroy (&sAttr[1]);

}
/* adapted from page 133 of Silberschatz */

