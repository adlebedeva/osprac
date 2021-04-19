#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <stdlib.h>

int D(int semid, struct sembuf* buf) {

	//struct sembuf buf;
	buf->sem_op = -1;
	buf->sem_flg = 0;
	buf->sem_num = 0;
	return semop(semid, buf, 1);
}

int A(int semid, struct sembuf* buf) {
	//struct sembuf buf;
	buf->sem_op = 1;
	buf->sem_flg = 0;
	buf->sem_num = 0;
	return semop(semid, buf, 1);
}

int main() {
	struct sembuf mybuf;
	int semid;
	int fd[2], result;
	size_t size;
	char resstring[21];
	/* Попытаемся создать pipe */
	if (pipe(fd) < 0) {
		/* Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу */
		printf("Can\'t create pipe\n");
		exit(-1);
	}
	key_t key; /* IPC ключ */
	if ((key = ftok(pathname, 0)) < 0) {
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((semid = semget(key, 1, 0666)) < 0) {
		printf("Semaphore not found. Trying to create...\n");
		if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
			printf("Can\'t get semid\n");
			exit(-1);
		}
		printf("Create successful!\n");
	}
	result = fork();

	if (result < 0) {
		printf("Can\'t fork child\n");
		exit(-1);
	}
	else if (result > 0) {
		srand(time(NULL));
		for (int i = 0; i < rand() * 1000; ++i);
		size = write(fd[1], "Message from parent!", 21);
		if (size != 21) {
			printf("Can\'t write all string\n");
			exit(-1);
		}
		else {
			printf("Parent wrote his message.\n");
		}
		D(semid, &mybuf); // wait the child to write
		size = read(fd[0], resstring, 21);
		if (size < 0) {
			printf("Can\'t read string\n");
			exit(-1);
		}
		printf("Parent read: %s\n", resstring);
		close(fd[1]);
		close(fd[0]);
		printf("Parent exit\n");
	}
	else {
		size = read(fd[0], resstring, 21);
		if (size < 0) {
			printf("Can\'t read string\n");
			exit(-1);
		}

		printf("Child read: %s\n", resstring);
		size = write(fd[1], "Message from child!!", 21);
		A(semid, &mybuf); // let the parent read
		if (size != 21) {
			printf("Can\'t write all string\n");
			exit(-1);
		}
		else {
			printf("Child wrote his message.\n");
		}

		close(fd[1]);
		close(fd[0]);
		printf("Child exit\n");
	}
	return 0;
}