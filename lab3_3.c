#include <stdio.h>
int main(int argc, char *argv[], char *envp[])
{
printf("Arguments of argv: ");
	for(int i=0;i<argc;i++)
	printf("%s, ",argv[i]);

char* temp="";
int i=0;
printf("\n");
printf("Arguments of envp: ");
while(temp!=NULL)
{
printf("%s, ", envp[i]);
temp=envp[i];
i++;
}
}
