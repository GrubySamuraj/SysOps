#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <wait.h>

int main()
{
    for (int i = 0; i < 3; i++)
    {
        pid_t pid = fork();
        switch (pid)
        {
        case -1:
            perror("fork error");
            exit(1);
        case 0:;
            // proces potomny
            int err = execlp("./output/cw3a", "./output/cw3a", "1", "10", (char *)NULL);
            if (err != 0)
            {
                perror("execlp error");
                exit(2);
            }
            break;
        default:;
            // proces macierzysty:
            break;
        }
    }
    int status;
    pid_t wpid;
    while ((wpid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Child process (PID: %d) exited with status: %d\n", wpid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            int signal_num = WTERMSIG(status);
            printf("Child process (PID: %d) terminated by signal: %d (%s)\n", wpid, signal_num, strsignal(signal_num));
        }
    }
    return 0;
}