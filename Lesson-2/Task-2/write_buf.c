#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char text[100];
    memset(text, 0, 100);
    printf("Enter the text you want to write to your file \n > ");
    scanf("%99[^\n]", text);
  
	  FILE *fp;
  
	if((fp=fopen(argv[1], "wb"))==NULL) {
		  perror("fopen()");
		  exit(EXIT_FAILURE);
	}
	
	fwrite(&text, strlen(text), 1, fp);
	fclose (fp);
	
	exit(EXIT_SUCCESS);
}
