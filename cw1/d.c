#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

// program wywoluje 3 razy funkcje fork oraz funkcje sleep aby uspic proces potomny aby macierzysty zdazyl sie skonczyc
// i aby procesy potomne zostaly przejete przez demon systemd albo init

int main()
{
    for (int i = 0; i < 3; i++)
    {
        switch (fork())
        {
        case -1:
            perror("fork error");
            exit(1);
        case 0:
            // proces potomny
            pid_t pid = getpid();
            pid_t ppid = getppid();
            pid_t pgid = getpgid(pid);
            printf("\n|\n");
            printf("\n\n");
            sleep(1);
            printf("\n(%d)(%d)(%d)", pid, ppid, pgid);
            break;
        default:
            // proces macierzysty:
            pid_t parentPid = getpid();
            pid_t parentPpid = getppid();
            pid_t parentPgid = getpgid(parentPid);
            sleep(3);
            printf("\n(%d)(%d)(%d)", parentPid, parentPpid, parentPgid);
            break;
        }
    }
    return 0;
}