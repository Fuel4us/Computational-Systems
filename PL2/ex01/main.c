#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	int fd[2];
	pid_t p;

	if(pipe(fd) == -1) {
		printf("Erro a criar pipe");
		exit(0);
	}

	p = fork();
	if(p == -1) {
		printf("Erro a criar processo filho");
		exit(0);
	}

	if(p != 0) {
		close(fd[0]);

		int pid = getpid();

		int res = write(fd[1], &pid, sizeof(int));
		if(res == -1) {
			printf("Erro a escrever no pipe");
			exit(0);
		}

		printf("PID Pai: %d\n", pid);

		close(fd[1]);
	} else {
		close(fd[1]);

		int pid;

		int res = read(fd[0], &pid, sizeof(int));
		if(res == -1) {
			printf("Erro a ler do pipe");
			exit(0);
		}

		printf("PID Filho: %d\n", pid);

		close(fd[0]);
	}

	return 0;
}
