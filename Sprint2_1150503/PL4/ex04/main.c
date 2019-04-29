#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include "struct.h"

#define MIN_WAIT 1
#define MAX_WAIT 5

int sem_values[1] = {1};

int main(){
	int data_size = sizeof(Structure);

	int fd = shm_open("/shm_ex04", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(fd, data_size) == -1){
		perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
	}

  srand(time(NULL));
	int time_wait = rand() % MAX_WAIT + (MIN_WAIT - 1);
	sem_t *sem;

	sem = sem_open("/shm_ex04", O_CREAT, 0644, sem_values[0]);
	if(sem == SEM_FAILED){
		perror("Creating Semaphore ERROR \n");
    exit(EXIT_FAILURE);
	}

	sem_wait(sem);

  Structure *shared_data = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}

  sprintf(shared_data->text_lines[shared_data->index], "I'm the Father - with PID %d", getpid());
  shared_data->index++;

  printf("Waiting %d seconds\n", time_wait);
  sleep(time_wait);

  sem_post(sem);

  printf("\nShared Memory:\n");
  int i;
  for(i = 0; i < shared_data->index; i++)
    printf("%s\n", shared_data->text_lines[i]);

    int num;

  	printf("\nDelete a line? (1=yes / 0=no)\n");
  	fflush(stdin);
  	scanf("%d", &num);

    if(num == 1){
      struct timespec ts;

      if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
      {
        perror("Timespec ERROR\n");
        exit(0);
      }

      ts.tv_sec += 12;

      if((i = sem_timedwait(sem, &ts)) == -1){
        perror("Operation timedout\n");
        exit(0);
      }else{
        memset(shared_data->text_lines[shared_data->index - 1], 0, 80);
        shared_data->index--;
        sem_post(sem);
      }
    }

    printf("\nShared Memory:\n");
    for(i = 0; i < shared_data->index; i++)
      printf("%s\n", shared_data->text_lines[i]);

    munmap(shared_data, data_size);
    close(fd);

return 0;
}
