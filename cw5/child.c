#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 8

// argv[1] - path
// argv[2] - producent.txt

int main(int argc, char *argv[])
{
    char *path = argv[0];
    char buffer[BUFFER_SIZE];
    printf("argv[1], %s\n", argv[1]);
    int fd_in = open(argv[1], O_RDONLY);
    int fd = open(path, O_WRONLY);
    ssize_t bytes_read;

    printf("child dziala!\n");

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
        // producent
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
    if (bytes_read == -1)
    {
        perror("Błąd w funkcji read");
        exit(EXIT_FAILURE);
    }
    // Zamknięcie potoków
    close(fd);
    close(fd_in);
    return 0;
}