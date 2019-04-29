#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "struct.h"

#define STR_SIZE 50
#define NR_DISC 10

int main(){
	int data_size = sizeof(aluno), i;

	int fd = shm_open("/shm_ex08", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(fd, data_size) == -1){
		perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
	}

aluno* shared_data = (aluno*) mmap(NULL, data_size, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0);	if(shared_data == MAP_FAILED){
    if(shared_data == MAP_FAILED)
    perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}

  pid_t pid;

	if((pid = fork()) == -1){
			perror("Fork failed\n");
			exit(EXIT_FAILURE);
	}

  if(pid > 0){
		printf("\nNumber:");
		fflush(0);
		scanf("%d", &shared_data->numero);

		printf("\nName:");
		fflush(0);
		scanf("%s", shared_data->nome);

		for(i = 0; i < NR_DISC; i++){
			printf("\nGrade %d: ", i + 1);
			fflush(0);
			scanf("%d", &shared_data->disciplinas[i]);
		}

		shared_data->endGrades = 1;

		wait(NULL);
	}else if(pid == 0){
		while(shared_data->endGrades == 0);

		int highest = shared_data->disciplinas[0], lowest = shared_data->disciplinas[0], soma = shared_data->disciplinas[0];
		float media;

		for(i = 1; i < NR_DISC; i++){
			if(shared_data->disciplinas[i] < lowest)
				lowest = shared_data->disciplinas[i];
			if(shared_data->disciplinas[i] > highest)
				highest = shared_data->disciplinas[i];
			soma += shared_data->disciplinas[i];
		}

		media = soma / NR_DISC;

		printf("\nBest grade: %d", highest);
		printf("\nLowest grade: %d", lowest);
		printf("\nAverage: %f\n", media);

		exit(0);
	}

	if(munmap(shared_data, data_size) == -1){
		perror("Cant read object.\n");
		exit(EXIT_FAILURE);
	}

	if(close(fd) == -1){
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	shm_unlink("/shm_ex08");

	return 0;
}
