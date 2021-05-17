#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define MESSAGE_LENGTH 255

// CLient
struct client
{
    long mtype;
    struct
    {
        pid_t pid;
        float message;
    } info;
} clientbuf;

struct server
{
    long mtype;
    struct
    {
    	float message;
    } info;
} serverbuf;

int main(void)
{
  int msqid;
  char pathname[]="11-3b.c";
  key_t  key;
  int len, maxlen;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  srand(time(NULL));
  
  //Client initialization
  clientbuf.info.pid = getpid();
  clientbuf.info.message = rand();
  clientbuf.mtype = 1;

  printf("Client send message; type of the message = %ld, message = '%f'\n", clientbuf.mtype, clientbuf.info.message);

  if (msgsnd(msqid, &clientbuf, sizeof(clientbuf.info), 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  printf("Wait message from the server.\n");

  if (msgrcv(msqid, &serverbuf, sizeof(serverbuf.info), getpid(), 0) < 0) {
    printf("Can\'t receive message from queue.\n");
    exit(-1);
  }
  printf("Message from the server: %f\n", serverbuf.info.message);

  return 0;
}