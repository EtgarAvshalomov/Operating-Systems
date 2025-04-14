#include <stdio.h>
#include <windows.h>
#include <string.h>
#define BUFFER_SIZE 256
int main(int argc, char* argv[])
{
    printf("Pipe Child - Start\n");
    HANDLE ParentReadHandle, ChildWriteHandle;
    CHAR buffer[BUFFER_SIZE] = { 0 };
    DWORD read;
    DWORD written;

    // Get the handle for reading from parent
    ParentReadHandle = GetStdHandle(STD_INPUT_HANDLE);

    if (argc > 1) {
        ChildWriteHandle = (HANDLE)atoi(argv[1]);
    }
    else {
        fprintf(stderr, "Pipe Child: No write handle provided\n");
        return 1;
    }

    /* have the child read from the pipe */
    if (ReadFile(ParentReadHandle, buffer, BUFFER_SIZE, &read, NULL))
        printf("Pipe Child: Received Message: %s\n", buffer);
    else
        fprintf(stderr, "Pipe Child: Error reading from pipe\n");

    // Optional: Process the message - uncomment and fix this as needed
    unsigned int length = strlen(buffer);
    for (int i = 0; i < length; i++) {
        if (buffer[i] == ',' || buffer[i] == ' ') buffer[i] = ';';
        else if (buffer[i] >= 'A' && buffer[i] <= 'Z') buffer[i] += 32;
        else if (buffer[i] >= 'a' && buffer[i] <= 'z') buffer[i] -= 32;
    }

    printf("Pipe Child - Sending message to father\n");

    /* write back to parent through the WriteHandle (STD_OUTPUT_HANDLE) */
    if (!WriteFile(ChildWriteHandle, buffer, BUFFER_SIZE, &written, NULL))
        fprintf(stderr, "Pipe Child: Error writing to pipe\n");

    printf("Pipe Child - End\n");
    return 0;
}