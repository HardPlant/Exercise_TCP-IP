#include <stdio.h>
#include <windows.h>

#define MAXLINE 1024

int main(int argc, char *argv[])
{
	HANDLE readPipe, writePipe;
	SECURITY_ATTRIBUTES sec;
	int written;
	int rtv;
	TCHAR command[240] = { 0, };
	char buf[MAXLINE];
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;

	sec.nLength = sizeof(SECURITY_ATTRIBUTES); // for CreateProcess()
	sec.bInheritHandle = TRUE; // inherits to child process
	sec.lpSecurityDescriptor = NULL;

	CreatePipe(&readPipe, &writePipe, &sec, 0); // BOOL CreatePipe(PHANDLE hReadPipe, hWritePipe, LPSECURITY_ATTR, nSize), use defaultSize if nSize = 0
	// Create read/write pipe
	snprintf((char*)command, sizeof(command),"C:\\ch11_winpipe_cli.exe %d", (int)readPipe);
	printf("%s\n", command);
	si.cb = sizeof(si);
	rtv = CreateProcess(NULL, command, // execute echo_anonypipe_cli
		NULL, NULL,
		TRUE, CREATE_NEW_CONSOLE, // for new console
		NULL, NULL,
		&si, &pi);
	if (!rtv)
	{
		printf("Error %d\n", GetLastError());
		return 1;
	}
	while (1)
	{
		printf("> ");
		fgets(buf, MAXLINE, stdin);
		if (strncmp(buf, "quit\n", 5) == 0) break;
		WriteFile(writePipe, buf, strlen(buf), &written, NULL);
	}
	CloseHandle(readPipe);
	CloseHandle(writePipe);
}