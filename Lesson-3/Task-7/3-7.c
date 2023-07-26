#include<stdlib.h>
 #include<stdio.h>
 #include<string.h>
 #include<unistd.h>
 #include<sys/types.h>
 #include<sys/ipc.h>
 #include<sys/msg.h>
 #define MAX_TEXT 512
 
struct my_msg{
	long int msg_type;
	char some_text[MAX_TEXT];
};
 
 int main() {
     int isRunning = 1;
     int msgid;
     
     struct my_msg some_data;
     
     char text[256]; 
     FILE* fp;
     
     msgid=msgget((key_t)14534,0666|IPC_CREAT);
     
     if (msgid == -1) {
            perror("msgget() error");
            exit(EXIT_FAILURE);
     }
     
	if((fp=fopen("text.txt","rb")) == NULL) {
			perror("fopen() error");
			exit(EXIT_FAILURE);
	 }
	 
     while(isRunning){
     
     	while (fgets(text, 256, fp) != NULL){
     	
     		fread(text, sizeof(text), 1, fp);
			
            some_data.msg_type = 1;           
            strcpy(some_data.some_text,text);
            
            if(msgsnd(msgid,(void *)&some_data, MAX_TEXT,0)==-1) {
            	perror("msgsnd() error");
       		}
            
            sleep(2);
     		                       
     	}

     	
		fclose(fp);
				
 		strcpy(text, "end of file.");
 		
 		some_data.msg_type = 255;
        strcpy(some_data.some_text,text);
        
        if(msgsnd(msgid,(void *)&some_data, MAX_TEXT,0)==-1) {
        	perror("msgsnd() error");
        } 
        
        isRunning = 0;     

     } 
   exit(EXIT_SUCCESS);
 }
 
