#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 256

int main(VOID)
{
    printf("Pipe Father - Start\n");

    TCHAR ProcessName[256];
    HANDLE ParentReadHandle, ParentWriteHandle; // Parent to child
    HANDLE ChildReadHandle, ChildWriteHandle;   // Child to parent
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD written;

    printf("Pipe Father - Please Enter a string: No more that 256 characters: ");

    char message[BUFFER_SIZE] = "";
    char in_data[256] = { 0 };
    fgets(in_data, 256, stdin);

    size_t len = strlen(in_data); // Removes the unnecessary \n from the message
    if (len > 0 && in_data[len - 1] == '\n') {
        in_data[len - 1] = '\0';
    }

    strcat_s(message, 256, in_data);

    /* set up security attributes so that pipe handles are inherited */
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    /* allocate memory */
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));

    printf("Pipe Father - Pipes are ready! Creating pipe child in 3... 2... 1...\n");

    /* create the parent -> child pipe */
    if (!CreatePipe(&ParentReadHandle, &ParentWriteHandle, &sa, 0)) {
        fprintf(stderr, "Create Parent-to-Child Pipe Failed\n");
        return 1;
    }

    if (!CreatePipe(&ChildReadHandle, &ChildWriteHandle, &sa, 0)) {
        fprintf(stderr, "Create Child-to-Parent Pipe Failed\n");
        CloseHandle(ParentReadHandle);
        CloseHandle(ParentWriteHandle);
        return 1;
    }

    /* establish the START_INFO structure for the child process */
    GetStartupInfo(&si);
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); // ******************* Shkufit 1 *******************

    /* redirect the standard input to the read end of the pipe */
    si.hStdInput = ParentReadHandle;
    si.dwFlags = STARTF_USESTDHANDLES;

    /* we do not want the child to inherit the write end of the pipe */
    SetHandleInformation(ParentWriteHandle, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(ChildReadHandle, HANDLE_FLAG_INHERIT, 0);

    TCHAR CommandLine[512];
    swprintf(CommandLine, 512, L"C:\\Pipe_Son.exe %d", (int)ChildWriteHandle);

    /* create the child process */
    if (!CreateProcess(NULL,
        CommandLine,
        NULL,
        NULL,
        TRUE, /* inherit handles */
        0,
        NULL,
        NULL,
        &si,
        &pi))
    {
        fprintf(stderr, "Process Creation Failed\n");
        CloseHandle(ParentReadHandle);
        CloseHandle(ParentWriteHandle);
        CloseHandle(ChildReadHandle);
        CloseHandle(ChildWriteHandle);
        return -1;
    }

    /* close the unused ends of the pipes */
    CloseHandle(ParentReadHandle);  // Child uses this
    CloseHandle(ChildWriteHandle);  // Child uses this

    printf("Pipe Father - Sending a message to the child\n");
    /* the parent now wants to write to the pipe */
    if (!WriteFile(ParentWriteHandle, message, BUFFER_SIZE, &written, NULL))
        fprintf(stderr, "Error writing to pipe\n");

    /* close the write end of the pipe */
    CloseHandle(ParentWriteHandle);

    printf("Pipe Father - Waiting for child\n");
    /* wait for the child to exit */
    WaitForSingleObject(pi.hProcess, INFINITE);

    /* have the father read from the pipe */
    CHAR buffer[BUFFER_SIZE];
    DWORD read;
    if (ReadFile(ChildReadHandle, buffer, BUFFER_SIZE, &read, NULL))
        printf("Pipe Father - Message Got Back: %s\n", buffer);
    else
        fprintf(stderr, "Father: Error reading from pipe\n");

    printf("Pipe Father - Closing Pipe\n");
    /* close all handles */
    CloseHandle(ChildReadHandle);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread); // ******************* Shkufit 3 *******************
    printf("Pipe Father - End\n");

    return 0;
}

