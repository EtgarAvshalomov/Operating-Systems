#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>

int main() {
	TCHAR ProcessName[256];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	printf("Father Of Fibbo Son - Start\n");

	char Pr_Data[256] = "C:\\Fibbo_Son.exe ";
	char in_data[10] = { 0 };
	scanf_s("%s", in_data, 10);
	strcat_s(Pr_Data, 256, in_data);

	int i = 0;
	while (ProcessName[i] = (TCHAR)Pr_Data[i]) i++;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if (!CreateProcess(NULL,   // No module name (use command line).
		ProcessName,     // Command line.
		NULL,                  // Process handle not inheritable.
		NULL,                  // Thread handle not inheritable.
		FALSE,                 // Set handle inheritance to FALSE.
		0,                         // No creation flags.
		NULL,                  // Use parent's environment block.
		NULL,                  // Use parent's starting directory.
		&si,                     // Pointer to STARTUPINFO structure.
		&pi)                    // Pointer to PROCESS_INFORMATION structure.
		) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		return -1;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);
	printf("Father Of Fibbo Son - BACK\n");

	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	printf("Father Of Fibbo Son - END\n");
}
