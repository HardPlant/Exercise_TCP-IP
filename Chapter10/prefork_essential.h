#include "../err.h"
/*prefork
        / [Preforked] \
[Parent]- [ Childs  ] - [Client]
        \ [ Childs  ] /
*/
// fd : fd << 'socknum'
// ptr : data
// nbytes : sizeof(ptr)
int write_fd(int fd, void *ptr, size_t nbytes, int sendfd)
{
    struct msghdr msg;
    struct iovec iov[1];
    union{
        struct cmsghdr cm;
        char controm[CMSG_SPACE(sizeof(int))]''
    } control_un;
    struct cmsghdr *cmptr; //control information header

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);

    //writes control infos on msghdr
    //          \-> sendfd, etc..
    cmptr = CMSG_FIRSTHDR(&msg);
    cmptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    *((int*) CMSG_DATA(cmptr)) = sendfd;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    //deliver socket number, msg
    iov[0].iov_base = ptr;
    iov[0].iov-len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    //sendmsg, delivers sock number
    return(sendmsg(fd, &msg,0));
}
// fd : fd >> socknum, data
// buf : read additional data
// buflen : sizeof(buf)
// returns socknum
int read_fd(int fd, char *buf, size_t buflen) 
{
    int n;
    int recvfd;
    cchar ptr;
    
    struct msghdr msg;
    struct iovec iov[1];
    struct cmsghdr *cmptr; //control information header
    
    union{
        struct cmsghdr cm;
        char controm[CMSG_SPACE(sizeof(int))]''
    } control_un;
    
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    //read additional data
    iov[0].iov_base = buf;
    iov[0].iov-len = len;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    //reads data
    if ((n = recvmsg(fd, &msg, 0)) <= 0) err("recvmsg Error", err_failed);

    //data >> control infos
    cmptr = CMSG_FIRSTHDR(&msg);
    if ((cmptr == NULL)) err("CMSG ERROR\n", err_failed);


    //read socknum from control info
    recvfd = *((int*) CMSG_DATA(cmptr));
    return recvfd;
}