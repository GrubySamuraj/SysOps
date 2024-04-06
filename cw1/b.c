#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// UID, GID, PID, PPID i PGID
// b) Wywolac funkcje fork trzy razy (najlepiej w petli for) i wypisac powyzsze iden-
// tyfikatory dla procesu macierzystego oraz wszystkich procesow potomnych (najle-
// piej dla kazdego procesu w pojedynczej linii). Przy pomocy funkcji wait sprawic,
// aby proces macierzysty zaczekal na zakonczenie wszystkich procesow potomnych.

int main()
{
    printf("START\n");
    printf("\n---------------------------------------------------------\n");
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
            pid_t pid = getpid();
            gid_t gid = getgid();
            pid_t ppid = getppid();
            pid_t pgid = getpgid(pid);
            if (pgid == -1)
            {
                perror("pgid error");
                exit(1);
            }
            printf("potomne PID:%d, UID: %d, gid: %d, ppid: %d, pgid: %d \n", pid, uid, gid, ppid, pgid);
            break;
        default:
            // proces macierzysty:
            pid_t parentPid = getpid();
            uid_t parentUid = getuid();
            gid_t parentGid = getgid();
            pid_t parentPpid = getppid();
            pid_t parentPgid = getpgid(parentPid);
            if (parentPgid == -1)
            {
                perror("pgid error");
                exit(1);
            }
            printf("macierzyste PID:%d, UID: %d, gid: %d, ppid: %d, pgid: %d \n", parentPid, parentUid, parentGid, parentPpid, parentPgid);
            if (wait(NULL) == -1)
            {
                perror("wait error");
                exit(1);
            }
            break;
        }
    }
    printf("\n---------------------------------------------------------\n");
    printf("STOP\n");
    return 0;
}