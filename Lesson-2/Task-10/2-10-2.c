#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd;
  char buf[80];

  char* fifoChannelName = "/tmp/fifo0001.1";
  mkfifo(fifoChannelName, 0666);
  
  while (1) {
      fd = open(fifoChannelName,O_WRONLY);
      fgets(buf, 80, stdin);
      write(fd, buf, strlen(buf)+1);
      close(fd);
      sleep(5);
  }
  exit(EXIT_SUCCESS);
}
