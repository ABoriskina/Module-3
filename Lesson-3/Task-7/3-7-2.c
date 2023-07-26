#include<stdlib.h>
 #include<stdio.h>
 #include<string.h>
 #include<unistd.h>
 #include<sys/types.h>
 #include<sys/ipc.h>
 #include<sys/msg.h>
 
 struct my_msg{
	long int msg_type;
	char some_text[BUFSIZ];
};
 
int main(){
	int isRunning=1;
	int msgid;
	
	struct my_msg some_data;
	long int msg_to_rec=0;
	
	msgid=msgget((key_t)14534,0666|IPC_CREAT);
	
	while(isRunning){
		msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_rec, 0);  
		               
		printf("RECIEVED FROM 3-7: %s\n", some_data.some_text);
		
		if(some_data.msg_type == 255){
			isRunning = 0;
		}
	}
	
	msgctl(msgid,IPC_RMID,0);
  exit(EXIT_SUCCESS);
}
