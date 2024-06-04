#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

// Stworzyc wlasna biblioteke, prostych w uzyciu funkcji do
// 1.tworzenia,
// 2.otwierania,
// 3.uzyskiwania wartosci
// 4.operowania:
// a) podniesienie semafora
// b) obnizenie semafora
// 5.zamykania
// 6.usuwania semafora
#ifndef _SEMAFORY_H_
#define _SEMAFORY_H_
// tworzenie semafora
sem_t *createSem(const char *name, unsigned int value)
{
    sem_t *sem = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (sem == SEM_FAILED)
    {
        perror("Nie udało się utworzyć semafora\n");
        printf("line: %d \n", __LINE__);
        printf("file: %s \n", __FILE__);
        exit(EXIT_FAILURE);
        return NULL;
    }
    printf("utworzono semafor: %s\n", name);
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
        return NULL;
    }
    return sem;
}

// uzyskiwanie wartosci semafora

int chechSem(sem_t *sem)
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
        return 0;
    }
    return 1;
}

// opuszczenie  semafora
int waitSem(sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("Nie udało się opuścić semafora");
        exit(EXIT_FAILURE);
        return 0;
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
        return 0;
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
        return 0;
    }
    return 1;
}

#endif