#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "functions.h"

#define BUFFER_SIZE 1024
#define MAX_CHARS 256
#define MAX_ITEMS 256
#define MAX_WORDS 30
#define DOTS_NUM 10

int main(int argc, char *args[]) {

    int status;
    char *words[MAX_WORDS];
    int wordsNum;
    char validInputs[MAX_ITEMS][MAX_WORDS][MAX_CHARS];
    int validWordsNum[MAX_ITEMS] = {0};
    int validInputsCount = 0;

    // User inserts item name and price
    while(1) {
        char input[MAX_CHARS];
        printf("Insert item name and item cost or 'stop': ");
        fgets(input, MAX_CHARS, stdin);

        if(!strcmp(input, "stop\n")) break;

        // checks words and numbers
        wordsNum = split_line(input, words, MAX_WORDS);
        int i = 0;
        char c;
        int validArgs = 1;

        if (wordsNum < 2) {
            printf("Invalid argument for item price in CreateFlyer\n");
            continue;
        }

        while(1){
            c = words[wordsNum-1][i++];
            if(!c) break;
            
            // Check if the last argument is a float
            if(!((c >= '0' && c <= '9') || c == '.')) {
                printf("Invalid argument for item price in CreateFlyer\n");
                validArgs = 0;
                break;
            }
        }

        if(!validArgs) continue;


        for (int i=0; i<wordsNum; i++) {
            strcpy(validInputs[validInputsCount][i], words[i]);
        }
        validWordsNum[validInputsCount++] = wordsNum;
        
    }

    if(validInputsCount == 0) {
        printf("No items added, aborting mission");
        return 0;
    }

    // User chooses a message for the flyer
    char message[MAX_CHARS];
    while(1) {
        printf("Choose a message:\n");
        printf("1. Enjoyed\n");
        printf("2. Shop Now and have fun\n");
        printf("3. Have Fun\n");
        printf("4. Other\n");
        char choice[MAX_CHARS];
        fgets(choice, MAX_CHARS, stdin);

        if(!strcmp(choice, "1\n")) {
            strcpy(message, "\nEnjoyed\n");
            break;
        }
        if(!strcmp(choice, "2\n")) {
            strcpy(message, "\nShop Now and have fun\n");
            break;
        }
        if(!strcmp(choice, "3\n")) {
            strcpy(message, "\nHave Fun\n");
            break;
        }
        if(!strcmp(choice, "4\n")) {
            printf("Enter a custom message: ");
            strcpy(message, "\n");
            char buffer[MAX_CHARS];
            fgets(buffer, MAX_CHARS, stdin);
            strcat(message, buffer);
            break;
        }
        printf("Invalid input!\n");
    }

    // Check if the name of the company exists in camp_partic and if it doesn't we add it
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

    if(seen == 0) {
        const char *filename = "Black_Friday/camp_partic.txt";
        // Open the file for writing
        int fd = open(filename, O_WRONLY | O_APPEND, 0644);
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }

        // Write to the file
        ssize_t bytes_written = write(fd, args[1], strlen(args[1]));
        if (bytes_written == -1) {
            perror("Error writing to file");
            close(fd);
            return 1;
        }

        // Write \n
        bytes_written = write(fd, "\n", 1);
        if (bytes_written == -1) {
            perror("Error writing to file");
            close(fd);
            return 1;
        }

        // Close the file
        close(fd);
    }
    else close(fd);

    char filename[MAX_CHARS];
    char text[MAX_CHARS] = {""};
    int textLength = 0;
    strcpy(filename, "Black_Friday/");
    strcat(filename, args[1]);
    strcat(filename, ".txt");

    strcat(text, args[1]);
    strcat(text, " Sale\n");
    strcat(text, args[2]);
    strcat(text, "% off\n\n");

    // Open the file for writing
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Adding the items to the flyer
    for(int i=0; i<validInputsCount; i++) {
        
        // Concatinates the name of the item
        int j;
        for (j=0; j<validWordsNum[i]-1; j++) {
            strcat(text, validInputs[i][j]);
            if(j != validWordsNum[i] -2) strcat(text, " ");
        }
        
        // Concatinates dots
        for(int z=0; z<DOTS_NUM; z++) strcat(text, ".");

        // Concatinates the price of the item
        strcat(text, validInputs[i][j]);
        strcat(text, "NIS\n");
    }

    // Concatinates the message
    strcat(text, message);

    // Counts the length of the text
    int index = 0;
    c = text[index++];
    while(c) {
        textLength++;
        c = text[index++];
    }

    // Write to the file
    ssize_t bytes_written = write(fd, text, textLength);
    if (bytes_written == -1) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }

    // Close the file
    close(fd);

    printf("Flyer Created\n");

    return 0;
}