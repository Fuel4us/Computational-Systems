#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void){

	int fd[3 - 1][2], i;
	pid_t p;

	for (i = 0; i < 3 - 1; i++){
		if (pipe(fd[i]) == -1){
			printf("Pipe not created properly");
			exit(0);
		}
	}

	for (i = 0; i < 3; i++){
		if ((p = fork()) == -1){
			printf("Childs Process error id %d.", i + 1);
			exit(0);
		}

		if (p == 0){
			if (i == 0){
				close(fd[1][0]);
				close(fd[1][1]);

				close(fd[0][0]);

				dup2(fd[0][1], 1);

				close(fd[0][1]);

				execlp("ls", "ls", "-la", NULL);
			} else if (i == 1){
				close(fd[0][1]);
				close(fd[1][0]);

				dup2(fd[0][0], 0);
				dup2(fd[1][1], 1);

				close(fd[0][0]);
				close(fd[1][1]);

				execlp("sort", "sort", NULL);
			} else if (i == 2){
				close(fd[0][0]);
				close(fd[0][1]);

				close(fd[1][1]);

				dup2(fd[1][0], 0);

				close(fd[1][0]);

				execlp("wc", "wc", "-l", NULL);
			}
		}
	}
	wait(NULL);

	return 0;
}
