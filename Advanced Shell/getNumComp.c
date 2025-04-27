#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {

    // Check the number of lines in camp_partic.txt
    int fd = open("Black_Friday/camp_partic.txt", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    char buffer[BUFFER_SIZE + 1]; // +1 for null terminator
    ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);
    if (bytesRead < 0) {
        perror("Failed to read file");
        close(fd);
        return 1;
    }

    buffer[bytesRead] = '\0'; // Null-terminate
    close(fd);

    // Counting the lines
    int counter = 0;
    int i = 0;
    char c = buffer[i++];
    while(c) {
        if(c == '\n') counter++;
        c = buffer[i++];
    }

    printf("%d companies take part in the Campaign\n", counter);

    return 0;
}