#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include "sem.h"
#include <stdio.h>


semaphore_t * semaphore_create(char *semaphore_name, int initval)
{
	int fd;
    semaphore_t *semap;
    pthread_mutexattr_t psharedm;
    pthread_condattr_t psharedc;

	remove(semaphore_name);
    fd = open(semaphore_name, O_RDWR | O_CREAT | O_EXCL, 0666);
    if (fd < 0)
        return (NULL);
    (void) ftruncate(fd, sizeof(semaphore_t));    
    (void) pthread_mutexattr_init(&psharedm);
    (void) pthread_mutexattr_setpshared(&psharedm, PTHREAD_PROCESS_SHARED);
    (void) pthread_condattr_init(&psharedc);
    (void) pthread_condattr_setpshared(&psharedc, PTHREAD_PROCESS_SHARED);
    semap = (semaphore_t *) mmap(NULL, sizeof(semaphore_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close (fd);
    (void) pthread_mutex_init(&semap->lock, &psharedm);
    (void) pthread_cond_init(&semap->nonzero, &psharedc);
    semap->count = initval;
    semap->state = 0;
    
    return (semap);
}

barrier_t* barrier_create(char *barrier_name, int initval)
{
	int fd;
    barrier_t *bmap;
    pthread_mutexattr_t psharedm;
    pthread_condattr_t psharedc;

	remove(barrier_name);
    fd = open(barrier_name, O_RDWR | O_CREAT | O_EXCL, 0666);
    if (fd < 0)
        return (NULL);
    (void) ftruncate(fd, sizeof(barrier_t));    
    (void) pthread_mutexattr_init(&psharedm);
    (void) pthread_mutexattr_setpshared(&psharedm, PTHREAD_PROCESS_SHARED);
    (void) pthread_condattr_init(&psharedc);
    (void) pthread_condattr_setpshared(&psharedc, PTHREAD_PROCESS_SHARED);
    bmap = (barrier_t *) mmap(NULL, sizeof(semaphore_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close (fd);
    (void) pthread_mutex_init(&bmap->lock, &psharedm);
    (void) pthread_cond_init(&bmap->nonzero, &psharedc);
    bmap->count = 0;
    bmap->max = initval;
    
    return (bmap);
}


semaphore_t* semaphore_open(char *semaphore_name)
{
    int fd;
    semaphore_t *semap;
    fd = open(semaphore_name, O_RDWR, 0666);
    if (fd < 0)
        return (NULL);
    semap = (semaphore_t *) mmap(NULL, sizeof(semaphore_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close (fd);
    return (semap);
}

barrier_t* barrier_open(char *barrier_name)
{
    int fd;
    barrier_t *bmap;
    fd = open(barrier_name, O_RDWR, 0666);
    if (fd < 0)
        return (NULL);
    bmap = (barrier_t *) mmap(NULL, sizeof(barrier_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close (fd);
    return (bmap);
}


void semaphore_post(semaphore_t *semap)
{
    pthread_mutex_lock(&semap->lock);
    if (semap->count == 0)
        pthread_cond_signal(&semap->nonzero);
    semap->count++;
    pthread_mutex_unlock(&semap->lock);
}


void semaphore_wait(semaphore_t *semap)
{
    pthread_mutex_lock(&semap->lock);
    while (semap->count == 0)
        pthread_cond_wait(&semap->nonzero, &semap->lock);
    semap->count--;
    pthread_mutex_unlock(&semap->lock);
}

void barrier_wait(barrier_t *bmap){
	pthread_mutex_lock(&bmap->lock);
	bmap->count++;
	if (bmap->count == bmap->max)
		pthread_cond_broadcast(&bmap->nonzero);
	else {
		while(bmap->count != bmap->max)
			pthread_cond_wait(&bmap->nonzero, &bmap->lock);
	}
	pthread_mutex_unlock(&bmap->lock);	
}


void semaphore_close(semaphore_t *semap)
{
    munmap((void *) semap, sizeof(semaphore_t));
}

void barrier_close(barrier_t *bmap)
{
    munmap((void *) bmap, sizeof(barrier_t));
}
