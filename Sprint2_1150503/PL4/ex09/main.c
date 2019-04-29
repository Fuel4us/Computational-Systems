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

int sem_values[2] = {0,0};
char *sem_names[2] = {"/sem_ex09_1", "/sem_ex09_2"};

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

    for(i = 0; i < 2; i++) {
  		if(fork() == 0) {
  			if(i == 0) {
  				printf("Child %d - buy_chips();\n", i);

  				sem_post(sem[1]);

  				sem_wait(sem[0]);

  				printf("Child %d - eat_and_drink();\n", i);
  			} else {
  				printf("Child %d - buy_beer();\n", i);

  				sem_post(sem[0]);

  				sem_wait(sem[1]);

  				printf("Child %d - eat_and_drink();\n", i);
  			}

  			exit(0);
  		}
  	}

  	for(i = 0; i < 2; i++)
  		wait(NULL);

  	for(i = 0; i < 2; i++)
  		sem_unlink(sem_names[i]);

	return 0;
}
