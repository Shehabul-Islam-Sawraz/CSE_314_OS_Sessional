#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

int item_to_produce, item_consumed, curr_buf_size;
int total_items, max_buf_size, num_workers, num_masters;

int *buffer;

pthread_mutex_t mutexReq;
pthread_cond_t condFull;
pthread_cond_t condEmpty;

void print_produced(int num, int master) {
    printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {
    printf("Consumed %d by worker %d\n", num, worker);
}


// produce items and place in buffer
// modify code below to synchronize correctly
void *generate_requests_loop(void *data) {
    int thread_id = *((int *)data);
    while(1) {
        pthread_mutex_lock(&mutexReq); // Acquire the lock

        // Checks if we have already produced our required number of items
        // If yes, then the work of the thread is done 
        if(item_to_produce >= total_items) {
            pthread_mutex_unlock(&mutexReq);
	        break;
        }

        while((curr_buf_size >= max_buf_size) && (item_to_produce < total_items)){
            pthread_cond_wait(&condEmpty, &mutexReq); // Wait until the buffer gets an empty space
        }

        if(item_to_produce >= total_items) {
            pthread_mutex_unlock(&mutexReq);
	        break;
        }

        buffer[curr_buf_size++] = item_to_produce;
        print_produced(item_to_produce, thread_id);
        item_to_produce++;

        pthread_mutex_unlock(&mutexReq);
        pthread_cond_broadcast(&condFull); // Send signal to all waiting consumer(happens
                                           // when buffer is empty) that there is some item 
                                           // in the buffer  
    }
    return 0;
}

// write function to be run by worker threads
// ensure that the workers call the function print_consumed when they consume an item
void *consume_requests_loop(void *data) {
    int thread_id = *((int *)data);
    while(1) {
        pthread_mutex_lock(&mutexReq); // Acquire the lock

        // Checks if we have already produced our required number of items
        // If yes, then the work of the thread is done 
        if(item_consumed >= total_items) {
            pthread_mutex_unlock(&mutexReq);
	        break;
        }

        while((curr_buf_size == 0) && (item_consumed < total_items)){
            pthread_cond_wait(&condFull, &mutexReq); // Wait until the buffer gets an item
        }

        if(item_consumed >= total_items) {
            pthread_mutex_unlock(&mutexReq);
	        break;
        }

        print_consumed(buffer[--curr_buf_size], thread_id);
        item_consumed++;

        pthread_mutex_unlock(&mutexReq);
        pthread_cond_broadcast(&condEmpty); // Send signal to all waiting master(happens when
                                            // buffer is full) that an item is consumed
    }
    return 0;
}


int main(int argc, char *argv[])
{
    int* master_thread_id;
    int* worker_thread_id;
    pthread_t* master_thread;
    pthread_t* worker_thread;
    item_to_produce = 0;
    item_consumed = 0;
    curr_buf_size = 0;
  
    int i;
  
    if (argc < 5) {
        printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./master-worker 10000 1000 4 3\n");
        exit(1);
    }
    else {
        num_masters = atoi(argv[4]);
        num_workers = atoi(argv[3]);
        total_items = atoi(argv[1]);
        max_buf_size = atoi(argv[2]);
    }
    
    // Initializing mutex lock & conditional variables
    pthread_mutex_init(&mutexReq, NULL);
    pthread_cond_init(&condFull, NULL);
    pthread_cond_init(&condEmpty, NULL);

    buffer = (int *)malloc (sizeof(int) * max_buf_size);

    // create master producer threads
    master_thread_id = (int *)malloc(sizeof(int) * num_masters);
    master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
    for (i = 0; i < num_masters; i++)
        master_thread_id[i] = i;

    for (i = 0; i < num_masters; i++)
        pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
  
    // create worker consumer threads
    worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
    worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
    for (i = 0; i < num_workers; i++)
        worker_thread_id[i] = i;

    for (i = 0; i < num_workers; i++)
        pthread_create(&worker_thread[i], NULL, consume_requests_loop, (void *)&worker_thread_id[i]);
  
    // wait for all threads to complete
    // Join master threads
    for (i = 0; i < num_masters; i++){
        pthread_join(master_thread[i], NULL);
        printf("master %d joined\n", i);
    }

    // Join worker threads
    for (i = 0; i < num_workers; i++){
        pthread_join(worker_thread[i], NULL);
        printf("worker %d joined\n", i);
    }
  
    /*----Deallocating Buffers---------------------*/
    free(buffer);
    free(master_thread_id);
    free(master_thread);
    free(worker_thread_id);
    free(worker_thread);

    // Destroying mutex lock and conditional variables
    pthread_mutex_destroy(&mutexReq);
    pthread_cond_destroy(&condFull);
    pthread_cond_destroy(&condEmpty);

    return 0;
}
