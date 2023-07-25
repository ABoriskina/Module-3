#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void listener(int);

int counter = 0;

int main () {
   signal(SIGINT, listener);

   while(1) {
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
   exit(EXIT_SUCCESS);
}

void listener(int sig) {
   counter++;
   if (counter == 3)
   	exit(EXIT_SUCCESS);
   else 
   	printf("I am waiting for the third SIGINT...\n");
}
