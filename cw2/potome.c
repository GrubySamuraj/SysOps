#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

// program printuje argumenty przekeazane w funkcji, maja byc to pid, gid uid ppid oraz pgid z cw2.c

int main(int argc, char *argv[])
{
    printf("potomne: pid: %s, uid: %s,gid: %s, ppid: %s, pgid: %s \n", argv[1], argv[2], argv[3], argv[4], argv[5]);
    return 0;
}