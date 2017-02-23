#define _WINSOCK_DEPRECATED_NO_WARNINGS // winsock deprecated API »ç¿ë
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include "../../err.h"
int main(int argc, char **argv)
{
	WSADATA wsaData;
	int iResult;
	struct hostent *myent;

	if (argc != 2) err("[domain name]\n", err_one);

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) err("WSAStartup Error\n", err_one);

	myent = gethostbyname(argv[1]);

	if (myent == NULL) err("Not Found Domain Name\n", err_one);

	printf("Host Name : %s\n", myent->h_name);

	while (*myent->h_addr_list != NULL)
	{
		printf("%s\n",
			inet_ntoa(*(struct in_addr*)*myent->h_addr_list));
		myent->h_addr_list++;
	}

	WSACleanup();

	return 0;
}