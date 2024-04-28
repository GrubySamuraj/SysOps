#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 8

// argv[0] - path

int main(int argc, char *argv[])
{
    int fd_out, fd;
    ssize_t bytes_read;
    char *path = argv[0];
    char buffer[BUFFER_SIZE];

    printf("argc: %d\n", argc);
    printf("argv[0]: %s\n", argv[0]);
    printf("argv[1]: %s\n", argv[1]);

    fd_out = open(argv[1], O_WRONLY | O_TRUNC);
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

    close(fd_out);
    close(fd);
    return 0;
}