#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <wait.h>
#include "semafory.h"
#include "dzielona.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5  // Liczba elementow bufora

// argv[1] - nazwa programu potomnego (konsumenta)
// argv[2] - nazwa programu macierzystego (producenta)
// argv[3] - nazwa pliku konsumenta do zapisywania
// argv[4] - nazwa pliku producenta do czytania

int main(int argc, char *argv[])
{
    // Segment pamieci dzielonej
    typedef struct
    {
        char bufor[NBUF][NELE]; // Wspolny bufor danych
        int wstaw, wyjmij;      // Pozycje wstawiania i wyjmowania z bufora
    } SegmentPD;
    const char *semKonsumentName = "/semKonsument";
    const char *semProducentName = "/semProducent";
    const char *memName = "/memProdKons";
    sem_t *semKonsument = createSem(semKonsumentName);
    sem_t *semProducent = createSem(semProducentName);
    int mem = createMem(memName, sizeof(SegmentPD));
    int pid = fork();
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
        if (execlp(argv[1], semKonsumentName, semProducentName, NULL) == -1)
        {
            perror("execlp error w child");
            exit(EXIT_FAILURE);
        }
        break;
    default:
        // parent
        // konsument
        if (execlp(argv[2], semProducentName, NULL) == -1)
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
}