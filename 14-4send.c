#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int needNext = 1;

void handle(int sig) {
	needNext = 0;
}

int main() {
  pid_t my_pid, to_pid;
  int value;
  (void)signal(SIGUSR1, handle);

  my_pid = getpid();
  printf("Sender pid = %d\n", my_pid);

  printf("Enter pid = ");
  scanf("%d", &to_pid);

  printf("Enter value = ");
  scanf("%d", &value);

  int byte = 1;
  while (needNext);
  while (byte != 0) {
    if ((value & byte) == 0) {
      kill(to_pid, SIGUSR1);
    } else {
      kill(to_pid, SIGUSR2);
    }
    byte = byte << 1;
	needNext = 1;
	while(needNext);
  }
}
