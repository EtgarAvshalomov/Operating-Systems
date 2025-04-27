#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "functions.h"

#define BUFFER_SIZE 1024
#define MAX_CHARS 256
#define MAX_ITEMS 256

int main(int argc, char *args[]) {

    // Get the names of the participating companies
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

    char companies[MAX_ITEMS][MAX_CHARS];
    int compNum = 0;
    int charIndex = 0;
    int bIndex = 0;

    // Get the names of the companies from camp_partic.txt
    char c = buffer[bIndex++];
    char temp[2];
    while (c) {
        strcpy(companies[compNum], "");
        while(c != '\n') {
            temp[0] = c;
            temp[1] = '\0';
            strcat(companies[compNum], temp);
            c = buffer[bIndex++];
        }
        c = buffer[bIndex++];
        charIndex = 0;
        compNum++;
    }

    // Get the number of companies and their orders
    GetNumComp();
    for(int i=0; i<compNum; i++) {
        GetOrderNum(companies[i]);
    }

    return 0;
}