#include <stdio.h>
#include <windows.h>

#define MAXLINE 1024
int main(int argc, char* argv[])
{
	HANDLE readPipe = (HANDLE)(atoi(argv[1]));
	int readn;
	char buf[MAXLINE];

	while (1)
	{
		memset(buf, 0x00, MAXLINE);

		ReadFile(readPipe, buf, MAXLINE, &readn, NULL);
		printf(" --> %s", buf);
	}
}