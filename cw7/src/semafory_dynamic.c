#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../libs/semafory_dynamic.h"

// tworzenie semafora
sem_t *createSem(const char *name)
{
    sem_t *sem = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (sem == SEM_FAILED)
    {
        perror("Nie udało się utworzyć semafora\n");
        printf("line: %d \n", __LINE__);
        printf("file: %s \n", __FILE__);
        exit(EXIT_FAILURE);
    }
    return sem;
}

// otwieranie
sem_t *openSem(const char *name)
{
    sem_t *sem = sem_open(name, S_IRUSR | S_IWUSR);
    if (sem == SEM_FAILED)
    {
        perror("Nie udalo sie otworzyc semafora\n");
        printf("line: %d \n", __LINE__);
        printf("file: %s \n", __FILE__);
        exit(EXIT_FAILURE);
    }
    return sem;
}

// uzyskiwanie wartosci semafora
int checkSem(sem_t *sem)
{
    int value = 0;
    if (sem_getvalue(sem, &value) == -1)
    {
        perror("Nie udalo sie odczytac semafora");
        exit(EXIT_FAILURE);
    }
    return value;
}

int raiseSem(sem_t *sem)
{
    if (sem_post(sem) == -1)
    {
        perror("Nie udało się podnieść semafora");
        exit(EXIT_FAILURE);
    }
    return 1;
}

int waitSem(sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("Nie udało się opuścić semafora");
        exit(EXIT_FAILURE);
    }
    return 1;
}

// zamykanie semafora
int closeSem(sem_t *sem)
{
    if (sem_close(sem) == -1)
    {
        perror("Nie udało się zamknąć semafora");
        exit(EXIT_FAILURE);
    }
    return 1;
}

// usuniecie semafora
int removeSem(const char *name)
{
    if (sem_unlink(name) == -1)
    {
        perror("Nie udało się usunąć semafora");
        exit(EXIT_FAILURE);
    }
    return 1;
}
