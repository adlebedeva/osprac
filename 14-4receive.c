#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int value;
int byte;
pid_t sender_pid;

void zero(int sig) {
  byte <<= 1;
  kill(sender_pid, SIGUSR1);
}

void one(int sig) {
  value |= byte;
  byte <<= 1;
  kill(sender_pid, SIGUSR1);
}

int main() {
  pid_t my_pid;
  value = 0;
  byte = 1;
  (void) signal(SIGUSR1, zero);
  (void) signal(SIGUSR2, one);

  my_pid = getpid();
  printf("Receiver pid = %d\n", my_pid);

  printf("Sender pid = ");
  scanf("%d", &sender_pid);
  kill(sender_pid, SIGUSR1);

  while (1) {
    if(byte == 0) {
      printf("Received = %d\n", value);
      break;
    }
  }
}
