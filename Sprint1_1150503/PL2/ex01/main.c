#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	int fd[2];
	pid_t p;

	if(pipe(fd) == -1) {
		printf("PIPE error");
		exit(0);
	}

	p = fork();
	if(p == -1) {
		printf("child process not created");
		exit(0);
	}

	if(p != 0) {
		close(fd[0]);

		int pid = getpid();

		int res = write(fd[1], &pid, sizeof(int));
		if(res == -1) {
			printf("pipe communication failed");
			exit(0);
		}

		printf("Fathers PID: %d\n", pid);

		close(fd[1]);
	} else {
		close(fd[1]);

		int pid;

		int res = read(fd[0], &pid, sizeof(int));
		if(res == -1) {
			printf("failed to read response");
			exit(0);
		}

		printf("Sons PID: %d\n", pid);

		close(fd[0]);
	}

	return 0;
}
