#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
    s->counter = value;
    if (pthread_mutex_init(&s->mutex_lock, NULL) != 0) {
        perror("Failed to initialize mutex lock!");
        exit(1);
    }
    if (pthread_cond_init(&s->cond_wait, NULL) != 0) {                                    
        perror("Failed to initialize conditional variable!");                                        
        exit(1);                                                                    
    }   
    
}

void zem_down(zem_t *s) {
    pthread_mutex_lock(&s->mutex_lock);
    while(s->counter<=0){
        pthread_cond_wait(&s->cond_wait,&s->mutex_lock);
    }
    s->counter--;
    pthread_mutex_unlock(&s->mutex_lock);
}

void zem_up(zem_t *s) {
    pthread_mutex_lock(&s->mutex_lock);
    s->counter++;
    pthread_cond_signal(&s->cond_wait);
    pthread_mutex_unlock(&s->mutex_lock);
}
