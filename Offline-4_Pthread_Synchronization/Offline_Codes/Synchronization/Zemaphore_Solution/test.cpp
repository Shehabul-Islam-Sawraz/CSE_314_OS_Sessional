#include "semaphore.cpp"
#include <ctime>
#include <algorithm>

using namespace std;

long index_var;
long *menAcquireTime;
long *menReleaseTime;
long *womenAcquireTime;
long *womenReleaseTime;

struct bathroom_lock lock;
pthread_spinlock_t spinlock;

void *Men(void* arg)
{
	int threadNUmber = *((int *)arg);

	// Occupying the Lock
	MenLock(&lock);

	pthread_spin_lock(&spinlock);
	menAcquireTime[threadNUmber] = index_var;
	index_var++;
	pthread_spin_unlock(&spinlock);

	// printf("Men: %d has acquired the lock\n", threadNUmber);
	usleep(10000);

	pthread_spin_lock(&spinlock);
	menReleaseTime[threadNUmber] = index_var;
	index_var++;
	pthread_spin_unlock(&spinlock);

	// Releasing the Lock
	MenUnlock(&lock);
	// printf("Men: %d has released the lock\n",threadNUmber);
	return 0;
}

void *Women(void* arg)
{
	int threadNUmber = *((int *)arg);

  	// Occupying the Lock
	WomenLock(&lock);

	pthread_spin_lock(&spinlock);
	womenAcquireTime[threadNUmber] = index_var;
	index_var++;
	pthread_spin_unlock(&spinlock);

	// printf("Women: %d has acquired the lock\n",threadNUmber);
  	usleep(10000);

	pthread_spin_lock(&spinlock);
	womenReleaseTime[threadNUmber] = index_var;
	index_var++;
	pthread_spin_unlock(&spinlock);

	// Releasing the Lock
	WomenUnlock(&lock);
	// printf("Women: %d has released the lock\n",threadNUmber);
	return 0;
}

int main(int argc, char *argv[])
{

	int *threadNUmber;
	pthread_t *threads;

	InitalizeMenWomenLock(&lock);
	pthread_spin_init(&spinlock, 0);

	int men_num_threads;
	int women_num_threads;

	men_num_threads = atoi(argv[1]);
	women_num_threads = atoi(argv[2]);

	index_var = 0;
	menAcquireTime = new long[men_num_threads*2];
	menReleaseTime = new long[men_num_threads*2];
	womenAcquireTime = new long[women_num_threads];
	womenReleaseTime = new long[women_num_threads];

	int num_threads = 2*men_num_threads + women_num_threads;

	threads = (pthread_t*) malloc(num_threads * (sizeof(pthread_t)));

	int count = 0;
	for(int i=0; i<men_num_threads; i++)
	{
		int *arg = (int *)malloc((sizeof(int)));
		if (arg == NULL){
			printf("Couldn't allocate memory for thread arg.\n");
			exit(EXIT_FAILURE);
		}
		*arg = i;
		int ret = pthread_create(threads+count, NULL, Men, (void*) arg);
		if(ret){
			printf("Error - pthread_create() return code: %d\n",ret);
			exit(EXIT_FAILURE);
		}
		count++;
	}

	for(int i=0; i<women_num_threads; i++)
	{
		int *arg = (int *)malloc((sizeof(int)));
		if (arg == NULL){
			printf("Couldn't allocate memory for thread arg.\n");
			exit(EXIT_FAILURE);
		}
		*arg = i;
		int ret = pthread_create(threads+count, NULL, Women,( void*) arg);
		if(ret)
		{
			printf("Error - pthread_create() return code: %d\n",ret);
			exit(EXIT_FAILURE);
		}
		count++;
	}

	for(int i=0; i<men_num_threads; i++)
	{
		int *arg = (int *)malloc((sizeof(int)));
		if (arg == NULL)
		{
			printf("Couldn't allocate memory for thread arg.\n");
			exit(EXIT_FAILURE);
		}
		*arg = men_num_threads + i;
		int ret = pthread_create(threads+count,NULL,Men,(void*) arg);
		if(ret)
		{
			printf("Error - pthread_create() return code: %d\n",ret);
			exit(EXIT_FAILURE);
		}
		count++;
	}

	for(int i=0; i<num_threads; i++)
		pthread_join(threads[i],NULL);

	for(int i=0; i<men_num_threads*2; i++)
		printf("Men %d Lock Time: %ld Unlock Time: %ld \n", i, menAcquireTime[i], menReleaseTime[i]);

	for (int i = 0; i<women_num_threads; i++)
		printf("Women %d Lock Time: %ld Unlock Time: %ld \n", i, womenAcquireTime[i], womenReleaseTime[i]);
}