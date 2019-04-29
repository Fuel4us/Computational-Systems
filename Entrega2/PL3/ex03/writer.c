#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM 10

int main(void){

	int fd = shm_open("/shm_ex03", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1) {
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	int size = NUM * sizeof(int);

	if(ftruncate(fd, size) == -1) {
		perror("Space in Shared Memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	int *array = (int*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(array == NULL) {
		perror("Error maping the object.");
		exit(EXIT_FAILURE);
	}

	int i;
	srand((unsigned) time(NULL));
	for(i=0; i<10; i++)
		array[i]= rand() % 21;

	printf("Array numbers.\n");
	for(i = 0; i < NUM - 1; i++)
		printf("%d, ", array[i]);
		printf("%d\n", array[NUM - 1]);

	if(munmap(array, size) == -1) {
		perror("Cant read object.\n");
		exit(EXIT_FAILURE);
	}

	if(close(fd) == -1) {
		perror("Closing shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
