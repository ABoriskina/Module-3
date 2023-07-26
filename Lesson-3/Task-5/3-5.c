#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

static int waiter = 1;
FILE *fp;
static int isOpened = 0;

void listener(int sig){
	switch (sig){
	 case 2:
		printf("SIGINT was detected.\n");
		if (isOpened)
			waiter = 0;
		else 
			printf("Cannot quit. File is opened.\n");
		break;
	case 3:
		printf("SIGQUIT was detected.\n");
		
		if (isOpened)
			waiter = 0;
		else 
			printf("Cannot quit. File is opened.\n");
		break;
	}
}

int main(int argc, char* argv[]) {

	signal(SIGINT, listener);
	signal(SIGQUIT, listener);
	
	do{	
		int number = 1;
		char numberArray[5];


		if((fp=fopen("file.txt", "a"))==NULL) {
			printf("Errors occurred.");
			exit(EXIT_FAILURE);
		}
		isOpened = 1;	
		memset(numberArray, 0, 5);
		sprintf(numberArray,"%d", number % 10000);
		number++;
		strcat(numberArray, " ");
		fwrite(&numberArray, strlen(numberArray), 1, fp);
		sleep(3);
		fclose (fp);
		isOpened = 0;
		sleep(3);
		
	}
	
	while(waiter);

	exit(EXIT_SUCCESS);
}
