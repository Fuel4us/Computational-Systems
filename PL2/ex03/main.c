#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	int fd[2];
	pid_t p;

  char msg1[] = "Hello World";
  char msg2[] = "Goodbye!";

	if(pipe(fd) == -1) {
		printf("Pipe not created properly");
		exit(0);
	}

	p = fork();
	if(p == -1) {
		printf("Childs Process error");
		exit(0);
	}

	if(p != 0) {
		close(fd[0]);

		int size1 = sizeof(msg1), size2 = sizeof(msg2);

		write(fd[1], &size1, sizeof(int));
		write(fd[1], &msg1, sizeof(msg1));

		write(fd[1], &size2, sizeof(int));
		write(fd[1], &msg2, sizeof(msg2));

		close(fd[1]);
	} else {
		close(fd[1]);

		int size1;
		read(fd[0], &size1, sizeof(int));
		char msg1[size1];
		read(fd[0], &msg1, size1);


		int size2;
		read(fd[0], &size2, sizeof(int));
		char msg2[size2];
		read(fd[0], &msg2, size2);

		printf("1: %s\n2: %s\n", msg1, msg2);

		close(fd[0]);
	}

	return 0;
}
