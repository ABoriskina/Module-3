#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

int main (int argc, char *argv[]){
	
	DIR *dir = opendir(argv[1]);
	struct dirent* newDir;
	
	while ((newDir = readdir(dir)) != NULL) {
        printf("%s\n", newDir->d_name);
    }
	
	closedir(dir);
  
  exit(EXIT_SUCCESS);
}
