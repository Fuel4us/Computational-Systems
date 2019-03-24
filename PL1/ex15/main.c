#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(void) {
	char filename[100];
	int status;
	pid_t pid;

	while(1){
		printf("\nInsert a file name\n");
		fgets(filename, 80, stdin);

		if(filename[0] == '\n')
			break;

		if(filename[strlen(filename) - 1] = '\n')
			filename[strlen(filename) - 1] = '\0';

		pid = fork();

		if(pid == 0){
			execlp("ls", "ls", filename, NULL);
			exit(-1);
		}
		else{
			wait(&status);
			if(WIFEXITED(status)){
				if(WEXITSTATUS(status) == 0)
					printf("The file exists");
				else
					printf("The file doesn´t exist");
			}
		}
	}
	return 0;
}
