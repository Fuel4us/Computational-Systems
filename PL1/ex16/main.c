#include <stdio.h>
#include <sys/wait.h>
#include "func.h"

int main(void) {
	int x, res;
	
	my_exec("ls");
	wait(&x);
	res = WEXITSTATUS(x);
	
	if(WEXITSTATUS(x) == -1)
		printf("Command: ls | Error exec\n");
	else
		printf("Command: ls | Result: %d\n", res);
	
	my_exec("who");
	wait(&x);
	res = WEXITSTATUS(x);
	
	if(WEXITSTATUS(x) == -1)
		printf("Command: who | Error exec\n");
	else
		printf("Command: who | Result: %d\n", res);
	
	my_exec("ps");
	wait(&x);
	res = WEXITSTATUS(x);
	
	if(WEXITSTATUS(x) == -1)
		printf("Command: ps | Error exec\n");
	else
		printf("Command: ps | Result: %d\n", res);
	
	return 0;
}