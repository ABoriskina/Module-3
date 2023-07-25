#include <sys/stat.h>
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
		struct stat buffer;
		
		int fr = stat(newDir->d_name, &buffer);
		
		if(fr != 0){
			printf("Errors occurred. Code [%d]", fr);
			exit(EXIT_FAILURE);
		}
		
		mode_t mode = buffer.st_mode;

		if (S_ISLNK(mode))
			printf("%s [type link]\n", newDir->d_name);
		else if (S_ISREG(mode))
			printf("%s [type regular file]\n", newDir->d_name);
		else if (S_ISDIR(mode))
			printf("%s [type catalog]\n", newDir->d_name);
		else if (S_ISCHR(mode))
			printf("%s [type symbolic]\n", newDir->d_name);
		else if (S_ISBLK(mode))
			printf("%s [type block]\n", newDir->d_name);
		else if (S_ISFIFO(mode))
			printf("%s [type FIFO channel]\n", newDir->d_name);
		else if (S_ISSOCK(mode))
			printf("%s [type socket]\n", newDir->d_name);      
    }
	
	closedir(dir);
  exit(EXIT_SUCCESS);
}
