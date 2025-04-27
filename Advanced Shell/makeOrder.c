#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <sys/wait.h>
#include "functions.h"

#define BUFFER_SIZE 1024
#define MAX_CHARS 256
#define MAX_ITEMS 256
#define MAX_WORDS 30
#define MAX_FLYER_WORDS 100
#define DOTS_NUM 10

int main(int argc, char *args[]) {

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

    close(fd);

    if(!seen) {
        printf("Company %s doesn't exist\n", args[1]);
        return 1;
    }

    // Reading the contents of the flyer
    char filename[MAX_CHARS];
    strcpy(filename, "Black_Friday/");
    strcat(filename, args[1]);
    strcat(filename, ".txt");

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("Flyer with name %s doesn't exist\n", args[1]);
        return 0;
    }

    strcpy(buffer, "");
    bytesRead = read(fd, buffer, BUFFER_SIZE);
    if (bytesRead < 0) {
        perror("Failed to read file");
        close(fd);
        return 0;
    }

    buffer[bytesRead] = '\0'; // Null-terminate

    close(fd);

    GetFlyer(args[1]);

    int status;
    char *words[MAX_WORDS];
    int wordsNum;
    char validInputs[MAX_ITEMS][MAX_WORDS][MAX_CHARS];
    int validWordsNum[MAX_ITEMS] = {0};
    int validInputsCount = 0;
    char input[MAX_CHARS];

    // User inserts item name and quantity
    while(1) {
        printf("Insert item name and quantity or 'stop': ");
        fgets(input, MAX_CHARS, stdin);

        if(!strcmp(input, "stop\n")) break;

        // checks words and numbers
        wordsNum = split_line(input, words, MAX_WORDS);
        int i = 0;
        char c;
        int validArgs = 1;

        if(wordsNum < 2) {
            printf("Invalid arguments!\n");
            printf("Expected arguments: [item name] [quantity]\n");
            continue;
        }

        while(1){
            c = words[wordsNum-1][i++];
            if(!c) break;
            
            // Check if the last argument is a float
            if(!(c >= '0' && c <= '9')) {
                printf("Invalid argument for quantity in MakeOrder\n");
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

    // User confirms or cancels the order
    while (1) {
        printf("To confirm you order, insert 'confirm' or 'cancel': ");
        fgets(input, MAX_CHARS, stdin);
        if (!strcmp(input, "confirm\n")) break;
        if (!strcmp(input, "cancel\n")) {
            printf("Order canceled\n");
            return 1;
        }
        printf("Invalid input!\n");
    }

    if(validInputsCount == 0) {
        printf("No items ordered, aborting mission");
        return 0;
    }

    char text[MAX_CHARS] = {""};
    int textLength = 0;

    strcpy(text, args[1]);
    strcat(text, " Order\n\n");

    float sum = 0;
    int discount;
    // Adding the items to the order and checks that the item exists
    for(int i=0; i<validInputsCount; i++) {

        int j;
        int iBuffer = 0;
        int iWord = 0;
        int seen;
        char c = buffer[iBuffer++];

        // Calculates the discount like so: discount%
        while(c != '\n') c = buffer[iBuffer++];

        c = buffer[iBuffer++];
        char discountString[MAX_CHARS];
        strcpy(discountString, "");
        char temp[2];
        while(c != '%') {
            temp[0] = c;
            temp[1] = '\0';
            strcat(discountString, temp);
            c = buffer[iBuffer++];
        }

        char *endptr;
        discount = atoi(discountString);

        // Checks that the item exists in the flyer
        for (j=0; j<validWordsNum[i]-1; j++) {
            seen = 0;
            while(c) {
                if (c == validInputs[i][j][iWord]) iWord++;
                else iWord = 0;
                
                if (validInputs[i][j][iWord] == '\0') {
                    seen = 1;
                    break;
                }
                c = buffer[iBuffer++];
            }
            if(!seen) {
                printf("The item %s does not exist in the flyer\n", validInputs[i][j]);
                return 0;
            }
        }

        // Calculates the sum of the price without the discount
        c = buffer[iBuffer++];
        while(c == '.') c = buffer[iBuffer++];
        char priceString[MAX_CHARS];
        strcpy(priceString, "");
        while(c != 'N') {
            temp[0] = c;
            temp[1] = '\0';
            strcat(priceString, temp);
            c = buffer[iBuffer++];
        }
        sum += strtof(priceString, &endptr) * atoi(validInputs[i][validWordsNum[i]-1]);
        
        // Concatinates the name of the item
        for (j=0; j<validWordsNum[i]-1; j++) {
            strcat(text, validInputs[i][j]);
            if(j != validWordsNum[i] -2) strcat(text, " ");
        }
        
        // Concatinates dots
        strcat(text, " - ");

        // Concatinates the quantity of the item and calculates the sum
        strcat(text, validInputs[i][j]);
        int quantity = atoi(validInputs[i][j]);
        strcat(text, "\n");
    }

    // Adds the discounted price to the order
    char catBuffer[MAX_CHARS];
    float discountedSum = sum * ((100.0 - (float)discount) / 100);
    sprintf(catBuffer, "\nTotal Price: %.2f NIS", discountedSum);
    strcat(text, catBuffer);

    // Adds the current date to the order
    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    sprintf(catBuffer, "\nDate: %d-%02d-%02d\n", 
           now->tm_year + 1900, 
           now->tm_mon + 1, 
           now->tm_mday);

    strcat(text, catBuffer);

    // Counts the length of the text
    int index = 0;
    c = text[index++];
    while(c) {
        textLength++;
        c = text[index++];
    }

    strcpy(filename, "Black_Friday/Order ");
    strcat(filename, args[1]);
    strcat(filename, "/");
    strcat(filename, args[2]);
    strcat(filename, ".txt");

    // Creating child process to insert the company name and discount
    __pid_t id = fork();

    if (id == -1){
        printf("Failed to create a new process in MakeOrder\n");
        return 0;
    }

    // Calling the shell with the correct arguments
    if (id == 0) {

        char* shellArgs[5];
        shellArgs[0] = "sh";
        shellArgs[1] = "-c";
        shellArgs[2] = "mkdir \"Black_Friday/Order $0\"";
        shellArgs[3] = args[1];
        shellArgs[4] = NULL;

        execvp("sh", shellArgs);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    // Open the file for writing
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
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

    id = fork();

    if (id == 0) {
        char* shellArgs[6];
        shellArgs[0] = "sh";
        shellArgs[1] = "-c";
        shellArgs[2] = "chmod 444 \"Black_Friday/Order $0/$1.txt\"";
        shellArgs[3] = args[1];
        shellArgs[4] = args[2];
        shellArgs[5] = NULL;

        execvp("sh", shellArgs);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    printf("Order Created\n");

    return 0;
}