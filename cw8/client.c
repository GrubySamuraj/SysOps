#include "kolejka.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

Queue server_queue;
Queue client_queue;
char client_queue_name[QUEUE_NAME_LENGTH];

void cleanup()
{
    if (closeQueue(&client_queue) == -1)
    {
        fprintf(stderr, "Failed to close client queue.\n");
    }
    if (unlinkQueue(client_queue_name) == -1)
    {
        fprintf(stderr, "Failed to unlink client queue.\n");
    }
    printf("Client queue closed and unlinked.\n");
}

void handle_signal(int sig)
{
    exit(0);
}

void random_sleep()
{
    srand(time(NULL));
    sleep(rand() % 3 + 1); // Sleep for 1 to 3 seconds
}

int main()
{
    snprintf(client_queue_name, sizeof(client_queue_name), "/%d", getpid());
    if (createQueue(&client_queue, client_queue_name, O_RDONLY) == -1)
    {
        exit(EXIT_FAILURE);
    }
    if (openQueue(&server_queue, SERVER_QUEUE_NAME, O_WRONLY) == -1)
    {
        exit(EXIT_FAILURE);
    }

    atexit(cleanup);
    signal(SIGINT, handle_signal);

    printQueueAttributes(&client_queue);

    char input[MAX_MESSAGE_LENGTH];

    while (1)
    {
        printf("Enter expression (or Ctrl+D to quit): ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        char message[MAX_MESSAGE_LENGTH];
        snprintf(message, sizeof(message), "%s %s", client_queue_name, input);
        sendMessage(&server_queue, message);

        random_sleep();

        char response[MAX_MESSAGE_LENGTH];
        if (receiveMessage(&client_queue, response, sizeof(response)) == -1)
        {
            continue; // Log error and continue
        }
        printf("Server response: %s\n", response);
    }

    return 0;
}
