I/O Multiplization
====
How to :
> 1. make group of fds
> 2. check if read data exists on fds in group
> 3. if one of fd have data to read, process read.
> 4. after process, go to 2.

Socket Programming + I/O Multiplization
====
socket : accept() or read() in one thread
-> accept() if socket == listen, read() if socket == connect

Scnario : 
 1. call socket()
 2. call bind()
 3. call listen()
 4. add listening socket to fd_set

 5. call select()
 6. select() returns if data come to listening / connected
 7. check socket to FD_ISSET macro
     * if data is income to listening socket, call accept() to connect with client
     -> add connect socket to fd_set
     * if data is imcome to connected socket, read() it and process.
 8. go to 5.

