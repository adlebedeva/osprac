#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void) {
    int     msqid;
    char    pathname[] = "11-1a.c";
    key_t   key;
    int     length, max_length;

    struct mymsgbuf {
        long mtype;
        struct {
            short sinfo;
            float finfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Cannot generate key\n");
        exit(-1);
    }
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Cannot get msqid\n");
        exit(-1);
    }
    while (1) {
        max_length = sizeof(mybuf.info);
        if ((length = msgrcv(msqid, &mybuf, max_length, 0, 0)) < 0) {
            printf("Cannot receive message from queue\n");
            exit(-1);
        }
        if (mybuf.mtype == LAST_MESSAGE) {
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(0);
        }
        printf("Message type = %ld, sinfo = %i, finfo = %f\n",
            mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
    }
    return 0;
}