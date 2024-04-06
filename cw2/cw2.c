#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#define _XOPEN_SOURCE >= 500 || /* Since glibc 2.12: */ _POSIX_C_SOURCE >= 200809L

/*
Dany program wywoluje forka 3krotnie i generuje pid, uid, gid, ppid oraz pgid i wywoluje execlp aby wywolac program ktory jest argumentem wywolania
./a.out ./potomne.out

*/

int main(int argc, char *argv[])
{
    printf("START\n");
    printf("\n---------------------------------------------------------\n");
    for (int i = 0; i < 3; i++)
    {
        switch (fork())
        {
        case -1:
            perror("fork error");
            exit(1);
        case 0:;
            // proces potomny
            // char pidString[] = getpid() + "\0";
            pid_t pid = getpid();
            uid_t uid = getuid();
            gid_t gid = getgid();
            pid_t ppid = getppid();
            gid_t pgid = getpgid(pid);
            char pidStr[30];
            char uidStr[30];
            char gidStr[30];
            char ppidStr[30];
            char pgidStr[30];
            sprintf(pidStr, "%d", pid);
            sprintf(uidStr, "%d", uid);
            sprintf(gidStr, "%d", gid);
            sprintf(ppidStr, "%d", ppid);
            sprintf(pgidStr, "%d", pgid);
            int err = 0;
            err = execlp(argv[1], argv[1], pidStr, uidStr, gidStr, ppidStr, pgidStr, (char *)NULL);
            if (err != 0)
            {
                perror("execlp error");
                exit(2);
            }
            break;
        default:;
            // proces macierzysty:
            if (wait(NULL) == -1)
            {
                perror("wait error");
                exit(1);
            }
            break;
        }
        printf("\n---------------------------------------------------------\n");
        printf("STOP\n");
    }
    return 0;
}