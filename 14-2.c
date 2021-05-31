#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
	if (nsig == SIGINT) {
		printf("CTRL+C\n");
	} else if (nsig == SIGQUIT) {
		printf("CTRL+4\n");
	}
}

int main(void) {

	(void)signal(SIGINT, my_handler);
	(void)signal(SIGQUIT, my_handler);
	
	while(1){}
	return 0;
}

