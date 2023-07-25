#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

int main (int argc, char *argv[]){
	
	char text[100];
  FILE *fp;
	
	DIR *dir = opendir(".");
	struct dirent* newDir;
	
	int isFound = 0;
	
	while ((newDir = readdir(dir)) != NULL) {
		if (strcmp(argv[1], newDir->d_name) == 0)
			isFound = 1;	
  }
	
	closedir(dir);
	
	if (isFound == 0){
  	printf("\nCannot locate file. Create it? [Y/n] ");
  	char choice = getchar();
  			
  	if (choice != 'Y') 
  		exit(EXIT_FAILURE);
  			
  	fp = fopen(argv[1], "wb");
  	fprintf (fp,"A new file.");
  	fclose(fp);
  		
  	if((fp=fopen(argv[1],"rb"))==NULL) {
  			printf("Errors occurred.");
  			exit(EXIT_FAILURE);
  	}
  
  	fread(text, sizeof(text), 1, fp);		
  	fclose(fp);
  		
  	printf("File has following info:\n");
  	printf("%s", text);
	}
	else {
	
		if((fp=fopen(argv[1],"rb"))==NULL) {
				printf("Errors occurred.");
				exit(EXIT_FAILURE);
			}

			fread(text, sizeof(text), 1, fp);		
			fclose(fp);
			
			printf("File has following info:\n");
			printf("%s", text);
	}
  
	exit(EXIT_SUCCESS);

}
