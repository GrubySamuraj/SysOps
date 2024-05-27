#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 8

void removeFifo()
{
    unlink("fifarafa");
}

// argv[4] - nazwa programu parent
// argv[5] - nazwa programu child

int main(int argc, char *argv[])
{
    int fd_in, fd_out, fd;
    ssize_t bytes_read, bytes_written;
    int pid;
    char *path = "fifarafa";
    atexit(removeFifo);
    if (argc != 5)
    {
        perror("Zla ilosc argumentow! Powinno to wygladac tak: ./prog <read_file> <write_file>");
        exit(-1);
    }

    int err = mkfifo(path, 0777);
    if (err == 0)
    {
        printf("Potok nazwany został pomyślnie utworzony.\n");
    }
    else
    {
        printf("Błąd podczas tworzenia potoku nazwanego.\n");
        exit(EXIT_FAILURE);
    }
    pid = fork();

    switch (pid)
    {
    case -1:
    {
        perror("Fork error\n");
        exit(EXIT_FAILURE);
        break;
    }
    case 0:
    {
        // proces potomny
        // producent
        printf("argv[3]: %s, argv[1]: %s\n", argv[3], argv[1]);
        if (execlp(argv[3], path, argv[1]) == -1)
        {
            perror("excelp error");
            exit(EXIT_FAILURE);
        }
        break;
    }
    default:
    {
        // proces macierzysty
        // konsument
        printf("argv[4]: %s, argv[2]: %s\n", argv[4], argv[2]);
        if (execlp(argv[4], path, argv[2]) == -1)
        {
            perror("excelp error");
            exit(EXIT_FAILURE);
        }
        if (wait(NULL) == -1)
        {
            perror("wait error");
            exit(EXIT_FAILURE);
        }
        close(fd_out);
        close(fd);
        break;
    }
    }
    return 0;
}