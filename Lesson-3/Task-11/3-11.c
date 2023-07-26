#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define QUEUE_NAME "/my_queue"
#define QUEUE_NAME1 "/my_queue1"
#define PRIORITY 1
#define SIZE 512

int main(){

	mqd_t ds, ds1;
	
	int isRunning = 1;
    char text[SIZE]; 
	char new_text[SIZE];
	memset(text, 0, SIZE);
	memset(new_text, 0, SIZE);
	
	struct mq_attr queue_attr, queue_attr1, temp;
	
	queue_attr.mq_maxmsg = 10;
	queue_attr.mq_msgsize = SIZE;
			

	if ((ds = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0600, &queue_attr)) == (mqd_t)-1){ //для отправки
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}

	if ((ds1 = mq_open(QUEUE_NAME1, O_CREAT | O_RDWR, 0600, NULL)) == (mqd_t)-1){ //для получения
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}
	
	queue_attr1.mq_flags = 0;
	
	if (mq_setattr(ds1, &queue_attr1, NULL)){
		perror("mq_setattr");
		return -1;
	}
	
	
	//mq_close(ds);
	//mq_close(ds1);
	
	
	 while(isRunning){	 	

     	pid_t pid;
     	int rv;
     	int prio;
     	
     	switch (fork()){
     	
     		case -1:
     			perror("fork()");
     			exit(EXIT_FAILURE);
     		case 0:
     			fflush(stdin);  
     			memset(text, 0, SIZE);   			
				fgets(text, 512, stdin);
				
				if (mq_send(ds, text, strlen(text), PRIORITY) == -1){
					perror("mq_send()");
					exit(EXIT_FAILURE);
				}
				  			
				exit(EXIT_SUCCESS);
				
			default:
				memset(new_text, 0, SIZE);
				
				if (mq_receive(ds1, new_text, SIZE, &prio) == -1){
					perror("mq_receive()");
					exit(EXIT_FAILURE);
				}
				printf("\nYou've got a new message: %s\n", new_text);
											
		}
               
	}
	
	exit(EXIT_SUCCESS);
		
}
