#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "student.h"

int main(void){
	int fd, size = sizeof(Student);

	fd = shm_open("/shm_ex01", O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1) {
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(fd, size) == -1) {
		perror("Space in Shared Memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	Student *student = (Student*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(student == NULL) {
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}

	printf("Name: %s", student->name);
	printf("Nº Student:%d\n", student->number);

	if(munmap(student, size) == -1) {
		perror("Cant read object.\n");
		exit(EXIT_FAILURE);
	}

	if(close(fd) == -1) {
		perror("Closing shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	shm_unlink("/shm_ex01");

	return 0;
}
