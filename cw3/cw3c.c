#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <signal.h>
#define _XOPEN_SOURCE >= 500 || /* Since glibc 2.12: */ _POSIX_C_SOURCE >= 200809L
/*
Program ten sluzy do uzycia funkcji fork z execlp, gdzie najpierw zmieniamy potomka na wlasciciela grupy, zmieniajac pgid
na jego pid, pozniej odpalamy program forkleader.c, ten zas sluzy do wywolania fork-execlp 3 razy i wywolania programu
cw3a.c z parametrem 1 i 10, co oznacza, ze program ten wywola funkcje ktora wypisze pid i pgid tego procesu, a 10 to id
sygnalu ktory wywoluje (SIGUSR1), program glowny (czyli cw3c.c) i proces macierzysty czekaja za pomoca funkcji sleep i
wysylaja sygnal do calej grupy (-getpgid(pid) oznacza pobranie pgidu procesu potomnego i ustawienie go na -, co oznacza
cala grupe dla kill), uprzednio sprawdzajac czy w ogole istnieja. Pod koniec wypisuje informacje w forkleader oraz w cw3c
o procesach ktore zostaly zakonczone, samoistnie albo przez sygnal
*/

int main()
{
    pid_t pid = fork();
    int sig = SIGUSR1;
    typedef void (*sighandler_t)(int);
    switch (pid)
    {
    case -1:
        perror("fork error");
        exit(1);
    case 0:;
        // proces potomny
        if (setpgid(0, 0) == -1)
        {
            perror("setpgid error");
            exit(-1);
        }
        sighandler_t err = signal(sig, SIG_IGN);
        if (err == -1)
        {
            perror("Wystapil blad w funkcji signal");
            exit(-1);
        }
        int err2 = execlp("./output/forkleader", "./output/forkleader", (char *)NULL);
        if (err2 != 0)
        {
            perror("execlp error");
            exit(2);
        }
        break;
    default:;
        // proces macierzysty:
        sleep(4);
        int pgid = getpgid(pid);
        if (pgid == -1)
        {
            perror("getpgid error");
            exit(EXIT_FAILURE);
        }
        if (kill(-pgid, 0) == 0)
        {
            printf("Wyslano sygnal do procesu potomnego\n");
            sleep(4);
            kill(-pgid, sig);
        }
        else
        {
            printf("Proces potomny nie istnieje\n");
        }
        int statusParent;
        if (wait(&statusParent) == -1)
        {
            perror("wait error");
            exit(1);
        }
        if (WIFEXITED(statusParent))
        {
            printf("Proces potomny zwrocil: %d\n", WEXITSTATUS(statusParent));
        }
        else if (WIFSIGNALED(statusParent))
        {
            int signal_num = WTERMSIG(statusParent);
            printf("Proces potomny pid:(%d) zostal zakonczony przez sygnal: %d (%s) i ma status %d\n", pid, signal_num, strsignal(signal_num), statusParent);
        }
        break;
    }
    return 0;
}