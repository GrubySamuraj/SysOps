#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/*
Dany program ma mozliwosc wyboru co zrobic z sygnalem SIGTSTP (kombinacja CTRL+Z)
./cw3a.out <opcja> <numer_sygnalu> np. cw3a.out 1 20, gdzie mozna wybierac:
0 oznacza operacje domyslna
1 oznacza przekazanie do funkcji, ktora sprintuje komunikat
2 oznacza zignorowanie
*/
void handler()
{
    printf("PID: %d,PGID: %d\n", getpid(), getpgrp());
    exit(0);
}
int main(int nargs, char *args[])
{
    // int sig = SIGTSTP;
    int sig = atoi(args[2]);
    if (nargs > 3)
    {
        printf("Za duza ilosc parametrow, powinno to wygladac tak: ./cw3a.out <parametr> <numer_signalu>\n");
        exit(-1);
    }
    typedef void (*sighandler_t)(int);
    if (atoi(args[1]) == 0)
    {
        // operacja domyslna
        sighandler_t err = signal(sig, SIG_DFL);
        if (err == -1)
        {
            perror("Wystapil blad w funkcji signal");
            exit(-1);
        }
        if (pause() != -1)
        {
            perror("Wystapil blad w funkcji pause");
            exit(-1);
        }
        // printf("pid:%d\n", getpid());
    }
    else if (atoi(args[1]) == 1)
    {
        // przekazanie
        sighandler_t err = signal(sig, handler);
        printf("Signal number(przekazanie): %d\n", sig);
        if (err == -1)
        {
            perror("Wystapil blad w funkcji signal");
            exit(-1);
        }
        if (pause() != -1)
        {
            perror("Wystapil blad w funkcji pause");
            exit(-1);
        }
    }
    else if (atoi(args[1]) == 2)
    {
        // ignorowanie
        sighandler_t err = signal(sig, SIG_IGN);
        if (err == -1)
        {
            perror("Wystapil blad w funkcji signal");
            exit(-1);
        }
        if (pause() != -1)
        {
            perror("Wystapil blad w funkcji pause");
            exit(-1);
        }
    }
    else
    {
        perror("Nie ma takiej opcji\n");
        exit(-1);
    }
    return 0;
}