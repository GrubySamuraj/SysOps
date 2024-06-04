#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "semafory.h"
#include "dzielona.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5  // Liczba elementów bufora

// argv[1] - nazwa semafora konsumenta
// argv[2] - nazwa semafora producenta
// argv[3] - nazwa pliku producenta do odczytu
// argv[4] - nazwa pamięci dzielonej
int main(int argc, char *argv[])
{
    int fd_in;
    ssize_t bytes_read;
    char buffer[NELE];

    typedef struct
    {
        char bufor[NBUF][NELE]; // Wspólny bufor danych
        int wstaw;              // Pozycje wstawiania i wyjmowania z bufora
        int wyjmij;
    } SegmentPD;

    if (argc != 5)
    {
        fprintf(stderr, "Zła ilość argumentów! Oczekiwano 5.\n");
        exit(EXIT_FAILURE);
    }

    // sekcja prywatna
    sem_t *semKonsument = openSem(argv[1]);
    sem_t *semProducent = openSem(argv[2]);
    int memory = openMem(argv[4]);
    SegmentPD *fdmem = (SegmentPD *)mapMem(memory, sizeof(SegmentPD));

    fd_in = open(argv[3], O_RDONLY);
    if (fd_in == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        bytes_read = read(fd_in, buffer, sizeof buffer);
        if (bytes_read == -1)
        {
            perror("read function error");
            exit(EXIT_FAILURE);
        }

        if (bytes_read == 0)
        {
            break; // Koniec pliku
        }
        waitSem(semProducent);
        printf("Obnizono semafor producenta\n");

        // sekcja krytyczna - semafor obniżony
        strncpy(fdmem->bufor[fdmem->wstaw], buffer, sizeof fdmem->bufor[fdmem->wstaw]);
        printf("Wpisano do pamieci dzielonej: %s\n", buffer);
        fdmem->wstaw = (fdmem->wstaw + 1) % NBUF;

        raiseSem(semKonsument);
        printf("Podniesiono semafor Konsumenta\n");
        // podniesienie semafora
    }

    closeSem(semProducent);
    closeSem(semKonsument);

    unMapMem(fdmem, sizeof(SegmentPD));

    if (close(fd_in) == -1)
    {
        perror("close error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
