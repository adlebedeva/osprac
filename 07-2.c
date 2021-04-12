#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0; // The variable a is global static for the entire program,
// so it will be shared by both threads of execution.

/**
Below follows the text of the function
that will be associated with the 2nd thread.
Parameter dummy is used only for type compatibility.
For the same reason, the function returns void *.
**/
void void* dummy{
pthread_t mythid;
//
// Note that mythid variable is a dynamic local variable of mythread () function,
// that is, it is pushed on the stack and, therefore, is not shared by the threads.
//
mythid = pthread_self();
a = a + 1;
printf("Thread %u, Calculation result = %d\n", mythid, a);
return NULL;
}

/**
The main() function is also an associated function of the main thread.
**/
int main() {
	pthread_t thid1, thid2, mythid;
	int result1, result2;
	//
	// Try to create a new thread of execution associated with the mythread () function.
	// Pass it NULL as a parameter. If it succeeds,
	// the identifier of the new thread is written into the thid variable.
	// If an error occurs, stop working.
	//
	result1 = pthread_create(&thid1, (pthread_attr_t*)NULL, mythread, NULL);

	if (result1 != 0) {
		printf("Error on thread create, return value = %d\n", result1);
		exit(-1);
	}

	printf("Thread1 created, thid1 = %u\n", thid1);

	result2 = pthread_create(&thid2, (pthread_attr_t*)NULL, mythread, NULL);

	if (result2 != 0) {
		printf("Error on thread create, return value = %d\n", result2);
		exit(-1);
	}

	printf("Thread2 created, thid2 = %u\n", thid2);
	mythid = pthread_self();
	a = a + 1;
	printf("Thread %u, Calculation result = %d\n", mythid, a);
	//
	// Wait for the spawned thread to terminate, not caring what value it returns.
	// If this function is not called, the main() function may end before
	// the spawned thread is executed, which will automatically cause it to terminate,
	// distorting the results.
	//
	pthread_join(thid1, (void**)NULL);
	pthread_join(thid2, (void**)NULL);
	return 0;
}
