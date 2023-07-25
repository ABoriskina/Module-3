#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <malloc.h>
#include <errno.h>
#include <pwd.h>

#define FILE 100
#define CATALOG 111
#define OTHER 112

typedef struct FileSystem {
    struct FileSystem* parent;
    char name[256];
    struct dirent* dir;
    int type;
} FileSystem;

FileSystem fileSystem[1000];

int countFiles = 0;

int setEnvironment(char* location){
    int n = 0;
    DIR *dir = opendir(location);

    struct dirent* newDir;

    while ((newDir = readdir(dir)) != NULL) {

        struct stat buffer;
        char temp[1000];
        strcpy(temp, location);
        strcat(temp, newDir->d_name);

        int fr = stat(temp, &buffer);

        if (newDir->d_name[0] == '.')
            continue;

        if(fr != 0){
            perror("fstat() error");
            exit(EXIT_FAILURE);
        }

        mode_t mode = buffer.st_mode;

        n++;
        fileSystem[countFiles].dir = newDir;
        strcpy(fileSystem[countFiles].name, newDir->d_name);

        if (S_ISLNK(mode)){
            printf("\t%s\t\t[type link]\n", newDir->d_name);
            fileSystem[countFiles].type = OTHER;
        }

        else if (S_ISREG(mode)){
            printf("\t%s\t\t[type regular file]\n",  newDir->d_name);
            fileSystem[countFiles].type = FILE;
        }

        else if (S_ISDIR(mode)){
            printf("%d\t%s\t\t[type catalog]\n", countFiles, newDir->d_name);
            fileSystem[countFiles].type = CATALOG;
        }

        else if (S_ISCHR(mode)){
            printf("\t%s\t\t[type symbolic]\n",  newDir->d_name);
            fileSystem[countFiles].type = OTHER;
        }

        else if (S_ISBLK(mode)){
            printf("\t%s\t\t[type block]\n",  newDir->d_name);
            fileSystem[countFiles].type = OTHER;
        }

        else if (S_ISFIFO(mode)){
            printf("\t%s\t\t[type FIFO channel]\n",  newDir->d_name);
            fileSystem[countFiles].type = OTHER;
        }

        else if (S_ISSOCK(mode)){
            printf("\t%s\t\t[type socket]\n", newDir->d_name);
            fileSystem[countFiles].type = OTHER;
        }
        countFiles++;
    }

    closedir(dir);
    return n;
}

void makeTreeAction(FileSystem* fs, int command){

    FileSystem* f = (FileSystem*)malloc(sizeof(FileSystem));
    f->parent = fs;

    char* location = (char*)malloc(sizeof(char) * 1000);

    strcpy(location, fs->name);

    if (command != -1){

        printf("\n0\t[back]\n");

        setEnvironment(location);

        countFiles = 1;

        printf("\nGO TO CATALOG NUMBER > ");
        char buf[4];
        fgets(buf, sizeof(buf), stdin);

        sscanf(buf, "%d", &command);

        if(command > 0){
            strcat(location, fileSystem[command].name);
            strcat(location, "/");
            strcpy( f->name, location);
            free(location);
            makeTreeAction(f, command);
        }
        else if(command == 0){
            if (fs->parent != NULL){
                free(location);
                makeTreeAction(fs->parent, command);
            }
            else{
                printf("\n THERE IS NO WAY OUTTA HERE SORRY\n");
                free(location);
                free(f);
                makeTreeAction(fs, command);
            }

        }

    }

}

int main (){
    FileSystem* fs = (FileSystem*)malloc(sizeof(FileSystem));
    fileSystem[0].type = CATALOG;
    countFiles++;
    strcpy(fs->name, "/home/");
    fs->type = CATALOG;
    fs->parent=NULL;

    FileSystem* fs1 = (FileSystem*)malloc(sizeof(FileSystem));

    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    strcpy(fs1->name, "/home/");
    strcat(fs1->name, pw->pw_name);
    strcat(fs1->name, "/");
    fs1->parent = fs;

    makeTreeAction(fs1, 0);

    exit(EXIT_SUCCESS);
}
