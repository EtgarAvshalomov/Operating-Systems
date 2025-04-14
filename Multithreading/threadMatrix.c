#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N 10000

// Initializing variables
int arr[N];

int numToSearch;
int foundIndex;
pthread_t threads[2];

// finds the number in the threads side of the array
void* find(void* sideChoice) {
    char* side = (char *)sideChoice;
    // Adding a sleep of 1us (Stabilizes the threads timing)
    usleep(1);
    if (strcmp(side, "left") == 0) {
        int i;
        for(i=0; i<N/2; i++) {
            if(arr[i] == numToSearch) {
                pthread_cancel(threads[1]);
                foundIndex = i;
                // Check if the thread was cancelled
                pthread_testcancel();
                printf("\nThread 0: Number found in index: %d\n", foundIndex);
                printf("Thread 0: I killed thread 1... I WIN!\n");
                break;
            }
        }
    } else if (strcmp(side, "right") == 0) {
        int i;
        for (i=N/2; i<N; i++) {
            if(arr[i] == numToSearch) {
                pthread_cancel(threads[0]);
                foundIndex = i;
                // Check if the thread was cancelled
                pthread_testcancel();
                printf("\nThread 1: Number found in index: %d\n", foundIndex);
                printf("Thread 1: I killed thread 0... I WIN!\n");
                break;
            }
        }
    } else {
        printf("Enter either: 'left' or 'right'\n");
    }
}

void main() {

    // Randomizing the array
    srand(time(NULL));

    int i;
    for(i = 0; i < N; i++) {
        arr[i] = rand() % 101;
        printf("%d ", arr[i]);
    }
    
    // Getting input from the user
    printf("\n\nEnter a number to search (0-100): ");
    scanf("%d", &numToSearch);

    // Creating the threads
    pthread_create(&threads[0],NULL,find,"left");
    pthread_create(&threads[1],NULL,find,"right");

    // Waiting for the threads to end
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    if(!foundIndex) printf("Number not found. Enter a number between 0-100\n");
}