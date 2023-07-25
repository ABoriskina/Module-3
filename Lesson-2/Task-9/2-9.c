#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {

    char wholeString[100];
    char* arguments[100];
    bool searchForArgument = false;
    char temp1[100];
    memset(temp1, 0, 100);
    int countTempSymbols = 0;
    strcpy(wholeString, argv[1]);

    //char temp[100] = "/bin/";
    char temp[100];
    memset(temp, 0, 100);
    int numOfCommands = 0;
    int countCommands = 0;
    //int countCommandSymbols = 5;
    int countCommandSymbols = 0;

    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '|') {
            numOfCommands++;
        }
        strcat(wholeString, " ");
        strcat(wholeString, argv[i]);
    }
    strcat(wholeString, " |");
    numOfCommands++;
  
    char* commands[numOfCommands];
    arguments[countCommands] = (char*)malloc(sizeof (char) * 5);
    memset(arguments[countCommands], 0, 5);

    for(int i = 0; i < strlen(wholeString); i++){

        if (wholeString[i] == '|' && !searchForArgument){
            commands[countCommands] = (char*)malloc(sizeof(char) * (countCommandSymbols + 9));
            arguments[countCommands + 1] = (char*)malloc(sizeof (char) * 5);
            strcpy(commands[countCommands], temp);
            countCommands++;
            memset(temp, 0, 100);

            countCommandSymbols = 0;
        }
        else if (wholeString[i] == '-' &&!searchForArgument){
            searchForArgument = true;
            temp1[countTempSymbols++] = wholeString[i];
        }

        else if (!(wholeString[i] == ' ' && wholeString[i + 1] != '-') && !searchForArgument && wholeString[i] != ' '){
            temp[countCommandSymbols] = wholeString[i];
            countCommandSymbols++;
        }

        else if (searchForArgument && wholeString[i] != ' '){
            temp1[countTempSymbols++] = wholeString[i];
        }
        else if (searchForArgument && wholeString[i] == ' '){
            searchForArgument = false;
            strcpy(arguments[countCommands], temp1);
            memset(temp1, 0, 100);
            countTempSymbols=0;
        }

    }

    printf("Will be executed: \n");
    for(int i = 0; i < countCommands; i++){
        printf("%s", commands[i]);
        printf("%s\n", arguments[i]);
    }

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

	printf("Result:\n");

    int p1[2];
    int p2[2];
    char message[1500];

    if (pipe(p1) || pipe(p2)) {
        perror("pipe() error");
        exit(EXIT_FAILURE);
    }

    if (countCommands == 3){

        pid_t pid;
        pid_t pid1;
        int rv;
        int rv1;

        switch (pid = fork())  {
            case -1:
                perror("fork() error");
                exit(EXIT_FAILURE);
            case 0:
                dup2(p1[1], STDOUT_FILENO);
                close(p1[0]);

                execlp(commands[0], arguments[0], NULL);

            default:
                wait(&rv);
                
                switch(pid1 = fork()){
                    case -1:
                        perror("fork() error");
                        exit(EXIT_FAILURE);
                    case 0:
                        dup2(p2[1], STDOUT_FILENO);
                        dup2(p1[0], STDIN_FILENO );
                        close(p1[1]);
                        close(p2[0]);
                        
                        execlp(commands[1], commands[1], arguments[1], NULL);
                        exit(EXIT_SUCCESS);
                    default:
                        wait(&rv1);
                        close(p2[1]);
                        close(p1[0]);
                        dup2(p2[0], STDIN_FILENO );
                        
                        execlp(commands[2], commands[2], arguments[2], NULL);

                }

        }

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
    }
  
    exit(EXIT_SUCCESS);
}
