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
         char some_text[BUFSIZ];
 };
 int main() {
     int isRunning = 1;
     int msgid;
     char buffer[50];
     struct my_msg some_data;
     struct my_msg some_data1;
     long int msg_to_rec=0;
     
     int q = msgget((key_t)14534,0666|IPC_CREAT); //отправка 1 -полчатель 2
     int q1 = msgget((key_t)14535,0666|IPC_CREAT);//отправка 2 -полчатель 1
     
     while(isRunning)
     {             
     //------------------------------полчение--------------
         msgrcv(q,(void *)&some_data,BUFSIZ,msg_to_rec,0);                 
         printf("\n%s\n",some_data.some_text);
         
         if(strncmp(some_data.some_text,"bye",3)==0) {
                 isRunning = 0;
         }
            
         printf("\n > ");
         fgets(buffer,50,stdin);
       
         some_data1.msg_type=1;
         strcpy(some_data1.some_text,buffer);
       
         if(msgsnd(q1,(void *)&some_data1, MAX_TEXT,0)==-1) {
                 perror("msgsnd()");
         }
             
     }
      msgctl(q,IPC_RMID,0);
      msgctl(q1,IPC_RMID,0);
 }
