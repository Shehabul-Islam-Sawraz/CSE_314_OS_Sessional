#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "zemaphore.h"

using namespace std;

struct bathroom_lock
{
    int men;
    int women;
    zem_t menlock;
    zem_t womenlock;
};

void InitalizeMenWomenLock(struct bathroom_lock * lock);
void MenLock(struct bathroom_lock * lock);
void MenUnlock(struct bathroom_lock * lock);
void WomenLock(struct bathroom_lock * lock);
void WomenUnlock(struct bathroom_lock * lock);