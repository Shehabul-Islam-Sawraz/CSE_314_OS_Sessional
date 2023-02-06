#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw) {
    //	Write the code for initializing your read-write lock.
    rw->reader_threads = 0;
    rw->writer_threads = 0;
    rw->waiting_reader_threads = 0;
    rw->waiting_writer_threads = 0;
    pthread_mutex_init(&rw->mutex_lock, NULL);
    pthread_cond_init(&rw->cond_read_wait, NULL);
    pthread_cond_init(&rw->cond_write_wait, NULL);
}

void ReaderLock(struct read_write_lock * rw) {
    //	Write the code for aquiring read-write lock by the reader.
    pthread_mutex_lock(&rw->mutex_lock); // Acquire the lock
    
    // If writers already have the lock and a reader wants the lock
    // then we go for a wait till writer completes it's execution
    while(rw->writer_threads > 0){
        rw->waiting_reader_threads++;
        pthread_cond_wait(&rw->cond_read_wait, &rw->mutex_lock);
        rw->waiting_reader_threads--;
    }

    rw->reader_threads++;
    pthread_mutex_unlock(&rw->mutex_lock); // Release the lock
}

void ReaderUnlock(struct read_write_lock * rw) {
    //	Write the code for releasing read-write lock by the reader.
    pthread_mutex_lock(&rw->mutex_lock); // Acquire the lock
    rw->reader_threads--;

    // If this is the last read thread, so it is mandatory that reader_threads_waiting = 0
	// because otherwise they would have already entered.
	// So, if there are any waiting writer threads, we would just wake one of them up
    if(rw->reader_threads == 0 && rw->waiting_writer_threads > 0) {
        pthread_cond_signal(&rw->cond_write_wait);
    }

    pthread_mutex_unlock(&rw->mutex_lock); // Release the lock
}

void WriterLock(struct read_write_lock * rw) {
    //	Write the code for aquiring read-write lock by the writer.
    pthread_mutex_lock(&rw->mutex_lock); // Acquire the lock
    
    // If a thread is already writing or reading, then we can't give 
    // access to another write thread 
    while(rw->writer_threads > 0 || rw->reader_threads > 0 || rw->waiting_reader_threads > 0) {
        rw->waiting_writer_threads++;
        pthread_cond_wait(&rw->cond_write_wait, &rw->mutex_lock);
        rw->waiting_writer_threads--;
    }

    rw->writer_threads++;
    pthread_mutex_unlock(&rw->mutex_lock); // Release the lock
}

void WriterUnlock(struct read_write_lock * rw) {
    //	Write the code for releasing read-write lock by the writer.
    pthread_mutex_lock(&rw->mutex_lock); // Acquire the lock
    rw->writer_threads--;

    // If there are any waiting reader threads, we wake up all the 
    // reading threads 
    if(rw->waiting_reader_threads > 0) {
        pthread_cond_broadcast(&rw->cond_read_wait);
    }
    // Otherwise, if there are any writer threads, we wake them up
    else if(rw->waiting_writer_threads > 0) {
        pthread_cond_signal(&rw->cond_write_wait);
    }

    pthread_mutex_unlock(&rw->mutex_lock); // Release the lock
}
