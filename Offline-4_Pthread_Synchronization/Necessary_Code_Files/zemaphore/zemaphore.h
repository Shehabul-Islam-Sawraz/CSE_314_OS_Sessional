#include <pthread.h>

typedef struct zemaphore {
    int counter;
    pthread_mutex_t mutex_lock;
    pthread_cond_t cond_wait;
} zem_t;

void zem_init(zem_t *, int);
void zem_up(zem_t *);
void zem_down(zem_t *);
