#include <winsock2.h>
#include <stdio.h>

#define PORT 3500
#define IP "127.0.0.1"

int main()
{
    WSADATA WSAData;
    SOCKADDR_IN addr;
    SOCKET s;
    char buffer[1024];
    int readbytes;
    int i, len;

    if(WSAStartup(MAKEWORD(2,0), &WSAData) != 0) return 1;

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (s == INVALID_SOCKET) return 1;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.S_un.S_addr = inet_addr(IP);

    while(1) {}

    return 0;
}