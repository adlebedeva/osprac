#include <stdio.h>
#include <errno.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "stdlib.h"

int main()
{
    int shmid;
    int new = 1;
    char pathname[] = "07-1a.c";
    key_t key;
    FILE *file;

    file = fopen(pathname, "read");    
    fseek(file, 0L, SEEK_END);
    int file_size = 0;
    file_size = ftell(file);
    rewind(file);
    

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    

    if ((shmid = shmget(key, sizeof(int) + file_size * sizeof(char), 0666|IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }

    int *size_pointer;//вместо array в примере

    if ((size_pointer = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    *size_pointer = file_size;
    char *file_text;
    file_text = (char*)(size_pointer + 1);

    for (int i = 0; i < file_size; i++){
        file_text[i] = fgetc(file);
    }
        
    fclose(file);
    if (shmdt(size_pointer) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    return 0;
}
