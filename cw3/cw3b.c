#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <string.h>

/*
Dany program wywoluje forka razem z execlp i wywoluje w nim cw3a, pozniej w procesie macierzystym wypisuje PID oraz
wysyla sygnal uzytkownika do procesu potomnego, uprzednio sprawdzajac czy on istnieje (poprzez funkcje kill),
czeka az proces potomny sie wykona oraz sprawdza za pomoca makra w jaki sposob wyszedl proces, czy poprzez sygnal
czy samoistnie. Pod koniec wypisuje PID procesu potomnego, status oraz id sygnalu oraz nazwe sygnalu.
*/

int main()
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork error");
        exit(1);
    case 0:;
        // proces potomny
        int err = 0;
        err = execlp("./output/cw3a", "./output/cw3a", "0", "20", (char *)NULL);
        if (err != 0)
        {
            perror("execlp error");
            exit(2);
        }
        printf("Proces potomny, PID: %d", getpid());
        break;
    default:;
        // proces macierzysty:
        printf("ParentID: %d\n", getpid());
        if (kill(pid, 0) == 0)
        {
            printf("Wyslano sygnal do procesu potomnego\n");
            sleep(1);
            kill(pid, SIGUSR1);
            sleep(2);
        }
        else
        {
            printf("Proces potomny nie istnieje\n");
        }

        int status;

        if (wait(&status) == -1)
        {
            perror("wait error");
            exit(1);
        }
        if (WIFEXITED(status))
        {
            printf("Proces potomny zwrocil: %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            int signal_num = WTERMSIG(status);
            printf("Proces potomny pid:(%d) zostal zakonczony przez sygnal: %d (%s) i ma status %d\n", pid, signal_num, strsignal(signal_num), status);
        }
        break;
    }
    return 0;
}