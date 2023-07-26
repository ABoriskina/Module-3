#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>

#define QUEUE_NAME "/my_queue"
#define PRIORITY 0
#define SIZE 128

int main(){

	mqd_t ds;
	int isRunning = 1;
	
	char text[SIZE]; 
    FILE* fp;
    
    if((fp=fopen("text.txt","rb")) == NULL) {
			perror("fopen() error");
			exit(EXIT_FAILURE);
	 }
	 
	struct mq_attr queue_attr;
			
	queue_attr.mq_maxmsg = 10;
	queue_attr.mq_msgsize = SIZE;
	 
	if ((ds = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY , 0666, &queue_attr)) == (mqd_t)-1){
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}
	     
	 while(isRunning){	 	
     
     	while (fgets(text, 128, fp) != NULL){     									

			if (mq_send(ds, text, strlen(text), PRIORITY) == -1){
				perror("mq_send()");
				exit(EXIT_FAILURE);
			}
			
			memset(text, 0, SIZE);
			
			sleep(2);
		}
		
		fclose(fp);
		
		memset(text, 0, SIZE);
				
 		strcpy(text, "End of file.");
 		
		if (mq_send(ds, text, strlen(text), 255) == -1){
			perror("mq_send()");
			exit(EXIT_FAILURE);
		}
		
		if (mq_close(ds) == -1)
			perror("mq_close()");
        
        isRunning = 0;  
        
	}
	
	exit(EXIT_SUCCESS);
	
}
