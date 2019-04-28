#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BUFFER 81

int main(){
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

	if(p != 0){
		close(fd[0]);

		char string1[BUFFER];
		int size1;

		FILE *arq = fopen("teste.txt", "r");

		if(arq == NULL)
			printf("File couldnt be found\n");

		else{
			while(!feof(arq)){
				fgets(string1, BUFFER, arq);

				size1 = strlen(string1) + 1;

				int res = write(fd[1], string1, size1);

				if(res == -1) {
					printf("Couldnt write on pipe");
					exit(0);
				}
			}
		}

		fclose(arq);

		close(fd[1]);

		wait(NULL);
	}else{
		close(fd[1]);

		char string2[BUFFER];
		int res;

		while((res = read(fd[0], string2, sizeof(string2))) != 0){
			if(res == -1) {
				printf("Read error");
				exit(0);
			}
			printf("%s", string2);
		}

		printf("\n");

		close(fd[0]);
	}

	return 0;
}
