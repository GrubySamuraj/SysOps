#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 8

int main(int argc, char *argv[])
{
    int filedes[2], fd_in, fd_out;
    ssize_t bytes_read, bytes_written;
    int pid;
    if (argc != 3)
    {
        perror("Zla ilosc argumentow! Powinno to wygladac tak: ./prog <read_file> <write_file>");
        exit(-1);
    }
    if (pipe(filedes) == -1)
    {
        perror("Pipe error");
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
        char buffer[BUFFER_SIZE];
        close(filedes[0]);
        fd_in = open(argv[1], O_RDONLY);
        if (fd_in == -1)
        {
            perror("Failed to open readFile ");
            exit(EXIT_FAILURE);
        }
        while ((bytes_read = read(fd_in, buffer, sizeof(buffer))) > 0)
        {
            ssize_t saved = write(filedes[1], buffer, bytes_read);
            printf("Wyslano za pomoca potoku do consumera: %s\n", buffer);
            if (saved == -1)
            {
                perror("Read error\n");
                close(fd_in);
                close(filedes[1]);
                exit(EXIT_FAILURE);
            }
        }
        close(fd_in);
        close(filedes[1]);
        break;
    }
    default:
    {
        // proces macierzysty
        wait(NULL);
        close(filedes[1]);
        char buffer[BUFFER_SIZE];
        fd_out = open(argv[2], O_WRONLY | O_TRUNC);
        if (fd_out == -1)
        {
            perror("Failed to open writeFile");
            exit(EXIT_FAILURE);
        }
        while ((bytes_read = read(filedes[0], buffer, sizeof(buffer))) > 0)
        {
            ssize_t bytes_written = write(fd_out, buffer, sizeof(buffer));
            printf("Odebrano od consumera: %s\n", buffer);
            if (bytes_written == -1)
            {
                perror("Write error\n");
                close(fd_out);
                close(filedes[0]);
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == -1)
        {
            perror("Blad podczas odczutu z potoku");
            close(fd_out);
            close(filedes[0]);
            exit(EXIT_FAILURE);
        }

        close(fd_out);
        close(filedes[0]);
        break;
    }
    }
    return 0;
}