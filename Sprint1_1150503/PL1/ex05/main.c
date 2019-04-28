#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void f1(), f2();

int main(){
	int i, status;

	for(i = 0; i < 2; i++){
		pid_t p = fork();

		if(p == 0){
			if(i == 0)
				f1();
			else
				f2();
		}
	}

	for(i = 0; i < 2; i++){
		wait(&status);

		if(WIFEXITED(status))
			printf("%d\n", WEXITSTATUS(status));
	}

	return 0;
}

void f1(){
	sleep(1);

	exit(1);
}

void f2(){
	sleep(2);

	exit(2);
}
