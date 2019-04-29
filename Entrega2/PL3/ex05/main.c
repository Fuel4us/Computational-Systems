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

#define size 1000
#define childs 10

int main(){
	int data_size = sizeof(Data_type), i, vec[size];

	int fd = shm_open("/shm_ex05", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(fd, data_size) == -1){
		perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
	}

	Data_type* shared_data = (Data_type*) mmap(NULL, data_size, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}

	srand((unsigned) time(NULL));

	for(i = 0; i < size; i++){
		vec[i] = rand() % 1001;
	}

	pid_t pid;

	for(i = 0; i < childs; i++){
		if((pid = fork()) == -1){
			perror("Failed fork\n");
			exit(EXIT_FAILURE);
		}

		if(pid == 0){
			int x, biggest = -1;

			for(x = 0; x < size / childs; x++){
				if(vec[i * size / childs + x] > biggest)
					biggest = vec[i * size / childs + x];
			}

			shared_data->num[i] = biggest;

			exit(0);
		}
	}

	for(i = 0; i < childs; i++)
		wait(NULL);

	int biggest_global = -1;

	for(i = 0; i < childs; i++){
		if(shared_data->num[i] > biggest_global)
			biggest_global = shared_data->num[i];

		printf("Child %d: %d\n", i + 1, shared_data->num[i]);
	}

	printf("Biggest global: %d\n", biggest_global);

	if(munmap(shared_data, data_size) == -1){
		perror("Cant read object.\n");
		exit(EXIT_FAILURE);
	}

	if(close(fd) == -1){
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	shm_unlink("/shm_ex05");

	return 0;
}
