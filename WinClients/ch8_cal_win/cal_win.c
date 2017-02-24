#define _WINSOCK_DEPRECATED_NO_WARNINGS // winsock deprecated API »ç¿ë
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include "../../err.h"

#define PORT 3500
#define IP "192.168.59.128"
struct cal_data
{
	int left_num;
	int right_num;
	char op;
	int result;
	short int error;
};

int main(int argc, char **argv)
{
	WSADATA WSAData;
	SOCKADDR_IN addr;
	SOCKET s;
	int len;
	int sbyte, rbyte;
	struct cal_data sdata;
	if (argc != 4) err("[num1] [num2] [op]\n", err_one);
	ZeroMemory((void*)&sdata, sizeof(sdata));
	sdata.left_num = atoi(argv[1]);
	sdata.right_num = atoi(argv[2]);
	sdata.op = argv[3][0];

	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0) return err_one;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) return err_one;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(IP);

	if (SOCKET_ERROR ==
		connect(s, (struct sockaddr*)&addr, sizeof(addr)))
	{
		printf("%d, fail to connect\n", WSAGetLastError());
		closesocket(s);
		return 1;
	}

	len = sizeof(sdata);
	sdata.left_num = htonl(sdata.left_num);
	sdata.right_num = htonl(sdata.right_num);
	
	sbyte = send(s, (void*)&sdata, len, 0);
	if (sbyte != len) return err_one;
	
	rbyte = recv(s, (void*)&sdata, len, 0);
	if (rbyte != len) return err_one;

	if (ntohs(sdata.error != 0)) printf("CALC ERR %d\n", ntohs(sdata.error));
	printf("%d %c %d = %d\n", ntohl(sdata.left_num), sdata.op
		, ntohl(sdata.right_num), ntohl(sdata.result));

	closesocket(s);
	WSACleanup();
	return 0;
}