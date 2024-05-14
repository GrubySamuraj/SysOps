#include "semafory.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <wait.h>
#include <assert.h>
#include <signal.h>

void cleanup()
{
    removeSem("/semPowielacz");
}

void sigint_handler(int sig)
{
    printf("\nOtrzymano sygnał SIGINT (Ctrl+C).\n");
    assert(removeSem("/semPowielacz"));
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // printf("argv[1]: %s\n", argv[1]);
    // argv[1] - nazwa programu potomnego
    // argv[2] - liczba procesów potomnych
    // argv[3] - liczba sekcji prywatnych
    int numProcesses = 3; // argv[2]
    const char *semName = "/semPowielacz";
    sem_t *semaphore = createSem(semName);
    int val = chechSem(semaphore);
    printf("semaphore address: %d, value: %d \n", *semaphore, val);
    FILE *file = fopen("numer.txt", "w");
    if (file == NULL)
    {
        perror("Błąd podczas otwierania pliku numer.txt");
        return 1;
    }

    fprintf(file, "0"); // Zapisanie wartości 0 do pliku
    fclose(file);

    if (atexit(cleanup) != 0)
    {
        perror("Błąd podczas rejestracji funkcji cleanup() przez atexit()");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        perror("Błąd podczas rejestracji obsługi sygnału SIGINT");
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    for (int i = 0; i < numProcesses; ++i)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Błąd podczas tworzenia procesu potomnego");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Kod procesu potomnego
            if (execlp(argv[1], argv[1], NULL) == -1)
            {
                perror("execlp error");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            //  Kod procesu macierzystego
            if (wait(NULL) == -1)
            {
                perror("wait error");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}