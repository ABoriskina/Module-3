#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void stateTheEndOfProgram(){
    printf("\nProgram is ended.\n");
}


int main(int argc, char* argv[]) {
	pid_t pid;
	int rv;

	switch (pid = fork()){
	    case -1:
		printf("Error occurred.");
		exit(EXIT_FAILURE);
	    case 0:
		printf("\nChild proc with pid %d has following arguments: %d %s\n", getpid(), argc, argv[1]);
		exit(EXIT_SUCCESS);
	    default:
		printf("Parent proc with pid %d has following arguments: %d %s", getpid(), argc, argv[1]);
		wait(&rv);
		atexit(stateTheEndOfProgram);
	}

	exit(EXIT_SUCCESS);
}
