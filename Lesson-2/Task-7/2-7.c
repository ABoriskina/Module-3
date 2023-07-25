#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>

int main() {

    int p[2];
    char message[100];
    pid_t pid;
    int randomNumber;
    srand(time(NULL));

    for (int i = 0; i < 5; i++) {
        int rv;

        randomNumber = rand() % 10000;
        sprintf(message, "%d", randomNumber);

        if (pipe(p)) {
            perror("pipe() error");
            exit(EXIT_FAILURE);
        }

        switch (pid = fork()) {
            case -1:
                perror("fork() error");
                exit(EXIT_FAILURE);
            case 0:
                close(p[0]);

                write(p[1], message, strlen(message) + 1);
                exit(EXIT_SUCCESS);
            default:
                close(p[1]);
                read(p[0], message, sizeof(message));
                printf("\nTHIS WAS SENT TO PARENT: %s\n", message);
                wait(&rv);
        }
        memset(message, 0, 100);
    }

    exit(EXIT_SUCCESS);
}
