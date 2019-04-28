#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
	int customer_code;
	int product_code;
	int quantity;
} product;

int main(void) {
	product sales[50000];
	int products[50000];
	int i, p, numChild = 50000 / 10, child, fd[2];

	time_t t;
	srand((unsigned) time(&t));

	for(i = 0; i < 50000; i++) {
		sales[i].product_code = i;
		sales[i].quantity = rand() % 25;
	}

  if(pipe(fd) == -1) {
		printf("Pipe not created properly");
		return 0;
	}

	for(i = 0; i < 10; i++) {
		if((child = fork()) == -1) {
			printf("Chils process error");
			return 0;
		}

		if(child == 0) {
			int start = i * numChild;
			int end = (i + 1) * numChild;
			close(fd[0]);

			for(p = start; p < end; p++) {
				if(sales[p].quantity > 20)
					write(fd[1], &(sales[p].product_code), sizeof(int));
			}

			close(fd[1]);
			exit(0);
		}
	}
	close(fd[1]);

	int product_code, bytes, cont = 0;
	while((bytes = read(fd[0], &product_code, sizeof(int))) > 0) {
		products[cont] = product_code;
		cont++;
	}

	if(bytes == -1) {
		printf("Read error");
		return 0;
	}

	printf("Product sales above 20\n");
	for(i = 0; i < cont; i++)
		printf("Product Code: %d\n", products[i]);
	printf("\nTotal: %d\n", cont);

	return 0;
}
