#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

int main(int argc, char* argv[]) {
	uid_t uid = getuid();
    	struct passwd *pw = getpwuid(uid);
	printf("%s > ", pw->pw_name);
	
	char input[100];
	
	int tempCounter = 0;
	char temp[100];
	memset(temp, 0, 100);
	
	char** args= (char**)malloc(sizeof(char*) * 5);
	int argCounter = 0;
	
	scanf("%99[^\n]", &input);
	strcat(input, " ");
	
    for (int i = 0; i < 100; i++){
        if (input[i] != ' '){
            temp[tempCounter++] = input[i];
        }
        else{
            if (argCounter == 0)
            	args[argCounter] = (char*)malloc(sizeof(char) * strlen(temp) + 10);
            	
            else 
            	args[argCounter] = (char*)malloc(sizeof(char) * strlen(temp));
            	
            strcpy(args[argCounter++], temp);
            memset(temp, 0 , 100);
            tempCounter = 0;
        }
    }
    
    char* catalog = (char*)malloc(sizeof(char) * (strlen(args[0]) + 10));
    strcpy(catalog, "/usr/bin/");
    strcat(catalog, args[0]);
    strcpy(args[0], catalog);
    printf("%s", args[0]);
    args[argCounter] = NULL;

    execv(args[0], args);
}

