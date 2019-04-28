#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	int num_numbers = 2000, num_childs = 10;
	int numbers[num_numbers];
	int i;

	srand(time(NULL));
	int number = rand() % 10;

	printf("Number being searched: %d\n\n", number);

	for(i = 0; i < num_numbers; i++)
		numbers[i] = rand() % 10;      

	for(i = 0; i < num_childs; i++) {
		pid_t p = fork();
		if(p == 0) {
			int k;
			int aux = (i + 1) * (num_numbers / num_childs);

			for(k = i * (num_numbers / num_childs); k < aux; k++) {
				if(numbers[k] == number)
					exit(k % (num_numbers / num_childs));
			}
			exit(255);
		} else {
			int x;
			wait(&x);

			int index = WEXITSTATUS(x);
			if(index != 255)
				printf("Number found on relative index %d\n", index);
			else
				printf("Number not found\n");
		}
	}

	for(i = 0; i < num_numbers; i++) {
		if(i % (num_numbers / num_childs) == 0)
			printf("\n");
		printf("%d ", numbers[i]);
	}
	printf("\n");

	return 0;
}
