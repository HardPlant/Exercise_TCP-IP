#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXBUF 1024 // same length with client
#include "../../err.h"
int main(int argc, char **argv)
{
    int listen_fd, client_fd;
    socklen_t addrlen;
    int readn;
    char buf[MAXBUF];
    struct sockaddr_un client_addr, server_addr; // in->un

    if(access(argv[1], F_OK) == 0) unlink(argv[1]); // check argv[1] exist

    listen_fd = socket(AF_UNIX, SOCK_STREAM, 0); // INET->UNIX
    if(-1 == listen_fd) perr(err_failed);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, argv[1],strlen(argv[1]));

    if (err_failed == bind(listen_fd, (struct sockaddr*) &server_addr
            ,sizeof(server_addr))) perr(err_failed);
    
    if (err_failed == listen(listen_fd, 5)) perr(err_failed);

    while(1)
    {
        memset((void *)&client_addr, 0x00, sizeof(client_addr));
        printf("accept wait\n");
        client_fd = accept(listen_fd,
                (struct sockaddr*)&client_addr, &addrlen);
        while(1)
        {
            if(client_fd == -1) err("accept error", 0);

            memset(buf, 0x00, MAXBUF);
            readn = read(client_fd, buf, MAXBUF);
            if(readn == 0) break;
            printf("==> %s", buf);
            write(client_fd, buf, strlen(buf));
        }
    }
    return 0;

}

