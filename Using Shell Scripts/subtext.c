#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// Main function that constructs and executes a shell command to search for a word in a file.
int main(int argc, char* argv[]) {
    // Check for correct number of arguments.
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <word> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Arguments for the execvp function to execute a shell command.
    char *args[] = {
        "sh", "-c",
        "count=$(grep -w \"$1\" \"$2\" | wc -l); "
        "if [ $count -eq 1 ]; then "
            "echo \"$2 contains the word '$1'\"; "
        "elif [ $count -eq 0 ]; then "
            "echo 'No matches found!'; "
        "fi",
        "sh",
        argv[1],
        argv[2],
        NULL
    };

    // Execute the shell command.
    execvp("sh", args);

    // If execvp returns, an error occurred.
    perror("execvp failed");
    exit(EXIT_FAILURE);
}
