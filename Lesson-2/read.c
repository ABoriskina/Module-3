#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char* argv[]) {

    char text[100];

	  int fd = open(argv[1], O_RDONLY);
    int size;
    char* document = (char*)malloc(sizeof(char) * 100);

    if (fd == -1)
        printf("Errors occurred.");
    else {
  		size = read(fd, document, 100);
  		document[size] = '\0';
  		close(fd);
    }
    printf("You wrote next info:\n");
    printf("%s", document);
    
    exit(EXIT_SUCCESS);
}
