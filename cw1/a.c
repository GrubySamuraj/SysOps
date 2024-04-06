#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("START\n");
    printf("\n---------------------------------------------------------\n");
    uid_t uid = getuid();
    gid_t gid = getuid();
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgid(pid);
    printf("pid: %d\n", pid);
    printf("gid: %d\n", gid);
    printf("uid: %d\n", uid);
    printf("ppid: %d\n", ppid);
    printf("pgid: %d\n", pgid);
    printf("\n---------------------------------------------------------\n");
    printf("STOP\n");
    return 0;
}