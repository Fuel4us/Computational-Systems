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

int sem_values[2] = {2,2};
char *sem_names[2] = {"/sem_ex08_C", "/sem_ex08_S"};

int main(){
  int i;
	sem_t *sem[2];

  for(i = 0; i < 2; i++) {
    sem[i] = sem_open(sem_names[i], O_CREAT | O_EXCL, 0644, sem_values[i]);
	   if(sem[i] == SEM_FAILED){
		     perror("Creating Semaphore ERROR \n");
         exit(EXIT_FAILURE);
      }
    }

  pid_t pid;

	if((pid = fork()) == -1) {
		perror("Forking ERROR\n");
		exit(0);
	}

	if(pid == 0){
		while(1){
			sem_wait(sem[0]);

			printf("C");
			fflush(0);

			sleep(1);

			int val;
			sem_getvalue(sem[1], &val);

			if(val == 1)
				sem_post(sem[1]);
			if(val == 0){
				sem_post(sem[1]);
				sem_post(sem[1]);
			}
		}

		exit(0);
	}

	while(1){
		sem_wait(sem[1]);

		printf("S");
		fflush(0);

		sleep(1);

		int val;
		sem_getvalue(sem[0], &val);

		if(val == 1)
			sem_post(sem[0]);
		if(val == 0){
			sem_post(sem[0]);
			sem_post(sem[0]);
		}
	}

	for(i = 0; i < 2; i++)
		sem_unlink(sem_names[i]);

	return 0;
}
