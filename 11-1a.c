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
    int     i, len;

    struct mymsgbuf {
        long mtype;
        struct  {
            short sinfo;
            float finfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Cannot get msqid\n");
        exit(-1);
    }
    for (i = 1; i <= 5; i++) {
        mybuf.mtype = i;
        mybuf.info.sinfo = i;
        mybuf.info.finfo = i / 100.0;
        len = sizeof(mybuf.info);
        if (msgsnd(msqid, &mybuf, len, 0) < 0) {
            printf("Cannot send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-1);
        }
    }

    mybuf.mtype = LAST_MESSAGE;
    len = 0;

    if (msgsnd(msqid, &mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }
    return 0;
}