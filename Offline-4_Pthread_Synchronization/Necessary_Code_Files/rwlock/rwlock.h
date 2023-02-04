#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

struct read_write_lock
{
    unsigned int writer_threads;
    unsigned int reader_threads;
    unsigned int waiting_writer_threads;
    unsigned int waiting_reader_threads;
    pthread_mutex_t mutex_lock;
    pthread_cond_t cond_read_wait;
    pthread_cond_t cond_write_wait;
};

void InitalizeReadWriteLock(struct read_write_lock * rw);
void ReaderLock(struct read_write_lock * rw);
void ReaderUnlock(struct read_write_lock * rw);
void WriterLock(struct read_write_lock * rw);
void WriterUnlock(struct read_write_lock * rw);
