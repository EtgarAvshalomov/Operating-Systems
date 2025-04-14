#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {

    // Check for sufficient arguments
    if (argc < 1) {
        printf("Not enough arguments.\n");
        printf("Expected arguments: 1\n");
        exit(1);
    }

    char *my_str = argv[1];
    unsigned int my_str_length = strlen(my_str);

    // Initializing the matrix
    char words[256][256];

    // Splitting the string by the '@' sign
    int i;
    unsigned int word_counter = 0;
    int index = 0;
    for (i=0; i<my_str_length; i++){
        if (my_str[i] == '@') {
            word_counter++;
            index = 0;
            continue;
        }
        words[word_counter][index] = my_str[i];
        index++;
    }
    
    // Printing the words
    for (i=0; i<word_counter+1; i++) {
        printf("%s\n", words[i]);
    }

    return 0;
}