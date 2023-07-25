#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char text[100];
    printf("Enter the text you want to write to your file \n > ");
    scanf("%99[^\n]", text);

    int fd = open(argv[1], O_WRONLY);

    if (fd == -1) {
        printf("Errors occurred.");
        exit(EXIT_FAILURE);
    }
    else {
        write(fd, &text, strlen(text));
        close(fd);
    }
  
    exit(EXIT_SUCCESS);
}
