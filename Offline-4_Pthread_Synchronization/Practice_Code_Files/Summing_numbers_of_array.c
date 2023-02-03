#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/**
 * @Problem Statement:
 * 
 * Create 2 threads and sum the numbers of an array
 * of size 10 diving the array into two parts
*/

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
    //sleep(1);
    int index = *(int*)arg;
    int sum = 0;
    for(int j=0; j<5; j++){
        sum += primes[index+j];
    }
    printf("Local sum: %d\n", sum);
    *(int *)arg = sum;
    return arg;
}

int main(int argc, char* argv[]) {
    pthread_t th[2];
    for(int i=0; i<2; i++){
        int *var = malloc(sizeof(int));
        *var = i*5;
        if (pthread_create(&th[i], NULL, &routine, var) != 0) {
            perror("Failed to created thread");
        }
    }
    int sum = 0;
    for (int i = 0; i < 2; i++) {
        int* result;
        if (pthread_join(th[i], (void **)&result) != 0) {
            perror("Failed to join thread");
        }
        sum += *result;
        free(result);
    }
    printf("Global Sum: %d\n", sum);
    return 0;
}