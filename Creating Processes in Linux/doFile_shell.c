#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>    // For open()
#include <unistd.h>   // For read() and close()

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

//Main function: Reads a file, processes its content, and executes shell commands based on the content.
int main(int argc, char* argv[]) {
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
    while ((n = read(fd, &ch, 1)) == 1 && line_count < MAX_LINES) {
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

    // Arguments for the shell command to be executed.
    char *args[] = {
        "sh", "-c",
        "state=0; "
        "first=0; "
        "for word in \"$@\"; do "
            "if [ \"$first\" = \"0\" ]; then "
            "  first=1; "
            "  continue; "
            "fi; "
            "if [ \"$state\" = \"1\" ]; then "
            "  touch \"$word\"; "
            "  state=0; "
            "fi; "
            "if [ \"$state\" = \"2\" ]; then "
            "  mkdir -p \"$word\"; "
            "  state=0; "
            "fi; "
            "if [ \"$state\" = \"3\" ]; then "
            "  cp \"$1\" \"$word\"; "
            "  state=0; "
            "fi; "
            "if [ \"$word\" = \"file\" ]; then "
            "  state=1; "
            "  continue; "
            "elif [ \"$word\" = \"dir\" ]; then "
            "  state=2; "
            "  continue; "
            "elif [ \"$word\" = \"copy\" ]; then "
            "  state=3; "
            "  continue; "
            "fi; "
        "done",
        "sh",
        argv[1],
        NULL
    };

    unsigned int args_index = 5;
    // Populate the arguments array with parsed words.
    for (int i = 0; i < line_count; i++) {
        for (int j = 1; j < 3; j++) {
            args[args_index] = parsed_lines[i][j];
            args_index++;
        }
    }

    execvp("sh", args); // Execute the shell command.
    perror("execvp failed"); // If execvp returns, an error occurred.
    exit(EXIT_FAILURE);

    return 0;
}