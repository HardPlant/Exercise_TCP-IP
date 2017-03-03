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

## I/O Functions

 * read <=> fgets, write<=> fputs compatible
  - socket uses fd(int), fileI/O uses FILE*
  - we can get fd from FILE* using fdopen();

 * formatted Out Functions
  - printf : stdout
  - sprintf : string
  - fprintf : file

 * formatted In Functions 

  - scanf : stdin
  - sscanf : string
  - fscanf : file

  * format string

   - %[a-zA-Z]%[0-9] -> alphabetdata, intdata

  * fdopen(int fd, const char *mode) -> fd to FILE\*

   - usage:
   >int sock_fd;
   >FILE* sock_fp;
   >sock_fd = socket(..);
   >r_sock_fp = fdopen(sock_fd, "r"); // or "r+ / w+", rw is undefined
   >w_sock_fp = fdopen(sock_fd, "w");
   we can set multithread to read, process, write (but not used frequetly)

  * int fileno(FILE *stream)
    FILE* to fd

  * int fclose(FILE *fp);
    fopen(), fdopen() -> must close (internally calls close(), socket closed)
