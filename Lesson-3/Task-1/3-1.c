#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]) {

	fflush( stdout );
	pid_t pid = getpid();
	printf("PID OF PROG IS %d", pid);
	
    int number = 1;
    char numberArray[5];

	FILE *fp;

	
	while (1){
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
