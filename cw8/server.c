#include "kolejka.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

Queue server_queue;

void cleanup()
{
    if (closeQueue(&server_queue) == -1)
    {
        fprintf(stderr, "Failed to close server queue.\n");
    }
    if (unlinkQueue(SERVER_QUEUE_NAME) == -1)
    {
        fprintf(stderr, "Failed to unlink server queue.\n");
    }
    printf("Server queue closed and unlinked.\n");
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
    if (createQueue(&server_queue, SERVER_QUEUE_NAME, O_RDONLY) == -1)
    {
        exit(EXIT_FAILURE);
    }

    atexit(cleanup);
    signal(SIGINT, handle_signal);

    printQueueAttributes(&server_queue);

    printf("Server started. Waiting for messages...\n");

    while (1)
    {
        char msg[MAX_MESSAGE_LENGTH];
        if (receiveMessage(&server_queue, msg, sizeof(msg)) == -1)
        {
            continue; // Log error and continue
        }
        printf("Received message: %s\n", msg);

        char client_queue_name[QUEUE_NAME_LENGTH];
        char expression[MAX_MESSAGE_LENGTH];
        if (sscanf(msg, "%s %[^\n]", client_queue_name, expression) != 2)
        {
            fprintf(stderr, "Failed to parse message: %s\n", msg);
            continue;
        }

        Queue client_queue;
        if (openQueue(&client_queue, client_queue_name, O_WRONLY) == -1)
        {
            fprintf(stderr, "Failed to open client queue: %s\n", client_queue_name);
            continue; // Log error and continue
        }

        double num1, num2, result = 0;
        char operator;
        char response[MAX_MESSAGE_LENGTH];

        if (sscanf(expression, "%lf %c %lf", &num1, &operator, & num2) != 3)
        {
            snprintf(response, sizeof(response), "Invalid expression.");
        }
        else
        {
            printf("Parsed expression: %lf %c %lf\n", num1, operator, num2);
            switch (operator)
            {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0)
                {
                    snprintf(response, sizeof(response), "Division by zero.");
                    break;
                }
                result = num1 / num2;
                break;
            default:
                snprintf(response, sizeof(response), "Unknown operator.");
                break;
            }
            snprintf(response, sizeof(response), "Result: %.2lf", result);
        }

        printf("Calculated result: %s\n", response);

        random_sleep();

        if (sendMessage(&client_queue, response) == -1)
        {
            fprintf(stderr, "Failed to send message to client queue: %s\n", client_queue_name);
        }
        else
        {
            printf("Message sent to client queue: %s\n", client_queue_name);
        }

        if (closeQueue(&client_queue) == -1)
        {
            fprintf(stderr, "Failed to close client queue: %s\n", client_queue_name);
        }
    }

    return 0;
}
