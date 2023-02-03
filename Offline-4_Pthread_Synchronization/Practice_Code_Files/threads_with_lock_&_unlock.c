#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t mutex; // Used for locking a thread so that race condition do not occur

void* routine() {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex); // Locks a thread so that other threads can't interrupt 
                                    // before the execution of current thread is finished
        counter++;
        pthread_mutex_unlock(&mutex); // Unlocks the thread
    }
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2;
    pthread_mutex_init(&mutex, NULL); // Initializing lock, before thread is initiated
    if (pthread_create(&p1, NULL, &routine, NULL) != 0) {
        return 1;
    }
    if (pthread_create(&p2, NULL, &routine, NULL) != 0) {
        return 2;
    }
    if (pthread_join(p1, NULL) != 0) {
        return 5;
    }
    if (pthread_join(p2, NULL) != 0) {
        return 6;
    }
    pthread_mutex_destroy(&mutex); // Destroy the lock
    printf("Number of counter: %d\n", counter);
    return 0;
}