#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define size 100000000
int array[size];
int numThreads = 2;
long sum = 0;
pthread_mutex_t t;

void computeSum(void * arg)
{
	int start = (int *)arg;
	int i;
	int division = size / numThreads;
	for(i = start * division ; i < (start * division) + division ; i++){
		pthread_mutex_lock(&t);
		sum += array[i];
		pthread_mutex_unlock(&t);
	}
}

int main()
{
	int i;

	/* Initialize array with integer values */
	for(i = 0 ; i < 100000000 ; i++)
		array[i] = i % 7;

	/* Initialize threads */
	pthread_t threadList[numThreads];

	for(int n = 0; n < numThreads; n++){
		pthread_create(&threadList[n], NULL, computeSum, (void *)n);
		printf("Thread %d began execution.\n", n);
	}

	for(int n = 0; n < numThreads; n++){
		pthread_join(threadList[n], NULL);
		printf("Thread %d completed execution.\n", n);
	}

	printf("Sum of the array is: %ld\n",sum);

	return 0;
}
