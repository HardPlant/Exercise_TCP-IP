#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h> // for epoll, provides event pool create/management
#include <string.h>
#include <stdio.h>
#include "../err.h"
#define PORT_NUM 3600
#define EPOLL_SIZE 20
#define MAXLINE 1024

struct udata
{
    int fd;
    char name[8];
};

int user_fds[1024];
void send_msg(struct epoll_event ev, char *msg);

int main(int argc, char *argv[])
{
    struct sockaddr_in addr, clientaddr;
    struct epoll_event ev, *events;
    struct udata *user_data;
    int listenfd;
    int clientfd;
    int i;
    socklen_t addrlen, clilen;
    int readn;
    int eventn;
    int epollfd;
    char buf[MAXLINE];

    events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    if(err_failed == (epollfd = epoll_create(100)))
        return err_failed;

    addrlen = sizeof(addr);
    if(err_failed ==
        (listenfd = socket(AF_INET, SOCK_STREAM, 0)))
        return err_failed;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(err_failed == 
        bind(listenfd, (struct sockaddr*)&addr, addrlen))
        return err_failed;
    
    listen(listenfd, 5);
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
    memset(user_fds, -1, sizeof(int)*1024);

    while(1)
    {
        eventn = epoll_wait(epollfd, events, EPOLL_SIZE, -1);
        if(eventn == err_failed) return err_failed;

        for(i = 0; i < eventn; i++)
        {
            if(events[i].data.fd == listenfd)
            {//event occurs in listen -> make connect
            clilen = sizeof(struct sockaddr);
            clientfd = accept(listenfd,
                (struct sockaddr*)&clientaddr, &clilen);
            user_fds[clientfd] = 1;

            user_data = malloc(sizeof(user_data));
            user_data->fd = clientfd;
            sprintf(user_data->name, "user(%d)", clientfd);

            ev.events = EPOLLIN;
            ev.data.ptr = user_data;

            epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
            }
            else
            {//event occurs in clientfd->read data
                user_data = events[i].data.ptr;
                memset(buf, 0x00, MAXLINE);
                readn = read(user_data->fd, buf, MAXLINE);
                if(readn <= 0)
                {// disconnect event, remove user
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->fd,
                        events);
                    close(user_data->fd);
                    user_fds[user_data->fd] = -1;
                    free(user_data);
                }
                else
                {
                    send_msg(events[i], buf);
                }
            }
        }
    }
    return 0;
}

void send_msg(struct epoll_event ev, char *msg)
{
    int i;
    char buf[MAXLINE+24];
    struct udata *user_data;
    user_data =ev.data.ptr;
    for(i=0; i<1024; i++)
    {
        memset(buf, 0x00, MAXLINE+24);
        sprintf(buf, "%s %s", user_data->name, msg);
        if((user_fds[i] == 1))
        {
            write(i,buf,MAXLINE+24);
        }
    }
}