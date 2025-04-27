#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define MAX_CHARS 256

int main(int argc, char *args[]) {

    char filename[MAX_CHARS];
    strcpy(filename, "Black_Friday/");
    strcat(filename, args[1]);
    strcat(filename, ".txt");

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("Flyer with name %s doesn't exist\n", args[1]);
        return 0;
    }

    char buffer[BUFFER_SIZE + 1]; // +1 for null terminator
    ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);
    if (bytesRead < 0) {
        perror("Failed to read file");
        close(fd);
        return 0;
    }

    buffer[bytesRead] = '\0'; // Null-terminate
    printf("Flyer content:\n%s", buffer);

    close(fd);

    return 0;
}