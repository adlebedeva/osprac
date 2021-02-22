#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    int pfd;
    int fd[2];
    char a[100];
    int size=0;
    int c,b;

    if((pfd=pipe(fd))<0)
    {
        printf("Error!");
        exit(1);
    }
    else
    {
        do
        {
            c=write(fd[1],"h",1);
            size=(size+1);
            printf("%d\n",size);
        } 
        while(c != b);
    }

    return 0;
}

