#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

int sem_values[1] = {0};

int main(){
	sem_t *sem;

  sem = sem_open("/shm_ex05", O_CREAT, 0644, sem_values[0]);
	if(sem == SEM_FAILED){
		perror("Creating Semaphore ERROR \n");
    exit(EXIT_FAILURE);
	}

	if(fork() == 0) {
		printf("I'm the child\n");

		sem_post(sem);

		exit(0);
	}

	sem_wait(sem);

	printf("I'm the father\n");

	sem_unlink("/shm_ex05");

	return 0;
}
