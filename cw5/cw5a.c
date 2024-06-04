#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Jakub Dragosz

#define BUFFER_SIZE 50

void removeFifo()
{
    unlink("fifarafa");
}

int main(int argc, char *argv[])
{
    int fd_in, fd_out, fd;
    ssize_t bytes_read, bytes_written;
    int pid;
    char *path = "fifarafa";

    if (argc != 3)
    {
        perror("Zla ilosc argumentow! Powinno to wygladac tak: ./prog <read_file> <write_file>");
        exit(-1);
    }

    atexit(removeFifo);
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
        char buffer[BUFFER_SIZE];
        fd_in = open(argv[1], O_RDONLY);
        fd = open(path, O_WRONLY);

        if (fd_in == -1)
        {
            perror("Failed to open readFile ");
            exit(EXIT_FAILURE);
        }
        if (fd < 0)
        {
            printf("Błąd podczas otwierania potoku nazwanego do zapisu: %d.\n", fd);
            exit(EXIT_FAILURE);
        }
        while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0)
        {
            // Zapisanie danych do potoku nazwanego
            int result = write(fd, buffer, bytes_read);

            if (result < 0)
            {
                printf("Błąd podczas zapisywania danych do potoku nazwanego.\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("Dane zostały pomyślnie zapisane do potoku nazwanego.\n");
            }
        }
        if (read(fd_in, buffer, BUFFER_SIZE) == -1)
        {
            perror("read error");
            exit(EXIT_FAILURE);
        }
        // Zamknięcie potoków
        close(fd);
        close(fd_in);
        break;
    }
    default:
    {
        // proces macierzysty
        char buffer[BUFFER_SIZE];
        close(argv[2]);
        fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT);
        if (fd_out == -1)
        {
            perror("Failed to open writeFile");
            exit(EXIT_FAILURE);
        }
        fd = open(path, O_RDONLY);
        while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
        {
            ssize_t bytes_written = write(fd_out, buffer, bytes_read);
            printf("Odebrano od consumera: %s\n", buffer);
            if (bytes_written == -1)
            {
                close(fd_out);
                close(fd);
                perror("Write error\n");
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == -1)
        {
            perror("Blad podczas odczutu z potoku");
            close(fd_out);
            close(fd);
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