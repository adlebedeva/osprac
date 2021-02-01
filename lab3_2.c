#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;
  pid = fork();

  if (pid == -1) {
   return 1;
}
  else if (pid == 0) {
    printf("Its a child, pid=%d\n, ppid= %d\n", getpid(), getppid());
} else {
   printf("Its a parent, pid=%d\n, ppid= %d\n", getpid(), getppid());
}
return 0;
}
