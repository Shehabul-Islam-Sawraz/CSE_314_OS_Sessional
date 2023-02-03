#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int counter = 0;

/**
 * Memory is shared across all threads. So, all threads share the variable `counter` without any duplication.
*/

void* routine() {
    for(int i=0; i<1000000;i++){
        counter++;
    }
}

int main(int argc, char* argv[]) {
    /**
     * @pthread_t initializes the variables where API can store information about the threads 
     * @pthread_create initialize the threads
     * @pthread_join make calling thread wait for termination of the pthread_t variable
     */
    pthread_t p1, p2;
    if (pthread_create(&p1, NULL, &routine, NULL) != 0) {
        return 1;
    }
    if (pthread_create(&p2, NULL, &routine, NULL) != 0) {
        return 2;
    }
    if (pthread_join(p1, NULL) != 0) { 
        return 3;
    }
    if (pthread_join(p2, NULL) != 0) {
        return 4;
    }
    /**
     * According to our code, value of `counter` supposed to be 2000000
     * But, here race condition will happen between the threads, 
     * and we may get a much lower value than 2000000
     * N.B: Run the loop for a much large value. For smaller no of iterations,
     * race condition may not happen as it takes much smaller time to be done with iterations.
    */
    printf("Now counter is: %d\n", counter);
    return 0;
}