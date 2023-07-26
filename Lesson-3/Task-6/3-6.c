#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

static int isModifying = 0;

void prohibit(int signo, siginfo_t *info, void *context){
	printf("You cannot modify now.\n");
	isModifying = 1;  
}
void allow(int signo, siginfo_t *info, void *context){
	printf("You can modify now.\n");
	isModifying = 0;
}

int main() {
	
    int p[2];
    pid_t pid;
    
    long int number = 10;
    
	char text[100];
	char text2[100];
	
	struct sigaction act = { 0 }; 

    act.sa_flags = SA_SIGINFO ;
    act.sa_sigaction = &prohibit; 
    
    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        perror("sigaction()");
        exit(EXIT_FAILURE);
    }
    
    struct sigaction act2 = { 0 }; 

    act2.sa_flags = SA_SIGINFO;
    act2.sa_sigaction = &allow; 
    
    if (sigaction(SIGUSR2, &act2, NULL) == -1) {
        perror("sigaction()");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int rv;
		FILE *fp;

        if (pipe(p)) {
            perror("pipe() error");
            exit(EXIT_FAILURE);
        }

        switch (pid = fork()) {
            case -1:
                perror("fork() error");
                exit(EXIT_FAILURE);
            case 0:  //------дочерний процесс-------------
                close(p[0]);
				while (isModifying){
					
				}
				if((fp=fopen("file.txt","rb"))==NULL) {
					printf("Errors occurred.");
					exit(EXIT_FAILURE);
				}

				fread(text, sizeof(text), 1, fp);	//считал с файла	
				fclose(fp);
				
				sscanf(text, "%ld", &number);
				for (int i = 0; i < 3; i++){
					number *= number % 13;
				}
				number++;
				
				sprintf(text, "%ld", number);
				
                write(p[1], text, strlen(text) + 1);  //записал в фд
                
                exit(EXIT_SUCCESS);
                
            default:
				close(p[1]);
				read(p[0], text2, sizeof(text2));
				kill(0, SIGUSR1);
				if((fp=fopen("file.txt", "wb"))==NULL) {
					printf("Errors occurred.");
					exit(EXIT_FAILURE);
				}
				 
				
				fwrite(&text2, strlen(text2), 1, fp); //записал новое число (отправленное ребенком) в файл.
				fclose (fp);
				kill(0, SIGUSR2);                
                printf("THIS WAS SENT TO PARENT: %s\n", text2);
                wait(&rv);
        }
        
        memset(text, 0, 100);
        memset(text2, 0, 100);
        sleep(2);
    }

    exit(EXIT_SUCCESS);
}
