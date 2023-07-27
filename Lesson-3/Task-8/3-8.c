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
 		pid_t pid;
         int running=1;
         int msgid;
         struct my_msg some_data;
         struct my_msg some_data1;
         char buffer[50]; 
         long int msg_to_rec=0;
         
         int q = msgget((key_t)14534,0666|IPC_CREAT); //отправка 1 -полчатель 2
         int q1 = msgget((key_t)14535,0666|IPC_CREAT);//отправка 2 -полчатель 1
         
         if (q == -1 || q1 == -1) 
         {
                 printf("Error in creating queue\n");
                 exit(0);
         }

         		
 		switch(fork()){
 			case -1:
 				perror("fork()");
 				
 			case 0:
 				 while(running) {
				     fgets(buffer,50,stdin);
				     some_data1.msg_type=1;
				     strcpy(some_data1.some_text,buffer);
				     
				     if(msgsnd(q,(void *)&some_data1, MAX_TEXT,0)==-1)
				     	perror("msgsnd()");
				     
				     if(strncmp(buffer,"bye",3)==0)
				   		running=0;
				  }
				   		
				   	 exit(EXIT_SUCCESS);
		    default:
		    	while(running) {
					msgrcv(q1,(void *)&some_data,BUFSIZ,msg_to_rec,0);                 
				    printf("User sent you:%s\n",some_data.some_text);
				     
				    if(strncmp(some_data.some_text,"bye",3)==0)
				     running=0;
		        }
		         
 		}
             
 }
