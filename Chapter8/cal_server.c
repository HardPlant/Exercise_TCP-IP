#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "../err.h"

#define PORT 3600

struct cal_data
{
    int left_num;
    int right_num;
    char op;
    int result;
    short int error;
};

int main(int argc, char *argv[])
{
    struct sockaddr_in client_addr, sock_addr;
    int listen_sockfd, client_sockfd;
    int addr_len;
    struct cal_data rdata;
    int left_num, right_num;
    short int cal_result;

    listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sockfd == -1) perr(err_one);

    if(listen(listen_sockfd,5) == -1) perr(err_one);


    addr_len = sizeof(client_addr);
    client_sockfd = accept(listen_sockfd,
        (struct sockaddr*)&client_addr, &addr_len);
    if(client_sockfd == -1) perr(err_one);

    read(client_sockfd, (void*)&rdata, sizeof(rdata));

    rdata.error=0;

    left_num = ntohs(rdata.left_num);
    right_num = ntohs(rdata.right_num);

    if (rdata.op == '+')
        cal_result = left_num + right_num;
    else if (rdata.op == '-')
        cal_result = left_num - right_num;
    else if (rdata.op == '*')
        cal_result = left_num * right_num;
    else if (rdata.op == '/')
    {
        if(right_num != 0)
        {
            cal_result = left_num / right_num;
        }
        else
        {
            rdata.error=err_badOperand;
        }
    }
    else
    {
        rdata.error=err_badOp;
    }

    rdata.result = htonl(cal_result);
    rdata.error = htons(cal_result);
    write(client_sockfd, (void*)&rdata, sizeof(rdata));
    close(client_sockfd);
    
    close(listen_sockfd);
    return 0;
} 
