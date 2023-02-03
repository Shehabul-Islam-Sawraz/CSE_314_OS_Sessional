#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/**
 * @Problem Statement:
 * 
 * Create 10 threads, each taking a unique price from an array
 * named primes & print it on the screen
*/

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
    //sleep(1);
    int index = *(int*)arg;
    printf("%d ", primes[index]);
    free(arg);
}

int main(int argc, char* argv[]) {
    pthread_t th[10];
    for(int i=0; i<10; i++){
        int *var = malloc(sizeof(int));
        *var = i;
        if (pthread_create(&th[i], NULL, &routine, var) != 0) {
            perror("Failed to created thread");
        }
    }

    for (int i = 0; i < 10; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    
    return 0;
}