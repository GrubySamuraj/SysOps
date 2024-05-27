#include "semafory.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // Wypisywać PID procesu oraz wartość semafora przed, wewnątrz i po sekcji krytycznej,
    // a także numer wykonywanej sekcji krytycznej.
    // opuszczenie semafora
    char *semName = "/semPowielacz";
    // sekcja prywatna
    sem_t *sem = openSem(semName);
    int val = chechSem(sem);
    printf("Proces potomny %d zajął semafor. i ma id: %d oraz val: %d\n", getpid(), sem, val);
    waitSem(sem);
    // sekcja krytyczna
    printf("Proces potomny %d zajął semafor.\n", getpid());
    FILE *file = fopen("numer.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    int final_value;
    fscanf(file, "%d", &final_value);
    final_value++;
    fclose(file);
    sleep(rand() % 3 + 1);
    FILE *file = fopen("numer.txt", "w");
    if (file == NULL)
    {
        perror("Błąd podczas otwierania pliku numer.txt");
        exit(EXIT_FAILURE);
    }

    fprintf(file, final_value);
    fclose(file);
    val = chechSem(sem);
    printf("Proces potomny w sekcji krytycznej %d zajął semafor. i ma id: %d oraz val: %d\n", getpid(), sem, val);
    raiseSem(sem);
    // po
    val = chechSem(sem);
    printf("Proces potomny po sekcji krytycznej %d zajął semafor. i ma id: %d oraz val: %d\n", getpid(), sem, val);
    closeSem(sem);
    exit(EXIT_SUCCESS);
    return 0;
}