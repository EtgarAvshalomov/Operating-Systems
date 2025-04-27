#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_CHARS 256
#define BUFFER_SIZE 1024

int main(int argc, char *args[]) {

    // Check if the name of the company exists in camp_partic
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

    int seen = 0;
    int bIndex = 0;
    int wIndex = 0;
    char c = buffer[bIndex++];
    while(c) {
        if(c == args[1][wIndex]) wIndex++;
        else wIndex = 0;
        if(args[1][wIndex] == '\0') {
            seen = 1;
            break;
        }
        c = buffer[bIndex++];
    }

    close(fd);

    if(!seen) {
        printf("Company %s doesn't exist\n", args[1]);
        return 1;
    }

    DIR *dir;
    struct dirent *entry;
    int file_count = 0;

    // Open the directory
    char filepath[MAX_CHARS];
    strcpy(filepath, "Black_Friday/Order ");
    strcat(filepath, args[1]);
    dir = opendir(filepath);
    if (dir == NULL) {
        printf("%s has no orders\n", args[1]);
        return 1;
    }

    // Read entries and count them
    entry = readdir(dir);
    while (entry != NULL) {
        // Increment count for every entry (file or directory)
        if (entry->d_name[0] != '.') {
            file_count++;
        }
        entry = readdir(dir);
    }

    closedir(dir);

    // Print the total count of files
    printf("%s ---- %d Orders\n", args[1], file_count);

    return 0;
}