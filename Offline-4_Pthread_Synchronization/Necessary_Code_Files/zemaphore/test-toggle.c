#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include "zemaphore.h"

#define NUM_THREADS 3
#define NUM_ITER 10

zem_t zemaphore_t[NUM_THREADS];

void *justprint(void *data)
{
    int thread_id = *((int *)data);

    for(int i=0; i < NUM_ITER; i++) {
        zem_down(&zemaphore_t[thread_id]);
        printf("This is thread %d\n", thread_id);
        zem_up(&zemaphore_t[(thread_id+1)%NUM_THREADS]);
    }
    return 0;
}

int main(int argc, char *argv[])
{

    pthread_t mythreads[NUM_THREADS];
    int mythread_id[NUM_THREADS];

    
    for(int i =0; i < NUM_THREADS; i++) {
        mythread_id[i] = i;
        zem_init(&zemaphore_t[i], 0);
        pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }
    
    for(int i =0; i < NUM_THREADS; i++) {
        pthread_join(mythreads[i], NULL);
    }
    
    return 0;
}
