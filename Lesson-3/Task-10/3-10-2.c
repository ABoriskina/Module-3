#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_queue"
#define PRIORITY 0
#define SIZE 128

int main(){
	mqd_t ds;
	
	char new_text[SIZE];
	int isRunning = 1;

	struct mq_attr attr, old_attr;
	
	int prio;
	
	if ((ds = mq_open(QUEUE_NAME, O_RDONLY | O_NONBLOCK, 0666, NULL)) == (mqd_t)-1){
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}
	
	attr.mq_flags = O_RDONLY;
		
	if (mq_setattr(ds, &attr, NULL)){
		perror("mq_setattr()");
		exit(EXIT_FAILURE);
	}
	
	if (mq_getattr(ds, &old_attr)) {
		perror("mq_getattr()");
		exit(EXIT_FAILURE);
	}
	
	while(isRunning){	
			
		if (mq_receive(ds, new_text, SIZE, &prio) == -1){
			perror("mq_receive()");
			exit(EXIT_FAILURE);
		}
		
		printf("\nRECEIVED FROM 3-10: %s", new_text);
		
		if(prio == 255){
			isRunning = 0;
		
			if (mq_close(ds) == -1)
				perror("mq_close()");
				
			if (mq_unlink(QUEUE_NAME) == -1)
				perror("mq_unlink()");
		}
	}
	
	exit(EXIT_SUCCESS);
	
}
