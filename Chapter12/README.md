## TCP

  ssize_t send(int sockfd, void *buf, size_t len, int flags);
  ssize_t recv(int sockfd, const void *buf, size_t len, int flags);

## UDP

  ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                    struct sockaddr *src_addr, socklen_t *addrlen);
  ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                    const struct sockaddr *dest_addr, socklen_t addrlen);

## Socket Flags(int flags)

 > MSG_PEEK, MSG_DONTROUTE, MSG_DONTWAIT
   - MSG_PEEK : Don't flush buffer after read, use in multi subroutines
   - MSG_DONTROUTE : do not refer route table (LAN), only used in send(), sendto()
   - MGS_DONTWAIT : try no-wait(unlocking) communicate // Chapter 16

* OOB(out of band)
  Urgent message (do not send prior to other msgs, just emergency route)
  On-time gurantee (ignore Naggle algorithm, send immediately)
  uses urgent pointer in TCP packet

  when OS received this data, OS raises SIGURG signal -> program will handle
