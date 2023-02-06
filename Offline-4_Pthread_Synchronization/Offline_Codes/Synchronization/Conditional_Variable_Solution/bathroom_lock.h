#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

struct bathroom_lock
{
    int men;
    int women;
    int curr_size, capacity;
    char inUsedBy;
    pthread_mutex_t mutex_lock;
    pthread_cond_t men_wait;
    pthread_cond_t women_wait;
};

void InitalizeMenWomenLock(struct bathroom_lock * lock);
void MenLock(struct bathroom_lock * lock);
void MenUnlock(struct bathroom_lock * lock);
void WomenLock(struct bathroom_lock * lock);
void WomenUnlock(struct bathroom_lock * lock);