#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#ifndef _DZIELONA_H_
#define _DZIELONA_H_

int createMem(char *name, int len)
{
    int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (ftruncate(fd, len) == -1)
    {
        perror("Problem z wydluzeniem pamieci dzielonej");
        exit(EXIT_FAILURE);
    }
    if (fd == -1)
    {
        perror("Nie udało się utworzyć/otworzyć pamięci dzielonej\n");
        printf("line: %d \n", __LINE__);
        printf("file: %s \n", __FILE__);
        exit(EXIT_FAILURE);
        return NULL;
    }
    return fd;
}

int openMem(char *name)
{
    int fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Nie udało się utworzyć/otworzyć pamięci dzielonej\n");
        printf("line: %d \n", __LINE__);
        printf("file: %s \n", __FILE__);
        exit(EXIT_FAILURE);
        return NULL;
    }
    return fd;
}

int *mapMem(int fd, int size)
{
    int *mem = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED)
    {
        perror("Nie udało się zmapować pamięci dzielonej\n");
        printf("line: %d \n", __LINE__);
        printf("file: %s \n", __FILE__);
        exit(EXIT_FAILURE);
        return NULL;
    }
    return mem;
}

int unMapMem(int *addr, int size)
{
    if (munmap(addr, size) == -1)
    {
        perror("Nie udało się zunmapować pamięci dzielonej\n");
        printf("line: %d \n", __LINE__);
        printf("file: %s \n", __FILE__);
        exit(EXIT_FAILURE);
        return 0;
    }
    return 1;
}
#endif
