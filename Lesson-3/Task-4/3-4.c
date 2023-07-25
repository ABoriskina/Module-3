#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

static int waiter = 1;

void listener(int sig){
	switch (sig){
	 case SIGINT:
		printf("SIGINT was detected.\n");
		waiter = 0;
		break;
	case SIGQUIT:
		printf("SIGQUIT was detected.\n");
		waiter = 0;
		break;
	}
}

int main(int argc, char* argv[]) {

	signal(SIGINT, listener);
	signal(SIGQUIT, listener);
	
	do{	
		int number = 1;
		char numberArray[5];

		FILE *fp;

		if((fp=fopen("file.txt", "a"))==NULL) {
			printf("Errors occurred.");
			exit(EXIT_FAILURE);
		}
			
		memset(numberArray, 0, 5);
		sprintf(numberArray,"%d", number % 10000);
		number++;
		strcat(numberArray, " ");
		fwrite(&numberArray, strlen(numberArray), 1, fp);
		fclose (fp);
		sleep(1);
		
	}
	
	while(waiter);

	exit(EXIT_SUCCESS);
}
