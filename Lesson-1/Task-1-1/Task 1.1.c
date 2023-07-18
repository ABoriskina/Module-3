#include <unistd.h>
#include <stdio.h>

int main() {

    pid_t pid = getpid();
    pid_t ppid = getppid();
    uid_t uid = getuid();
    gid_t gid = getgid();
    pid_t sid = getsid(pid);

    printf("PID of PROG IS %d\n", pid);
    printf("PARENT PID of PROG IS %d\n", ppid);
    printf("USER ID of PROG IS %d\n", uid);
    printf("GID of PROG IS %d\n", gid);
    printf("SID of PROG IS %d\n", sid);
    return 0;

}
