#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) 
{
    s->value = value;
    pthread_cond_init(&s->cond, NULL);
    pthread_mutex_init(&s->lock, NULL);
}

void zem_down(zem_t *s) 
{
    pthread_mutex_lock(&s->lock);
    while(s->value <= 0)
        pthread_cond_wait(&s->cond, &s->lock);
    s->value--;
    pthread_mutex_unlock(&s->lock);
}   

void zem_up(zem_t *s) 
{
    pthread_mutex_lock(&s->lock);
    s->value++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->lock);
}