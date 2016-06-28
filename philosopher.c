#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include "sem.h"
 
#define THINKING 0
#define HUNGRY 1
#define EATING 2

semaphore_t *semap;
barrier_t *bmap;
int N, M, i;

char sem_name[30], mut_name[30], bar_name[30];

void think(int);
void eat(int);
void philosopher(int);
void take_forks(int);
void put_forks(int);
void test(int);
void sem_create(int);
void bar_create(int);
void s_wait();
void s_post();
void update_state(int);
int check_state(int);
void b_wait();
char get_sem(int);

int main(int argc, char* argv[]) {	
	
	N = atoi(argv[1]);
 	M = atoi(argv[2]);	
 	bar_create(N);
	/* create mutex 
	Negative value creates mutex 
	and postive creates semaphore */
	sem_create(-1);  
	int j;
	for (j = 0; j < N; j++){
		sem_create(j);
	}
	pid_t pid;		
	int status, k;	
	char postfix[5];																																																																																																																																																																																																															
	for (i = 0; i < N; i++){
		pid = fork();
		if(pid == 0){					
			snprintf(postfix,5,"%d",i);
			strcpy(sem_name,"/tmp/semaphore");
			strcat(sem_name, postfix);	
			philosopher(i);
		}
		else if(pid == -1) {
			perror("fork");
			exit(1);
		}
	}
	
	if(pid>0) {
		for(k = 0; k < N; k++){
			wait(&status);
			//printf("Exiting child %d\n", k);
		}
	}

	printf("Exiting\n");
	exit(0);
}																																																																															

void think(int p) {
	update_state(THINKING);
	printf("Philosopher %d: THINKING\n",p);
	sleep(1);
}

void eat(int p) {
	update_state(EATING);
	printf("Philosopher %d: EATING\n",p);
	sleep(1);
}

void philosopher(int p) {
	int count = M;
	/* -- Waiting at the barrier --*/
	b_wait(bar_name);	
	while (count--) {		
		think(p); 		
		take_forks(p);  
		eat(p); 			
		put_forks(p); 		
	}
	exit(1);
}

void take_forks(int p){
	s_wait(mut_name);
	update_state(HUNGRY);
	printf("Philosopher %d: HUNGRY\n",p);
	test(p); 
	s_post(mut_name);	
	s_wait(sem_name); 
	//printf("Philosopher %d taken up the forks\n",p);
}

void test(int p) {	
	if ( check_state(p) == HUNGRY && check_state((p+N-1)%N) != EATING && check_state((p+1)%N ) != EATING ) {
		char postfix[5];
		snprintf(postfix,5,"%d",p);
		char temp_name[30];																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													
		strcpy(temp_name,"/tmp/semaphore");
		strcat(temp_name, postfix);		
		s_post(temp_name);
	}
}

int check_state(int p) {
	int value;
	char postfix[5];
	snprintf(postfix,5,"%d",p);
	char temp_name[30];																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													
	strcpy(temp_name,"/tmp/semaphore");
	strcat(temp_name, postfix);		
	semap = semaphore_open(temp_name);
	value = semap->state;
	semaphore_close(semap);
	return value;
}
	
void put_forks(int p) {
	s_wait(mut_name);
	update_state(THINKING);
	//printf("Philosopher %d put down the forks\n",p);
	sleep(1);
	test((p+N-1)%N );
	test((p+1)%N );
	s_post(mut_name);
}


void sem_create(int p) {
	/*-- create semaphore (default value = 0) --*/
	if(p>=0){ 
		char postfix[5];
		snprintf(postfix,5,"%d",p);
		strcpy(sem_name,"/tmp/semaphore");
		strcat(sem_name, postfix);				
		semap = semaphore_create(sem_name,0);		
	} 
	else if(p<0) {  /*-- Create Mutex --*/		
		strcpy(mut_name, "/tmp/mutex");
		semap = semaphore_create(mut_name,1);						
	}
	
	if (semap == NULL)
		exit(1);
	semaphore_close(semap);
}

/*-- Barrier create --*/
void bar_create(int initval) {
	strcpy(bar_name,"/tmp/barrier");
	bmap = barrier_create(bar_name, N);
	if (bmap == NULL)
		exit(1);
	barrier_close(bmap);
}

/*-- Semaphore wait --*/
void s_wait(char* str) {
	semap = semaphore_open(str);
	if (semap == NULL)
		exit(1);
	semaphore_wait(semap);
	semaphore_close(semap);
}

/*-- Barrier wait --*/
void b_wait(char* str) {	
	bmap = barrier_open(str);	
	if (bmap == NULL)
		exit(1);	
	barrier_wait(bmap);
	barrier_close(bmap);	
}
	
/*-- Semaphore post --*/
void s_post(char* str) {
	semap = semaphore_open(str);
	if (semap == NULL)
		exit(1);
	semaphore_post(semap);
	semaphore_close(semap);
}

void update_state(int x) {
	semap = semaphore_open(sem_name);
	if (semap == NULL)
		exit(1);
	semap->state = x;	
	semaphore_close(semap);
}





	

