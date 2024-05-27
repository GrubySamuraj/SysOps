#ifndef _DZIELONA_H_
#define _DZIELONA_H_

int createMem(char *name, int len);
int openMem(char *name);
int *mapMem(int fd, int size);
int unMapMem(int *addr, int size);

#endif
