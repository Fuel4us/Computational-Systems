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
		perror("Open error\n");
		exit(0);
	}

	int size = sizeof(SharedData);

	if(ftruncate(fd, size) == -1){
		perror("Ftruncate error\n");
		exit(0);
	}

	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(sd == NULL){
		printf("Erro no mmap\n");
		exit(0);
	}

	int i;
	sem_t *sem[1];

	for(i = 0; i < 1; i++) {
		sem[i] = sem_open(sem_names[i], O_CREAT, 0644, sem_values[i]);
		if(sem[i] == SEM_FAILED){
			printf("Erro no sem_open\n");
			return 0;
		}
	}

	sem_wait(sem[0]);

	record *r = &(sd->record[sd->index]);

	printf("Insert number:\n");

	scanf("%d", &(r->number));
	fflush(0);

	printf("Insert name:\n");

	char aux[50];
	scanf("%s", aux);
	strcpy(r->name, aux);
	fflush(0);

	printf("Insert address:\n");

  char aux2[100];
	scanf("%s", aux2);
	strcpy(r->address, aux2);
	fflush(0);

	sd->index++;

	sem_post(sem[0]);

	for(i = 0; i < 1; i++)
		sem_unlink(sem_names[i]);

	munmap(sd, size);
	close(fd);

	return 0;
}
