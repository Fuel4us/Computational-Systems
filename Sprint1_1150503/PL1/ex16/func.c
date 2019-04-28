#include <unistd.h>
#include <stdlib.h>

void my_exec(char *command) {
	if(fork() == 0) {
		int res = execlp(command, command, NULL);
		exit(res);
	}
}