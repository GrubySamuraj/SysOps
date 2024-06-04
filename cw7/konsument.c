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
// argv[3] - nazwa pliku konsumenta do zapisu
// argv[4] - nazwa pamięci dzielonej
int main(int argc, char *argv[])
{
    int fd_out;
    ssize_t bytes_written;
    char buffer[NELE];

    typedef struct
    {
        char bufor[NBUF][NELE]; // Wspólny bufor danych
        int wyjmij;             // Pozycje wstawiania i wyjmowania z bufora
        int wstaw;
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

    fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd_out == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        waitSem(semKonsument);
        printf("Obniżono semafor Konsumenta");
        // sekcja krytyczna - semafor obniżony
        strncpy(buffer, fdmem->bufor[fdmem->wyjmij], sizeof fdmem->bufor[fdmem->wyjmij]);
        printf("Wyciagieto z pamieci dzielonej: %s\n", buffer);
        fdmem->wyjmij = (fdmem->wyjmij + 1) % NBUF;
        // podniesienie semafora
        raiseSem(semProducent);
        printf("Podniesienie semafora producenta\n");
        bytes_written = write(fd_out, buffer, sizeof(buffer));
        if (bytes_written == -1)
        {
            perror("write function error");
            exit(EXIT_FAILURE);
        }

        if (bytes_written == 0)
        {
            printf("Koniec pliku\n");
            break; // Koniec pliku
        }
    }

    unMapMem((int *)fdmem, sizeof(SegmentPD));

    closeSem(semProducent);
    closeSem(semKonsument);
    if (close(fd_out) == -1)
    {
        perror("close error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
