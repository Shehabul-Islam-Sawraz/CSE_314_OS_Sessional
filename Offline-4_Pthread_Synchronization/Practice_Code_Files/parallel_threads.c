#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* routine() {
    printf("Hello from threads\n");
    sleep(3);
    printf("Ending thread\n");
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
    return 0;
}