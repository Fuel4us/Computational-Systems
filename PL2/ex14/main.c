#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(void){

	char string[666];
	int fd[2];
	pid_t p;

  if(pipe(fd) == -1) {
		printf("Pipe not created properly");
		exit(0);
	}

	p = fork();
	if(p == -1) {
		printf("Childs Process error");
		exit(0);
	}

	if (p > 0){
		close(fd[0]);

		int i, size;

		for(i = 0; i < 100; i++)
			size = sprintf(string, "%sLine %d\n",string, i + 1);

		write(fd[1], string, size);

		close(fd[1]);

		wait(NULL);
	} else{
		close(fd[1]);

		dup2(fd[0], 0);

		close(fd[0]);

		execlp("more", "more", NULL);

		exit(-1);
	}

	return 0;
}
