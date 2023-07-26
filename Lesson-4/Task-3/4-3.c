#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS_COUNT 5
#define SIMULTANEOUSLY_ACTIVE 3
#define WRITING_IS_DONE 111
#define READING_IS_DONE 112
#define READING_IS_OFF 102

pthread_t tid[THREADS_COUNT];
void* status[THREADS_COUNT];


union semun {
	int val; 
	struct semid_ds *buf; 
	unsigned short *array; 
	struct seminfo *__buf; 
};

int semid;	
union semun arg;
struct sembuf lock_res = {0, -1, 0};
struct sembuf rel_res = {0, 1, 0};

FILE *fp;

int p[2];
long int number = 10;
char text[100];
char text2[100];  
int isReadingFile = 0;      

void* sendToFD(void* arg) {  //child  4 pieces
	
	printf("waiting for the semaphore\n");	
	semop(semid, &lock_res, 1);		
	printf("file read semaphore locked\n");	
	
	isReadingFile = 1;
	
	fread(text, sizeof(text), 1, fp);	//считал с файла	
	//fclose(fp);
	isReadingFile = 0;
	
	semop(semid, &rel_res, 1);	
	printf("file read semaphore unlocked\n");
	
	
	
	sscanf(text, "%ld", &number);
	for (int i = 0; i < 3; i++){
		number *= number % 13;
	}
	number++;
	
	sprintf(text, "%ld", number);
	
    write(p[1], text, strlen(text) + 1);  //записал в фд       

	return (void *) WRITING_IS_DONE;

}

void* readFromFD(void* arg) {  //parent
		
	read(p[0], text2, sizeof(text2));
		
	while (isReadingFile); 
			
	fwrite(&text2, strlen(text2), 1, fp); //записал новое число (отправленное ребенком) в файл.
             
  printf("THIS HAS BEEN READ FROM FD: %s\n", text2);

   return (void *) READING_IS_DONE;
}


int main() {

	int i, j;

	if (pipe(p)) {
        perror("pipe() error");
        exit(EXIT_FAILURE);
    }
	
	if((fp=fopen("file.txt", "r+"))==NULL) {
		printf("Errors occurred.");
		exit(EXIT_FAILURE);
	}
	
	key_t key;		
	key = ftok("/etc/fstab", getpid());

	semid = semget(key, 1, 0666 | IPC_CREAT);
	arg.val = SIMULTANEOUSLY_ACTIVE; 
	semctl(semid, 0, SETVAL, arg);
	
	while(1){
		status[0] = 0;
	
		pthread_create(&tid[0], NULL, readFromFD, NULL);
		
		for(i = 1; i < THREADS_COUNT; i++){
			pthread_create(&tid[i], NULL, sendToFD, NULL);
		}
		
		
		for(j = 0; j < THREADS_COUNT; j++){
			pthread_join(tid[j], &status[j]);
		}
				
		sleep(3);
	}
		
	semctl(semid, 0, IPC_RMID);
	fclose (fp);

  exit(EXIT_SUCCESS);
}
