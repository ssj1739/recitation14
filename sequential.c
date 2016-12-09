#include <stdlib.h>
#include <stdio.h>

#define size 100000000
int array[size];
int numThreads = 1;
long sum = 0;

void computeSum(int start)
{
	int i;
	int division = size / numThreads;

	for(i = start * division ; i < (start * division) + division ; i++)
		sum += array[i];
}

int main()
{
	int i;

	/* Initialize array with integer values */
	for(i = 0 ; i < 100000000 ; i++)
		array[i] = i % 7;

	/* Compute the sum */
	computeSum(0);

	printf("Sum of the array is: %ld\n",sum);

	return 0;
}
