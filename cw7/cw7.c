#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <wait.h>
#include <signal.h>
#include <assert.h>
#include "semafory.h"
#include "dzielona.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5  // Liczba elementow bufora

// argv[1] - nazwa programu potomnego (konsumenta)
// argv[2] - nazwa programu macierzystego (producenta)
// argv[3] - nazwa pliku konsumenta do zapisywania
// argv[4] - nazwa pliku producenta do czytania

void sigint_handler(int sig)
{
    printf("\nOtrzymano sygnał SIGINT (Ctrl+C).\n");
    assert(removeSem("/semKonsument"));
    assert(removeSem("/semProducent"));
    assert(removeMem("/memProdKons"));
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // Segment pamieci dzielonej
    typedef struct
    {
        char bufor[NBUF][NELE]; // Wspolny bufor danych
        int wstaw, wyjmij;      // Pozycje wstawiania i wyjmowania z bufora
    } SegmentPD;
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        perror("Błąd podczas rejestracji obsługi sygnału SIGINT");
        exit(EXIT_FAILURE);
    }
    const char *semKonsumentName = "/semKonsument";
    const char *semProducentName = "/semProducent";
    const char *memName = "/memProdKons";
    // removeMem(memName);
    sem_t *semKonsument = createSem(semKonsumentName, 0);
    sem_t *semProducent = createSem(semProducentName, NBUF);
    int mem = createMem(memName, sizeof(SegmentPD));
    int pid = fork();
    if (argc != 5)
    {
        perror("Zła ilosc argumentow, powinno byc 5\n");
        exit(EXIT_FAILURE);
    }
    switch (pid)
    {
    case -1:
        // błąd
        perror("Błąd forka");
        exit(EXIT_FAILURE);
        break;
    case 0:
        // child
        // producent
        if (execlp(argv[1], argv[1], semKonsumentName, semProducentName, argv[4], memName, (char *)NULL) == -1)
        {
            perror("execlp error w child");
            exit(EXIT_FAILURE);
        }
        break;
    default:
        // parent
        // konsument
        if (execlp(argv[2], argv[2], semKonsumentName, semProducentName, argv[3], memName, (char *)NULL) == -1)
        {
            perror("execlp error w parent");
            exit(EXIT_FAILURE);
        }
        if (wait(NULL) == -1)
        {
            perror("wait error");
            exit(EXIT_FAILURE);
        }
        break;
    }
    removeMem(memName);
    removeSem(semKonsumentName);
    removeSem(semProducentName);
}