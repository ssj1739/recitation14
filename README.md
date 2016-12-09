Recitation 14: The Last Recitation :'(
-------------------------------------------

In this exercise, we will look at developing parallel code to make a sequential single-threaded program faster. You will find `sequential.c` in this repository. Compile it and run the executable. You should see something like:

```
	Sum of the array is: 299999995
```

The program initializes a large array of integers. Then it sums the elements of the array and finally prints the total to the console. You may notice that the execution takes a good few seconds before the program terminates successfully. This is simply because the array is very large. Let us see if we can utilize the hardware on our machines to make the execution faster.

In order to process the array with multiple cores or processors, we will need to use threads. As you know, a thread is a sequential piece of code with its own stack and a program or process can invoke multiple threads. Of course, the number of threads that can be executed simultaneously depends on the number of cores or processors that your machine has. If you run the program with more threads than the machine supports, the threads will run concurrently but will require context switching to move from one thread to the other. 

To use threads, we will use pthreads. We do so by including the library as follows:

```
	#include <pthread.h>
```

And we compile C programs that use pthreads by adding the following flag:

```
	gcc -o foo foo.c -lpthread
```

Now that we know how to compile and run programs with threads, lets actually see how to initialize and use threads. The following commands will initialize a thread variable:

```
	pthread_t thread;
```

Next, we need to start the thread. In pthreads, running a thread requires passing a function name as an argument. The code that the thread executes will be the code of this function. If you open `sequential.c` you will find that the program utilizes the `computeSum()` function to sum the elements of the array. In our parallel version, we would want to divide the array into multiple chunks and assign them to threads. Each thread will compute the sum of its chunk and then finally add it to the global sum shared by all threads. So, to start the thread with the `computeSum()` function, we would have the following:

```
	pthread_create(&thread, NULL, computeSum, (void*) t);
```

In the above command, the first argument specifies the pointer to the thread. We can ignore the second argument by passing NULL as we do not need it. The third is the name of the function that we want the thread to execute. And the fourth is an input argument to the function. The specification of pthreads requires that the function we pass to the thread requires an argument of type `void*`. This is not a big problem, as we can cast our items to `void*` and back. In our example, we want to input some indices, or one number to decide what portion of the array we want to compute (there is no need to pass the array for this exercise, lets keep it global). So if `t` is the item we want to pass to computeSum, then we call the `pthread_create()` as above and pass `t` by casting it to `void*` and we can then cast it back to `int` inside `computeSum*()`.

Once the threads have computed the sums of their own portions, we want to add them to the global sum. After that, we need a mechanism of terminating the threads. A thread terminates when the function it is executing returns. However, we do not want `main()` to terminate before the threads have finished their computation. So we need to call `join` on the threads in main. This is a mechanism by which the main method will wait for the thread to complete before proceeding. If we use multiple threads, we need to call `join` on all of them. You will find it useful to have an array of threads for this exercise. To call `join`:

```
	pthread_join(thread, NULL);
``` 

Where the first argument is the pthread_t variable and we can ignore the second one as before. Once all threads return, the main method can proceed beyond the call to `join`, print the sum and return.

## Exercise ##

1) Parallelize the computation in `sequential.c` and write your parallel code in a file called `parallel.c`. Compile and run your program. Do you get the same result as that of the sequential version? No? Try running it again. Do you get different outputs each time from the parallel version? Try to think why.

2) In order to compute the correct result as with the sequential version, we can apply a lock to the update procedure in `computeSum()`. We can do this as follows:

```

pthread_mutex_t mutex;	// global

int main()
{
	...
	
	pthread_mutex_init(&mutex, NULL);
	
	...
}

void computeSum(void* t)
{
	...

	pthread_mutex_lock(&mutex);
	sum += localSum;
	pthread_mutex_unlock(&mutex);

	...
}
```

Try running your code after applying the lock. See if the program now runs and computes the correct result. How did the lock fix the problem we had before?

3) Now that your parallel code works, lets see if it was actually worth the hassle. In linux, you can time the execution of your program by using the time command. Try the following:

```
	time ./sequential

	time ./parallel
```

Where `sequential` and `parallel` are the executables. You should see something like the following:

```
	Sum of the array is: 299999995

	real	0m1.269s
	user	0m4.347s
	sys		0m2.171s
```

Ignore the `user` and `sys` times for now. Look at the `real` time for both the parallel and the sequential version. Try to make sure that your parallel version is faster than the sequential one. If your parallel version is slower, ask for help. Also remember that all calls to functions in the pthread library involve expensive operations. So we do not want that overhead to swamp the gains of parallelizing our code.
