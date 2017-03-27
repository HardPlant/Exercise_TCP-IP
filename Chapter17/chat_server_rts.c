#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>
#include "../err.h"
#define PORT_NUM 3600
#define MAXLINE 1024

int user_fds[1024];

int setup_sigio(int fd)
{
    if(fcntl(fd, F_SETFL, O_RDWR|O_NONBLOCK|O_ASYNC)< 0)
        return err_failed;
    if(fcntl(fd, F_SETSIG, SIGRTMIN) < 0)
        return err_failed;
    if(fcntl(fd, F_SETOWN, getpid()) < 0)
        return err_failed;

    return 1;
}

int make_listener()
{
    int addrlen;
    int state;
    int listenfd;

    struct sockaddr_in addr;
    addrlen = sizeof(addr);
    printf("%s\n", "proceed.");

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
        return err_failed;
    
    memset((void*)&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT_NUM);
    printf("%s\n", "proceed.");
    printf("%s\n", "proceed.");
    if(bind(listenfd, (struct sockaddr*)&addr, addrlen
        == err_failed))
        return err_failed;
    printf("%s\n", "proceed.");
    if(listen(listenfd, 5)
        == err_failed)
        return err_failed;
    
    if(setup_sigio(listenfd))
        return listenfd;
    else
        return err_failed;
}

int make_connected_fd(int listen_fd)
{
    int sockfd;
    socklen_t addrlen;
    struct sockaddr_in addr;
    sockfd = accept(listen_fd, (struct sockaddr*)&addr, &addrlen);
    if(sockfd == err_failed)
        return err_failed;
    if(setup_sigio(sockfd) == err_failed)
        return err_failed;
    user_fds[sockfd] = 1;

    return sockfd;
}

void send_chat_msg(int fd, char *msg)
{
    int i;
    char buf[MAXLINE+24];
    memset(buf, 0x00, MAXLINE+24);
    sprintf(buf, "user name(%d) %s", fd, msg);
    for(i = 0; i < 1024; i++)
    {
        if(user_fds[i] == 1)
            write(i, buf, strlen(buf));
    }
}

int main(int argc, char *argv[])
{
    siginfo_t si;
    sigset_t set;
    int listenfd;

    int ev_num;
    char buf[MAXLINE];
    int readn;

    memset((void*)user_fds, 0x00, sizeof(int)*1024);
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN);
    sigprocmask(SIG_BLOCK, &set, NULL);

    listenfd = make_listener();
    if(listenfd == err_failed)
        perr(err_failed);
    while(1)
    {
        ev_num = sigwaitinfo(&set, &si);
        if(ev_num == SIGRTMIN)
        {
            if(si.si_fd == listenfd)
            {// if listenfd event
                if(make_connected_fd(listenfd) == err_failed)
                    perr(err_failed);
            }
            else
            {// if clientfd event
                memset(buf, 0x00, MAXLINE);
                readn = read(si.si_fd, buf, MAXLINE);
                if(readn <=0)
                {
                    user_fds[si.si_fd] = 0;
                    close(si.si_fd);
                }
                else
                    send_chat_msg(si.si_fd, buf);
                
            }
        }
    }

}

