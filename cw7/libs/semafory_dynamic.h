#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef _SEMAFORY_H_
#define _SEMAFORY_H_

sem_t *createSem(const char *name);
sem_t *openSem(const char *name);
int checkSem(sem_t *sem);
int raiseSem(sem_t *sem);
int waitSem(sem_t *sem);
int closeSem(sem_t *sem);
int removeSem(const char *name);

#endif
