#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

int main(int argc, char* argv[]){

	// Get the length of the first argument (the word to check)
	unsigned int word_length = strlen(argv[1]);

	// Check if there are enough characters provided after the word
	if (argc == 2 || word_length > argc-2) {
		printf("Not enough arguments for the word '%s'. Recieved: %d\n", argv[1], argc-2);
        printf("Expected arguments: at least %d\n", word_length);
		printf("%s not exists\n", argv[1]);
		exit(1);
	}

	// Create a boolean array to track which letters are found
	bool *checkArr = (bool *)calloc(strlen(argv[1]), sizeof(bool));
	
	int i;
	char *word = argv[1];
	// Go through each character argument
	for (i=2; i<argc; i++) {
		int j;
		// Check if this character matches a letter in the word
		for (j=0; j<word_length; j++) {
			if(word[j] == argv[i][0] && checkArr[j] == false) {
				checkArr[j] = true;
				break;
			}
		}
	}

	// Check if all letters in the word were found
	for (i=0; i<word_length; i++) {
		if (checkArr[i] == false) {
			printf("%s not exists\n", argv[1]);
			free(checkArr);
			exit(0);
		}
	}

	// All letters were found
	printf("%s exists\n", argv[1]);
	free(checkArr);
	exit(0);

	return 0;
}
