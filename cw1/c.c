#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// program wywoluje 3 razy funkcje fork oraz funkcje sleep aby uspic proces potomny aby macierzysty zdazyl sie skonczyc
// i aby procesy potomne zostaly przejete przez demon systemd albo init
int main()
{
    printf("START\n");
    printf("\n---------------------------------------------------------\n");
    pid_t pid = getpid();
    for (int i = 0; i < 3; i++)
    {
        printf("for wykonał się: %d raz\n", i);
        switch (fork())
        {
        case -1:
            perror("fork error");
            exit(1);
        case 0:
            // proces potomny
            uid_t uid = getuid();
            pid = getpid();
            gid_t gid = getgid();
            pid_t ppid = getppid();
            pid_t pgid = getpgid(pid);
            printf("potomne PID:%d, UID: %d, gid: %d, ppid: %d, pgid: %d \n", pid, uid, gid, ppid, pgid);
            sleep(20); // spi przez 20 sekund, aby proces macierzysty zdazyl sie skonczyc
            break;
        default:
            // proces macierzysty:
            pid_t parentPid = getpid();
            uid_t parentUid = getuid();
            gid_t parentGid = getgid();
            pid_t parentPpid = getppid();
            pid_t parentPgid = getpgid(parentPid);
            printf("macierzyste PID:%d, UID: %d, gid: %d, ppid: %d, pgid: %d \n", parentPid, parentUid, parentGid, parentPpid, parentPgid);
            break;
        }
    }
    printf("\n---------------------------------------------------------\n");
    printf("STOP\n");
    return 0;
}