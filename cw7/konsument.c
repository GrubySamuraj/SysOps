#include <string.h>
#include "semafory.h"
#include "dzielona.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5  // Liczba elementow bufora

// argv[1] - nazwa semafora konsumenta
// argv[2] - nazwa semafora producenta
// argv[3] - nazwa pliku konsumenta do zapisu
// argv[4] - nazwa pamięci dzielonej
int main(int argc, char *argv[])
{
    int fd_out;
    ssize_t bytes_read, bytes_written;
    int wstaw = 0;
    char buffer[NELE];
    int pid;

    typedef struct
    {
        char bufor[NBUF][NELE]; // Wspolny bufor danych
        int wstaw, wyjmij;      // Pozycje wstawiania i wyjmowania z bufora
    } SegmentPD;
    if (argc != 4)
    {
        perror("Za mała ilość argumentów!");
        exit(EXIT_FAILURE);
    }
    // sekcja prywatna
    sem_t *semKonsument = openSem(argv[1]);
    sem_t *semProducent = openSem(argv[2]);
    int memory = openMem(argv[4]);
    SegmentPD *fdmem = (SegmentPD *)mapMem(memory, sizeof(SegmentPD));

    fd_out = open(argv[3], O_RDONLY);
    if (fd_out == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // produkcja towaru
        if (bytes_read = write(fd_out, buffer, NELE) == -1)
        {
            perror("read function error");
            exit(EXIT_FAILURE);
        }

        waitSem(semKonsument);
        // sekcja krytyczna - semafor obniżony

        fdmem->wyjmij = (fdmem->wyjmij + 1) % NELE;
        // strncpy(fdmem->bufor[fdmem->wyjmij]);
        printf("Proces potomny %d zajął semafor.\n", getpid());
        // podniesienie semafora
        raiseSem(semProducent);
    }
    closeSem(semProducent);
    closeSem(semKonsument);

    exit(EXIT_SUCCESS);
    return 0;
}