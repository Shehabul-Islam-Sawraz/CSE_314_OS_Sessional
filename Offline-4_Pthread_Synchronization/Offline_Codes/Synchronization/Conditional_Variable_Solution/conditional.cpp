#include "bathroom_lock.h"

#define BATHROOM_CAPACITY 2

void InitalizeMenWomenLock(struct bathroom_lock * lock)
{
    //	Write the code for initializing your men-women lock.
    lock->men = 0;
    lock->women = 0;
    lock->capacity = BATHROOM_CAPACITY;
    lock->curr_size = 0;
    lock->inUsedBy = ' ';

    pthread_mutex_init(&lock->mutex_lock, NULL);
    pthread_cond_init(&lock->men_wait, NULL);
    pthread_cond_init(&lock->women_wait, NULL);
}

void MenLock(struct bathroom_lock * lock)
{
    pthread_mutex_lock(&lock->mutex_lock);

    while((lock->curr_size == lock->capacity) || (lock->curr_size>0 && !('M' == lock->inUsedBy))){
        pthread_cond_wait(&lock->men_wait, &lock->mutex_lock);
    }

    if(lock->curr_size == 0){
        lock->inUsedBy = 'M';
    }

    lock->curr_size++;

    pthread_mutex_unlock(&lock->mutex_lock);
}

void MenUnlock(struct bathroom_lock * lock)
{
    pthread_mutex_lock(&lock->mutex_lock);

    lock->curr_size--;

    if(lock->curr_size == 0){
        pthread_cond_broadcast(&lock->women_wait);
    }
    else{
        pthread_cond_signal(&lock->men_wait);
    }

    pthread_mutex_unlock(&lock->mutex_lock);
}

void WomenLock(struct bathroom_lock * lock)
{
    pthread_mutex_lock(&lock->mutex_lock);

    while((lock->curr_size == lock->capacity) || (lock->curr_size>0 && !('W' == lock->inUsedBy))){
        pthread_cond_wait(&lock->women_wait, &lock->mutex_lock);
    }

    if(lock->curr_size == 0){
        lock->inUsedBy = 'W';
    }

    lock->curr_size++;

    pthread_mutex_unlock(&lock->mutex_lock);
}

void WomenUnlock(struct bathroom_lock * lock)
{
    pthread_mutex_lock(&lock->mutex_lock);

    lock->curr_size--;

    if(lock->curr_size == 0){
        pthread_cond_broadcast(&lock->men_wait);
    }
    else{
        pthread_cond_signal(&lock->women_wait);
    }

    pthread_mutex_unlock(&lock->mutex_lock);
}
