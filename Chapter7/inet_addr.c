#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../err.h"

#define maxline 256
int main(int argc, char *argv[])
{
    char buf[maxline];
    struct in_addr laddr;
    unsigned int ipaddr = 0;
    
    if(argc !=2) err("Usage : [internet address]", err_one);

    ipaddr = inet_addr(argv[1]);
    printf("inet_addr() : %s -> %d\n", argv[1], ipaddr);
    printf("inet_nota() : %d -> %s\n", ipaddr, inet_ntoa(*(struct in_addr*)&ipaddr));

    if(inet_aton(argv[1],&laddr) != -1)
    {
        printf("inet_aton() : %s -> %d\n", argv[1], laddr.s_addr);
        printf("inet_ntoa() : %d -> %s\n", laddr.s_addr,
                                    inet_ntoa(*(struct in_addr*)&laddr.s_addr));
    }
    else
    {
        perror("Error ");
    }
    return 1;
}