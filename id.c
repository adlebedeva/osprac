#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
int main() 
{ 
printf("Real User ID = %d\n", getuid()); 
printf("Real Group ID = %d\n\n", getgid()); 
return 0;
}
