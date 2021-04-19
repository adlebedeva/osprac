#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/sem.h>


void try_semop(int sem_id, int sem_op) {
	struct sembuf mybuf;
	mybuf.sem_num = 0;
	mybuf.sem_flg = 0;
	mybuf.sem_op = sem_op;
	if (semop(sem_id, &mybuf, 1) < 0) {
		printf("Can\'t wait for condition\n");
		exit(-1);
	}
}
//increment
void A(int sem_id, int value) {
	try_semop(sem_id, value);
}
//decrement
void D(int sem_id, int value) {
	try_semop(sem_id, -value);
}
//zero check
void Z(int sem_id) {
	try_semop(sem_id, 0);
}

int main()
{
	int parent[2], result;
	size_t size = 0;
	key_t key;
	char pathname[] = "09-double_side.c";
	struct sembuf buffer;
	int semid;
	char resstring[14];
	//pipe creation
	if (pipe(parent) < 0) {
		printf("Невозможно создать pipe отца\n\r");
		exit(-1);
	}
	if ((key = ftok(pathname, 0)) < 0) {
		printf("Ключ не сгенерирован.\n");
		exit(-1);
	}
	//semaphor creation
	if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
		if (errno != EEXIST) {
			printf("Can\'t create semaphore set\n");
			exit(-1);
		}
		else if ((semid = semget(key, 1, 0)) < 0) {
			printf("Can\'t find semaphore\n");
			exit(-1);
		}
	}
	else {
		A(semid, 2);
	}

	int N;
	printf("Введите N: \n");
	scanf("%d", &N);
	if (N < 2) {
		printf("N должен быть больше или равен 2.\n");
		exit(-1);
	}

	/*
	Parent call func D to block child's process. Child in his iteration calls Z func and check if semaphore value
	equals zero and then increments two to semaphore. To set zero value to semaphore again parent should decrement 
	semaphore twice in the beginning ad in the end of a cycle. 	
	*/

	result = fork();
	if (result < 0) {
		printf("Неудачный fork\n\r");
		exit(-1);
	}

	// Parent
	else if (result > 0) {
		for (size_t i = 0; i < N; i++)
		{
			D(semid, 1);
			if (i != 0) {
				size = read(parent[0], resstring, 14);
				if (size < 0) {
					printf("Can\'t read string from pipe\n");
					exit(-1);
				}
				printf("%d. Parent read message:%s\n", i, resstring);
			}
			size = write(parent[1], "Hello, world!", 14);

			if (size != 14) {
				printf("Can\'t write all string to pipe\n");
				exit(-1);
			}
			D(semid, 1);
		}
		close(parent[0]);
	}
	else {
		// Child
		int counter = 0;
		for (int i = 0; i < N; ++i) {
			Z(semid);
			size = read(parent[0], resstring, 14);
			//read parents message
			if (size < 0) {
				printf("Can\'t read string from pipe\n");
				exit(-1);
			}
			//wtite parent
			printf("%d. Child read message:%s\n", ++counter, resstring);
			size = write(parent[1], "Hello, parent", 14);
			if (size != 14) {
				printf("Can\'t write all string to pipe: %d\n", size);
				exit(-1);
			}

			A(semid, 2);
		}

		close(parent[1]);
		close(parent[0]);
	}
	return 0;
}