#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char* argv[]) {
    char text[100];
    FILE *fp;
    
	if((fp=fopen(argv[1],"rb"))==NULL) {
		printf("Errors occurred.");
		exit(EXIT_FAILURE);
	}

	fread(text, sizeof(text), 1, fp);		
	fclose(fp);
	
	printf("You wrote next info:\n");
	printf("%s", text);
}
