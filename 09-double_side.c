#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>

int inc(int semid, struct sembuf* buf) {
    //struct sembuf buf;
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(semid, buf, 1);
}

int dec(int semid, struct sembuf* buf) {
    //struct sembuf buf;
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(semid, buf, 1);
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

    if (pipe(parent) < 0) {
        printf("Can't create pipe\n\r");
        exit(-1);
    }
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Key does not generated\n");
        exit(-1);
    }

    // semafor creation
    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("Semaphore not found. Trying to create... \n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can't get semid\n");
            exit(-1);
        }
        printf("Create successful!\n");
    }
    result = fork();

    if (result < 0) {
        printf("Can't fork child\n\r");
        exit(-1);
    }
    else if (result > 0) {
        int N;
        printf("Pls input N: \n");
        scanf("%d", &N);
        if (N < 2) {
            printf("N should be more or equals 2.\n");
            exit(-1);
        }
        for (size_t i = 0; i < N; i++)
        {
            // write children
            if (write(parent[1], "Hello, world!", 14) != 14) {
                printf("Can\'t write all string\n\r");
                exit(-1);
            }
            printf("Parent wrote his message!\n\r", i + 1);
            inc(semid, &buffer);
            dec(semid, &buffer)
            size = read(parent[0], resstring, 14);
            if (size != 14) {
                printf("Can\'t read from child\n\r");
                exit(-1);
            }
            printf("Parent read: %s\n\r", resstring);
        }
        close(parent[0]);
    }
    else {
        int counter = 0;
        while (1) {
            dec(semid, &buffer);
            size = read(parent[0], resstring, 14);

            if (size < 0) {
                close(parent[1]);
                close(parent[0]);
                printf("Success!\n");
                return 0;
            }
            printf("Child read: %s\n\r", ++counter, resstring);

            if (write(parent[1], "Hi, my parent", 14) != 14) {
                printf("Can't write all string.\n");
                exit(-1);
            }
            inc(semid, &buffer);
        }
    }
    return 0;
}