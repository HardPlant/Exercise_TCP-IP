#include <winsock2.h>
#include <stdio.h>

#define MAX_PACKETLEN 1024

int main()
{
    WSADATA wsaData;
    SOCKET listen_s, client_s;
    struct sockaddr_in server_addr, client_addr;
    char szReceiveBuffer[MAX_PACKETLEN];
    int readn, writen;
    int nPort = 3500;
	HANDLE hChatServer;
    if(WSAStartup(MAKEWORD(2,2),&wsaData) !=0) return 1;

    listen_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_s == INVALID_SOCKET) return 1;

    ZeroMemory(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(nPort);
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    if(SOCKET_ERROR
        == bind(listen_s, (struct sockaddr*)&server_addr,
                sizeof(struct sockaddr_in)))return 0;
    
    if(SOCKET_ERROR == listen(server_addr, 5)) return 0;

    ZeroMemory(&client_addr, sizeof(struct sockaddr_in));

    int nAcceptClientInfo = sizeof(struct sockaddr_in);
    client_s = accept(server_addr
        ,(struct sockaddr*) &client_addr,&nAcceptClientInfo);
    int nReceiveBytes = 0;

    while(1)
    {
        readn = recv(hClient, szReceiveBuffer, MAX_PACKETLEN, 0);
        if (nReceiveBytes > 0)
        {
            writen = send(hClient, szReceiveBuffer, nReceiveBytes, 0);
        }
        closesocket(client_s);
    }

    closesocket(server_s);
    WSACleanup();

}