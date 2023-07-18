#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

void stateTheEndOfProgram(){
    printf("\nProgram ended.\n");
}

void countSquares(int arr[], int size){

    for (int i = 0; i < size; i++){
        printf("\nPROC WITH PID= %d RETURNED %d\n", getpid(), arr[i] * arr[i]);
    }
}

int main(int argc, char* argv[]) {

    const int size1 = (argc - 1) / 2;
    const int size2 = (argc - 1) - size1;

    int numbersForParentCalculations[size1];
    int numbersForChildCalculations[size2];

    for (int i = 0; i < size1; i++)
        sscanf(argv[i + 1], "%d", &numbersForParentCalculations[i]);

    for (int i = size1; i < argc - 1; i++)
        sscanf(argv[i + 1], "%d", &numbersForChildCalculations[i - size1]);

    pid_t pid;
    int rv;

    switch (pid = fork()){
        case -1:
            printf("Error occurred.");
            exit(EXIT_FAILURE);
        case 0:
            printf("\nChild proc has pid %d\n", getpid());
            countSquares(numbersForChildCalculations, size2);
            exit(EXIT_SUCCESS);
        default:
            printf("Parent proc has pid %d\n", getpid());
            countSquares(numbersForParentCalculations, size1);
            wait(&rv);
            atexit(stateTheEndOfProgram);
    }

    exit(EXIT_SUCCESS);
}
