#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define QUEUE_NAME "/my_queue"
#define QUEUE_NAME1 "/my_queue1"
#define PRIORITY 1
#define SIZE 512

int main(){

	mqd_t ds, ds1;
	
	int isRunning = 1;
	
		
	struct mq_attr queue_attr, queue_attr1, temp;
	struct timespec timeout;
	timeout.tv_sec = 3;
	
	queue_attr1.mq_maxmsg = 10;
	queue_attr1.mq_msgsize = SIZE;
	
	if ((ds = mq_open(QUEUE_NAME, O_RDONLY | O_NONBLOCK, 0600, NULL)) == (mqd_t)-1){ //для получения
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}
	
	if ((ds1 = mq_open(QUEUE_NAME1, O_WRONLY , 0600, &queue_attr1)) == (mqd_t)-1){ //для отправки
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}
	
	queue_attr.mq_flags = 0;
	
	if (mq_setattr(ds, &queue_attr, NULL)){
		perror("mq_setattr");
		return -1;
	}
		     
	 while(isRunning){	 	
     
     	pid_t pid;
     	int rv;
     	int prio;
     	
     	char text[SIZE]; 
		char new_text[SIZE];
     	
     	switch (pid = fork()){
     		case -1:
     			perror("fork()");
     			exit(EXIT_FAILURE);
     		case 0:

			   fflush(stdin);
			   memset(text, 0, SIZE);
				
			   int retval;
           	   fd_set rfds;
          	   struct timeval tv;
          		
		       FD_ZERO(&rfds);
		       FD_SET(0, &rfds);

		       tv.tv_sec = 5;
		       tv.tv_usec = 0;

		       retval = select(1, &rfds, NULL, NULL, &tv);

		       if (retval == -1)
		           perror("select");
		           
		       else if (retval)
		           printf("Data is available now.\n");

		       else
		           printf("No data within five seconds.\n");
		           
				read(rfds, text, SIZE);
             
     			if (mq_send(ds1, text, strlen(text), PRIORITY) == -1){
					perror("mq_send()");
					exit(EXIT_FAILURE);
				}
				
				exit(EXIT_SUCCESS);
				
			default:
			
				memset(new_text, 0, SIZE);
				
				if (mq_receive(ds, new_text, SIZE, &prio) == -1){
					perror("mq_receive()");
					exit(EXIT_FAILURE);
				}
				
				printf("\nYou've got a new message: %s\n", new_text);
														
		}
               
	}
	
	exit(EXIT_SUCCESS);
		
}
