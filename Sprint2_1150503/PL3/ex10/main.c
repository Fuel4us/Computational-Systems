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

#define EXCHANGES 30

int main(){
	int data_size = sizeof(exchanges);

	int fd = shm_open("/shm_ex10", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(fd, data_size) == -1){
		perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
	}

exchanges* shared_data = (exchanges*) mmap(NULL, data_size, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0);	if(shared_data == MAP_FAILED){
    if(shared_data == MAP_FAILED)
    perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}

	shared_data->producer = 0;
	shared_data->consumer = 0;

	if(fork() == 0) {
		srand((unsigned) time(NULL));
		while(shared_data->producer < EXCHANGES) {
			while(shared_data->producer - shared_data->consumer >= 10);

			shared_data->num[shared_data->producer %10]= rand() % 1000;
			shared_data->producer++;
		}
		wait(NULL);
	} else {
		while(shared_data->consumer < EXCHANGES) {
			while(shared_data->consumer >= shared_data->producer);

			printf("num[%d] = %d\n", shared_data->consumer + 1, shared_data->num[shared_data->consumer %10]);
			shared_data->consumer++;
		}
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

	shm_unlink("/shm_ex10");

	return 0;
}
