#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define XMAX 45 // Maksymalna liczba kolumn ekranu
#define YMAX 20 // Maksymalna liczba wierszy ekranu

typedef struct
{
    int threadId;
    int numIterations;
} ThreadData;

pthread_mutex_t mutex;
int globalCounter = 0;

void gotoxy(unsigned x, unsigned y)
{
    printf("\033[%d;%dH\033[2K", y, x);
}

// Funkcja wykonywana przez każdy wątek
void *thread_function(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int threadId = data->threadId;
    int numIterations = data->numIterations;
    int privateCounter;

    for (int i = 0; i < numIterations; i++)
    {
        // Sekcja prywatna
        gotoxy(1, threadId + 1);
        printf("Wątek %d: sekcja prywatna\n", threadId + 1);
        sleep(1);

        // Sekcja krytyczna
        pthread_mutex_lock(&mutex);

        privateCounter = globalCounter;
        privateCounter++;
        sleep(1);
        globalCounter = privateCounter;

        gotoxy(XMAX, threadId + 1);
        printf("Wątek %d: sekcja krytyczna, licznik: %d\n", threadId + 1, globalCounter);

        pthread_mutex_unlock(&mutex);
        sleep(1);

        // Sekcja prywatna
        gotoxy(1, threadId + 1);
        printf("Wątek %d: sekcja prywatna\n", threadId + 1);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Użycie: %s <ilość wątków> <ilość iteracji>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numThreads = atoi(argv[1]);
    int numIterations = atoi(argv[2]);
    if (numThreads <= 0)
    {
        fprintf(stderr, "Niepoprawna ilość wątków. Podaj liczbę od 1.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("blad z mutexem");
        exit(EXIT_FAILURE);
    }

    printf("Adres mutexu: %p\n", (void *)&mutex);

    for (int i = 0; i < numThreads; i++)
    {
        threadData[i].threadId = i;
        threadData[i].numIterations = numIterations;
        int create_result = pthread_create(&threads[i], NULL, thread_function, (void *)&threadData[i]);
        if (create_result != 0)
        {
            fprintf(stderr, "Błąd przy tworzeniu wątku %d: %s\n", i, strerror(create_result));
            exit(EXIT_FAILURE);
        }
        printf("Wątek %d utworzony, ID: %lu\n", i, threads[i]);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("\033[0m");
    gotoxy(0, YMAX + 1);
    printf("Wszystkie wątki zakończone. Globalny licznik: %d\n", globalCounter);

    return 0;
}
