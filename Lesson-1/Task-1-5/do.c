#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int main(int argc, char* argv[]) {
    char* catalog = "/usr/bin/";
    char* nameOfProg = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 10));
    strcpy(nameOfProg, catalog);
    strcat(nameOfProg, argv[1]);
    printf("%s", nameOfProg);
    execl(nameOfProg, NULL);
    free(nameOfProg);
    exit(EXIT_SUCCESS);
}
