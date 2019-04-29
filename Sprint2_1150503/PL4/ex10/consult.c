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
#include "struct.h"
#include "struct2.h"

char *shm_names[1] = {"/shm_ex10"};
char *sem_names[1] = {"/sem_ex10"};
int sem_values[1] = {1};

int main(){
	int fd = shm_open(shm_names[0], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if(fd == -1){
    perror("Opening fd error \n");
		exit(EXIT_FAILURE);
	}

	int size = sizeof(SharedData);

	if(ftruncate(fd, size) == -1){
		perror("ftruncate ERROR\n");
		exit(0);
	}

	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(sd == NULL){
		printf("MMAP ERROR\n");
		exit(0);
	}

	int i;
	sem_t *sem[1];

	for(i = 0; i < 1; i++) {
		sem[i] = sem_open(sem_names[i], O_CREAT, 0644, sem_values[i]);
		if(sem[i] == SEM_FAILED){
			printf("Opening fd error\n");
			return 0;
		}
	}

	sem_wait(sem[0]);

	int number;

	printf("Insert number:\n");

	scanf("%d", &number);
	fflush(0);

	if(sd->index == 0){
		printf("No records available.\n");
	}else{
		int i;
		for(i = 0; i < sd->index; i++){
			record *r = &(sd->record[i]);
			if(r->number == number){
				printf("Record:\nNumber: %d  Name: %s  Address: %s\n", r->number, r->name, r->address);
			}
		}
	}

	sem_post(sem[0]);

	for(i = 0; i < 1; i++)
		sem_unlink(sem_names[i]);

	munmap(sd, size);
	close(fd);

	return 0;
}
