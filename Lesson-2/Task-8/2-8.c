#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>

int main() {

    int p1[2];
    int p2[2];
    char message1[100];
    char message2[100];
    pid_t pid;

    int randomNumber;
    srand(time(NULL));

    for (int i = 0; i < 3; i++) {
        int rv;

        randomNumber = rand() % 10000;
        sprintf(message1, "%d", randomNumber);

        //randomNumber = rand() % 10000;
        //sprintf(message2, "%d", randomNumber);

        if (pipe(p1) || pipe(p2)) {
            perror("pipe() error");
            exit(EXIT_FAILURE);
        }

        switch (pid = fork()) {
            case -1:
                perror("fork() error");
                exit(EXIT_FAILURE);
            case 0:
                close(p1[0]);
                close(p2[1]);

                write(p1[1], message1, strlen(message1) + 1);
                read(p2[0], message2, sizeof(message2));

                printf("\nTHIS WAS SENT TO CHILD: %s\n", message2);
                exit(EXIT_SUCCESS);
            default:
                close(p1[1]);
                close(p2[0]);

                read(p1[0], message1, sizeof(message1));
                int num;
                sscanf(message1, "%d", &num);
                num *= 2;
                sprintf(message2, "%d", num);
                write(p2[1], message2, strlen(message2) + 1);
                printf("\nTHIS WAS SENT TO PARENT: %s\n", message1);

                wait(&rv);
        }
        memset(message1, 0, 100);
    }

    exit(EXIT_SUCCESS);
}
