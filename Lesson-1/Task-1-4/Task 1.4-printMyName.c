#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    printf("\nREALLY DANGEROUS PROGRAM >:)\n");
    
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    
    for (int i = 1; i< argc; i++){   
    	printf("\nUSER %s HAS ASKED FOR NAME %s INFO\n", pw->pw_name, argv[i]);
    }      
    
    for (int i = 0; i< 3; i++){   
	sleep(1);
	printf("\nWE TRY TO SEARCH VERY C0NF1DENTI4L INFO...\n");
    }
    
    printf("\n------------------------------------------\n");
    	
    for (int i = 1; i< argc; i++){ 
      
    	pw = getpwnam(argv[i]);
    	
    	if(pw == NULL){
    		printf("\nWE CANNOT FIND SUCH USER HERE [%s]\n", argv[i]);
    	}
    	else 
    		printf("\nOH IT LOOKS LIKE WE FOUND SHELL HERE [%s]: %s\n",argv[i], pw->pw_shell);
    }
    
}
