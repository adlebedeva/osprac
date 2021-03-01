#include <stdio.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "stdlib.h"

int main()
{
    int* size_pointer;
    int shmid;
    char pathname[] = "07-1a.c";
    int file_size = 0;
    key_t key;

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((shmid = shmget(key, sizeof(int) + file_size * sizeof(char), 0666|IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }

    if ((size_pointer = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    file_size = *size_pointer;
    char *file_txt;
    file_txt = (char*)(size_pointer + 1);
    
    for (int i = 0; i < file_size; i++){
        putchar(file_txt[i]);
    }
        
    if (shmdt(size_pointer) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        printf("Can't delete shared memory\n");
        exit(-1);
    }
    return 0;
}
