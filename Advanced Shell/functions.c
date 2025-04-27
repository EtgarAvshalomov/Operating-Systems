#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "functions.h"

// Helping function
int split_line(char *line, char *words[], int maxWords) {
    int word_count = 0;
    char *token;
    
    // Tokenize the line using space, tab, and newline as delimiters.
    token = strtok(line, " \t\n");
    
    while (token != NULL && word_count < maxWords) {
        words[word_count] = strdup(token); // Duplicate the token and store it.
        word_count++;
        token = strtok(NULL, " \t\n");
    }

    return word_count;
}

// Program functions
int startShell() {

    int status;

    __pid_t id = fork();

    if (id == -1){
        printf("Failed to create a new process in startShell");
        return 0;
    }

    if (id == 0) {
        char* args[4];
        args[0] = "sh";
        args[1] = "-c";
        args[2] = "mkdir Black_Friday && touch Black_Friday/camp_partic.txt";
        args[3] = NULL;

        execvp("sh", args);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    return 1;
}

int deleteFolder() {
    int status;
    __pid_t id = fork();

    if (id == -1){
        printf("Failed to create a new process in startShell\n");
        return 0;
    }

    if (id == 0) {
        char* args[4];
        args[0] = "sh";
        args[1] = "-c";
        args[2] = "rm -rf Black_Friday";
        args[3] = NULL;

        execvp("sh", args);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);
}

// Commands
int GetFlyer(char *companyName) {
    int status;
    pid_t id = fork();

    if (id == 0) {
        char* args[5];
        args[0] = "sh";
        args[1] = "-c";
        args[2] = "./getFlyer \"$0\"";
        args[3] = companyName;
        args[4] = NULL;

        execvp("sh", args);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    return 1;
}

int CreateFlyer(char* companyName, char* discount) {

    int status;
    pid_t id = fork();

    if (id == 0) {
        char* args[6];
        args[0] = "sh";
        args[1] = "-c";
        args[2] = "./createFlyer \"$0\" $1";
        args[3] = companyName;
        args[4] = discount;
        args[5] = NULL;

        execvp("sh", args);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    return 1;
}

int GetNumComp() {
    int status;
    pid_t id = fork();

    if (id == 0) {
        char* args[4];
        args[0] = "sh";
        args[1] = "-c";
        args[2] = "./getNumComp";
        args[3] = NULL;

        execvp("sh", args);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    return 1;
}

int MakeOrder(char *companyName, char *customerName) {
    int status;
    pid_t id = fork();

    if (id == 0) {
        char* args[6];
        args[0] = "sh";
        args[1] = "-c";
        args[2] = "./makeOrder \"$0\" $1";
        args[3] = companyName;
        args[4] = customerName;
        args[5] = NULL;

        execvp("sh", args);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    return 1;
}

int GetOrderNum(char *companyName) {
    int status;
    pid_t id = fork();

    if (id == 0) {
        char* args[5];
        args[0] = "sh";
        args[1] = "-c";
        args[2] = "./getOrderNum \"$0\"";
        args[3] = companyName;
        args[4] = NULL;

        execvp("sh", args);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    return 1;
}

int GetSummary() {
    int status;
    pid_t id = fork();

    if (id == 0) {
        char* args[4];
        args[0] = "sh";
        args[1] = "-c";
        args[2] = "./getSummary";
        args[3] = NULL;

        execvp("sh", args);

        // If execvp returns, an error occurred.
        perror("ERROR");
        return(0);
    }

    waitpid(id, &status, 0);

    return 1;
}