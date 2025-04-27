#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "functions.h"

#define MAX_LENGTH 256
#define MAX_WORDS 30
#define MAX_ARGUMENTS 31
#define MAX_CHARS 256

// Main function that constructs and executes a shell command to search for a word in a file.
int main() {

    // Call function to create Black_Friday dir with file camp_partic.txt
    if(!startShell()) exit(1);
    
    char buffer[MAX_LENGTH];
    while(1) {

        printf("AdvShell>");
        fgets(buffer, MAX_LENGTH, stdin);

        if(strlen(buffer) == 1) continue;

        char *words[MAX_WORDS];
        int word_count = split_line(buffer, words, MAX_WORDS);

        // Arguments for the execvp function to execute a shell command.
        char *args[MAX_ARGUMENTS] = {
            words[0],
        };     

        // --- Custom commands ---

        if (!strcmp(words[0], "exit")) {
            deleteFolder();
            printf("Goodbye\n");
            exit(0);
        }

        if (!strcmp(words[0], "CreateFlyer")) {
            // Check for enough arguments
            if(word_count < 3) {
                printf("Invalid arguments!\n");
                printf("Expected agruments: CreateFlyer [company name] [discount]\n");
                continue;
            }

            // Check for valid input
            int wIndex = 0;
            int err = 0;
            char c = words[word_count-1][wIndex++];
            while(c) {
                if (!(c  >= '0' && c <= '9')) {
                    err = 1;
                    break;
                }
                c = words[word_count-1][wIndex++];
            }
            if (err) {
                printf("Discount must be a positive integer\n");
                continue;
            }
            if (atoi(words[word_count-1]) <= 0 || atoi(words[word_count-1]) > 100) {
                printf("Discount must be 1-100%%\n");
                continue;
            }

            // Pass valid company name to the function
            char companyName[MAX_CHARS];
            strcpy(companyName, "");
            for(int i=0; i<word_count-2; i++) {
                strcat(companyName, words[1+i]);
                if(i != word_count-3) strcat(companyName, " ");
            }
            CreateFlyer(companyName, words[word_count-1]);
            continue;
        }

        if (!strcmp(words[0], "GetFlyer")) {
            char companyName[MAX_CHARS];
            strcpy(companyName, "");
            for(int i=0; i<word_count-1; i++) {
                strcat(companyName, words[1+i]);
                if(i != word_count-2) strcat(companyName, " ");
            }
            GetFlyer(companyName);
            continue;
        }

        if (!strcmp(words[0], "GetNumComp")) {
            if(word_count > 1) {
                printf("Too many arguments, expected 0\n");
                continue;
            }
            GetNumComp();
            continue;
        }

        if (!strcmp(words[0], "MakeOrder")) {
            if(word_count < 3) {
                printf("Invalid arguments!\n");
                printf("Expected agruments: MakeOrder [company name] [customer name]\n");
                continue;
            }
            char companyName[MAX_CHARS];
            strcpy(companyName, "");
            for(int i=0; i<word_count-2; i++) {
                strcat(companyName, words[1+i]);
                if(i != word_count-3) strcat(companyName, " ");
            }
            MakeOrder(companyName, words[word_count-1]);
            continue;
        }

        if (!strcmp(words[0], "GetOrderNum")) {
            if(word_count < 2) {
                printf("Invalid arguments!\n");
                printf("Expected arguments: GetOrderNum [company name]\n");
                continue;
            }
            char companyName[MAX_CHARS];
            strcpy(companyName, "");
            for(int i=0; i<word_count-1; i++) {
                strcat(companyName, words[1+i]);
                if(i != word_count-2) strcat(companyName, " ");
            }
            GetOrderNum(companyName);
            continue;
        }

        if (!strcmp(words[0], "GetSummary")) {
            if (word_count > 1) {
                printf("Too many arguments, expected 0\n");
                continue;
            }
            GetSummary();
            continue;
        }
        
        // --- Passing a command to the shell ---

        for(int i=1; i<word_count; i++) {
            args[i] = words[i];
        }
        args[word_count+1] = NULL;
        
        int id, wid, status;
        id = fork();

        if (id == 0) {
            // Execute the shell command.
            execvp(words[0], args);

            // If execvp returns, an error occurred.
            printf("Not Supported\n");
            exit(EXIT_FAILURE);
        }

        waitpid(id, &status, 0);
    }

    return 0;
}
