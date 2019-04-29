#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "student.h"

int main(void){

	int fd = shm_open("/shm_ex01", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1) {
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	int size = sizeof(Student);

	if(ftruncate(fd, size) == -1) {
		perror("Space in Shared Memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	Student *student = (Student*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(student == NULL) {
		perror("Error maping the object.");
		exit(EXIT_FAILURE);
	}

	printf("\nStudent name:\n");
	fgets(student->name, sizeof(student->name), stdin);

	printf("Student number:\n");
	scanf("%d", &student->number);

	if(munmap(student, size) == -1) {
		perror("Cant read object.\n");
		exit(EXIT_FAILURE);
	}

	if(close(fd) == -1) {
		perror("Closing shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
