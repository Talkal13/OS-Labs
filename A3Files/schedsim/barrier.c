#include "barrier.h"
#include <errno.h>


#ifdef SEM_BARRIER

/* Wrapper functions to use pthread barriers */

int sys_barrier_init(sys_barrier_t* barrier, unsigned int nr_threads)
{
	sem_init(&barrier->mtx, 0, 1);
	sem_init(&barrier->queue, 0, 0);
	barrier->max_threads = nr_threads;
	barrier->nr_threads_arrived = 0;
	return 0;
}

int sys_barrier_destroy(sys_barrier_t* barrier)
{
	sem_destroy(&barrier->mtx);
	sem_destroy(&barrier->queue);
	return 0;
}

int sys_barrier_wait(sys_barrier_t *barrier)
{
	sem_wait(&barrier->mtx);
	barrier->nr_threads_arrived++;
	sem_post(&barrier->mtx);
	if (barrier->nr_threads_arrived != barrier->max_threads) {
		
		sem_wait(&barrier->queue);
	}
	else {
		sem_wait(&barrier->mtx);
		barrier->nr_threads_arrived = 0;
		sem_post(&barrier->mtx);
		sem_post(&barrier->queue);
	}
	if (barrier->nr_threads_arrived == 0)
		sem_post(&barrier->queue);
	return 0;
}

#else


/* Barrier initialization function */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
	/* Initialize fields in sys_barrier_t
	     ... To be completed ....
	*/
	pthread_mutex_init(&barrier->mutex, NULL);
	pthread_cond_init(&barrier->cond, NULL);
	barrier->max_threads = nr_threads;
	barrier->nr_threads_arrived = 0;
	return 0;
}

/* Destroy barrier resources */
int sys_barrier_destroy(sys_barrier_t *barrier)
{
	/* Destroy synchronization resources associated with the barrier
	      ... To be completed ....
	*/
	pthread_mutex_destroy(&barrier->mutex);
	pthread_cond_destroy(&barrier->cond);
	return 0;
}

/* Main synchronization operation */
int sys_barrier_wait(sys_barrier_t *barrier)
{
	/* Implementation outline:
	   - Every thread arriving at the barrier acquires the lock and increments the nr_threads_arrived
	    counter atomically
	     * In the event this is not the last thread to arrive at the barrier, the thread
	       must block in the condition variable
	     * Otherwise...
	        1. Reset the barrier state in preparation for the next invocation of sys_barrier_wait() and
	        2. Wake up all threads blocked in the barrier
	   - Don't forget to release the lock before returning from the function

	    ... To be completed ....
	*/
	pthread_mutex_lock(&barrier->mutex);
	barrier->nr_threads_arrived++;

	if (barrier->nr_threads_arrived != barrier->max_threads) {
		pthread_cond_wait(&barrier->cond, &barrier->mutex);
	}
	else {
		barrier->nr_threads_arrived = 0;
		pthread_cond_broadcast(&barrier->cond);
	}

	pthread_mutex_unlock(&barrier->mutex);

	return 0;
}

#endif /* POSIX_BARRIER */
