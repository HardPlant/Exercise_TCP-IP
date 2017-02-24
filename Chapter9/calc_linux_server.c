#include <sys/types.h> // for data types(linux-dependency)
#include <sys/stat.h> // for file informations
#include <sys/socket.h> //for Internet data
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h> // for TCPIPUDP macros
#include <arpa/inet.h> // for in_addr, h<->n bytes, internet operations

#include "../err.h"
#define PORT_NUM 3800
#define MAXLEN 256

struct cal_data{
    int left_num;
    int right_num;
    char op;
    int result;
    short int error;
};

int main(int argc, char *argv[])
{
    int sockfd;
    socketlen_t addrlen;
    int cal_result;
    int left_num, right_num;
    struct sockaddr_in addr, cliaddr;
    struct cal_data rdata;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(-1 == sockfd) perr(err_one);

    memset((void*)&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT_NUM);

    addrlen = sizeof(addr);
    if(-1 ==
        bind(sockfd, (struct sockaddr*)&addr, addrlen))
        perr(err_one);

    while(1)
    {
        addrlen = sizof(cliaddr);
        recvfrom(sockfd, (void*)&rdata, sizeof(rdata), NULL,
            (struct sockaddr *)&cliaddr, &addrlen);
    #if DEBUG
        printf("Client info : %s(%d)\n", inet_ntoa(cliaddr.sin_addr),
            ntohs(cliaddr.sin_port));
        printf("%d %d %d\n", ntohl(rdata.left_num), rdata.op,
            ntohl(rdata.right_num));
    #endif

        left_num = ntohl(rdata.left_num);
        right_num = ntohl(rdata.right_num);
        if(rdata.op == '+') cal_result = left_num + right_num;
        if(rdata.op == '-') cal_result = left_num - right_num;
        if(rdata.op == '*') cal_result = left_num * right_num;
        if(rdata.op == '/')
        {
            if (right_num != 0)
                cal_result = left_num / right_num;
            else
                rdata.error=htons(err_badOperand);
        }
        else
            rdata.error=htons(err_badOp);
    
    rdata.result=htonl(cal_result);
    sendto(sockfd,(void*)&rdata,sizeof(rdata,NULL,
        (struct sockaddr*)&cliaddr, &addrlen));
    }
    return 1;
}


