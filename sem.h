struct semaphore {	
	
	pthread_mutex_t lock;
    pthread_cond_t nonzero;
	unsigned count;
	unsigned int state;
};
typedef struct semaphore semaphore_t;

struct barrier {	
	
	pthread_mutex_t lock;
    pthread_cond_t nonzero;
	unsigned count;
	unsigned max;	
};
typedef struct barrier barrier_t;

semaphore_t* semaphore_create(char *semaphore_name, int initval);
semaphore_t* semaphore_open(char *semaphore_name);
void semaphore_post(semaphore_t *semap);
void semaphore_wait(semaphore_t *semap);
void semaphore_close(semaphore_t *semap);

barrier_t* barrier_create(char *barrier_name, int initval);
barrier_t* barrier_open(char *barrier_name);
void barrier_wait(barrier_t *bmap);
void barrier_close(barrier_t *bmap);
