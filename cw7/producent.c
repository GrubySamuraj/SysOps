#include <string.h>
#include "semafory.h"
#include "dzielona.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5  // Liczba elementow bufora

// argv[1] - nazwa semafora konsumenta
// argv[2] - nazwa semafora producenta
// argv[3] - nazwa pliku konsumenta
// argv[4] - nazwa pamięci dzielonej
int main(int argc, char *argv[])
{
    int fd_in;
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

    while (1)
    {
        // produkcja towaru
        if (bytes_read = read(fd_in, buffer, NELE) == -1)
        {
            perror("read function error");
            exit(EXIT_FAILURE);
        }

        waitSem(semProducent);
        // sekcja krytyczna - semafor obniżony

        fdmem->wstaw = (fdmem->wstaw + 1) % NELE;
        strcpy(fdmem->bufor[fdmem->wstaw], bytes_read);
        // fdmem->bufor[fdmem->wstaw] = bytes_read;

        printf("Proces potomny %d zajął semafor.\n", getpid());
        raiseSem(semKonsument);
        // podniesienie semafora
    }
    closeSem(semProducent);
    closeSem(semKonsument);

    exit(EXIT_SUCCESS);
    return 0;
}