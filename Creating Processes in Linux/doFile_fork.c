#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// Constants defining maximum limits for lines, line length, words per line, and word length.
#define MAX_LINES 100
#define MAX_LINE_LENGTH 1000
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 50

//Splits a line into words based on whitespace delimiters.
int split_line(char *line, char *words[]) {
    int word_count = 0;
    char *token;
    
    // Tokenize the line using space, tab, and newline as delimiters.
    token = strtok(line, " \t\n");
    
    while (token != NULL && word_count < MAX_WORDS) {
        words[word_count] = strdup(token); // Duplicate the token and store it.
        word_count++;
        token = strtok(NULL, " \t\n");
    }

    return word_count;
}

int string_len(const char* str) {
    if (!str)
        return 0;  // Return 0 if the string pointer is NULL

    unsigned int counter = 0;
    while (str[counter] != '\0') {
        counter++;
    }

    return counter;
}

int compare_string(char* str1, char* str2) {
    unsigned int len1, len2;
    len1 = string_len(str1);
    len2 = string_len(str2);

    if(len1 != len2) return 0;

    for(int i=0; i<len1; i++) {
        if(str1[i] != str2[i]) return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    char *lines[MAX_LINES]; // Array to store lines read from the file.
    int line_count = 0;     // Counter for the number of lines read.
    
    char buffer[MAX_LINE_LENGTH]; // Buffer to store characters read from the file.
    int buf_index = 0;            // Index for the buffer.
    
    // Open the file for reading using the open() system call.
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    char ch;
    ssize_t n;
    // Read the file character by character.
    while ((n = read(fd, &ch, 1)) && line_count < MAX_LINES) {
        if (ch == '\n') {
            buffer[buf_index] = '\0'; // Null-terminate the buffer to form a complete line.
            lines[line_count] = strdup(buffer); // Duplicate the line and store it.
            if (lines[line_count] == NULL) {
                perror("Memory allocation failed");
                break;
            }
            line_count++;
            buf_index = 0; // Reset buffer index for the next line.
        } else {
            // Ensure buffer does not overflow.
            if (buf_index < MAX_LINE_LENGTH - 1) {
                buffer[buf_index++] = ch;
            }
        }
    }

    // Handle the last line if it doesn't end with a newline character.
    if (buf_index > 0 && line_count < MAX_LINES) {
        buffer[buf_index] = '\0';
        lines[line_count] = strdup(buffer);
        if (lines[line_count] == NULL) {
            perror("Memory allocation failed");
        } else {
            line_count++;
        }
    }
    
    close(fd); // Close the file descriptor.

    char *parsed_lines[MAX_LINES][MAX_WORDS]; // Array to store words parsed from each line.

    // Process each line to extract words.
    for (int i = 0; i < line_count; i++) {
        char *words[MAX_WORDS];
        int word_count = split_line(lines[i], words);

        for (int j = 0; j < word_count; j++) {
            parsed_lines[i][j] = words[j];
        }
    }

    unsigned int condition = 0;
    for(int i=0; i<line_count; i++) {

        if(compare_string(parsed_lines[i][1], "file")) condition = 1;
        else if(compare_string(parsed_lines[i][1], "dir")) condition = 2;
        else if(compare_string(parsed_lines[i][1], "copy")) condition = 3;

        pid_t pid = fork();

        if (pid == 0) {

            // Handle each case using a child process
            if(condition == 1){
                char *args[3];
                args[0] = "touch";
                args[1] = parsed_lines[i][2]; // File name argument
                args[2] = NULL;
                execvp("touch", args);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
            if(condition == 2){
                char *args[3];
                args[0] = "mkdir";
                args[1] = parsed_lines[i][2]; // Folder name argument
                args[2] = NULL;
                execvp("mkdir", args);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
            if(condition == 3){
                char *args[4];
                args[0] = "cp";
                args[1] = argv[1];
                args[2] = parsed_lines[i][2]; // New File name argument
                args[3] = NULL;
                execvp("cp", args);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
