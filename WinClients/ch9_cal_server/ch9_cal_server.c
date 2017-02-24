#define _WINSOCK_DEPRECATED_NO_WARNINGS // winsock deprecated API »ç¿ë
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include "../../err.h"

#define PORT 3800
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
	SOCKET sockfd;
	int addrlen;
	int cal_result;
	int left_num, right_num;
	struct sockaddr_in addr, cliaddr;
	struct cal_data rdata;

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) perr(err_one);

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (-1 == sockfd) perr(err_one);

	memset((void*)&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	addrlen = sizeof(addr);
	if (-1 == bind(sockfd, (struct sockaddr*)&addr, addrlen))
		perr(err_one);

	while (1)
	{
		addrlen = sizeof(cliaddr);
		recvfrom(sockfd, (void*)&rdata, sizeof(cliaddr), 0,
			(struct sockaddr*)&cliaddr, &addrlen);
#if DEBUG
		printf("Client Info : %s(%d)\n".
			inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
		printf("%d %c %d\n", ntohl(rdata.left_num), rdata.op,
			ntohl(rdata.right_num));
#endif
		left_num = ntohl(rdata.left_num);
		right_num = ntohl(rdata.right_num);

		if (rdata.op == '+') cal_result = left_num + right_num;
		if (rdata.op == '-') cal_result = left_num - right_num;
		if (rdata.op == '*') cal_result = left_num * right_num;
		if (rdata.op == '/')
		{
			if (right_num != 0)
				cal_result = left_num / right_num;
			else
				rdata.error = htons(err_badOperand);
		}
		rdata.result = htonl(cal_result);
		sendto(sockfd, (void*)&rdata, sizeof(rdata), 0,
			(struct sockaddr*)&cliaddr, addrlen);
	}
	closesocket(sockfd);
	WSACleanup();
	return 0;
}