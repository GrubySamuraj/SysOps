#ifndef KOLEJKA_H
#define KOLEJKA_H

#include <mqueue.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define QUEUE_NAME_LENGTH 50
#define MAX_MESSAGE_LENGTH 50
#define SERVER_QUEUE_NAME "/server_queue"

typedef struct
{
    char name[QUEUE_NAME_LENGTH];
    mqd_t descriptor;
} Queue;

// Funkcja tworząca nową kolejkę komunikatów
int createQueue(Queue *queue, const char *name, int flags)
{
    strncpy(queue->name, name, QUEUE_NAME_LENGTH);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MESSAGE_LENGTH;
    attr.mq_curmsgs = 0;

    queue->descriptor = mq_open(name, flags | O_CREAT, 0644, &attr);
    if (queue->descriptor == (mqd_t)-1)
    {
        perror("mq_open");
        return -1;
    }
    return 0;
}

int openQueue(Queue *queue, const char *name, int flags)
{
    strncpy(queue->name, name, QUEUE_NAME_LENGTH);

    queue->descriptor = mq_open(name, flags);
    if (queue->descriptor == (mqd_t)-1)
    {
        perror("mq_open");
        return -1;
    }
    return 0;
}

// Funkcja wysyłająca komunikat do kolejki
int sendMessage(Queue *queue, const char *message)
{
    if (mq_send(queue->descriptor, message, strlen(message) + 1, 0) == -1)
    {
        perror("mq_send");
        return -1;
    }
    return 0;
}

// Funkcja odbierająca komunikat z kolejki
int receiveMessage(Queue *queue, char *buffer, size_t buffer_size)
{
    ssize_t bytes_read = mq_receive(queue->descriptor, buffer, buffer_size, NULL);
    if (bytes_read == -1)
    {
        perror("mq_receive");
        return -1;
    }
    return 0;
}

// Funkcja zamykająca kolejkę komunikatów
int closeQueue(Queue *queue)
{
    if (mq_close(queue->descriptor) == -1)
    {
        perror("mq_close");
        return -1;
    }
    return 0;
}

// Funkcja usuwająca kolejkę komunikatów
int unlinkQueue(const char *name)
{
    if (mq_unlink(name) == -1)
    {
        perror("mq_unlink");
        return -1;
    }
    return 0;
}

void printQueueAttributes(Queue *queue)
{
    struct mq_attr attr;
    if (mq_getattr(queue->descriptor, &attr) == -1)
    {
        perror("mq_getattr");
        return;
    }
    printf("Queue Name: %s\n", queue->name);
    printf("Descriptor: %d\n", queue->descriptor);
    printf("Attributes: Flags=%ld, Maxmsg=%ld, Msgsize=%ld, Curmsgs=%ld\n",
           attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
}

#endif // KOLEJKA_H
