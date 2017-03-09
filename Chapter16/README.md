Linux I/O Model
====

  I/O Have some models. now we will discuss some important models.

  * I/O models
  * Application of I/O models
  * pros and cons

      \     Block          NonBlock
 Sync      read()/write   read()/write()
                            (O_NONBLOCK)
 Async    I/O Multiplize   AIO, Realtime Signal

* Block and NonBlock

  be used for classify program status.
  for instance, write()/read() will block until have data to write/read

* Sync and async

  Sync uses clock, but async doesn't.

###Sync/Block Model

  read() waits until data to read. and read() syncs with incoming data.
  All examples following this model. (Single thread, multi thread, multi process)
  this model cannot handle two or more socket.
  but, this model is simple and safe, so many unix program follows this model.

###Sync/Unblock Model

  in unblock model, we need unblock socket.
  when read() to unblock socket, kernel just returns after socket 
  is ready for read. so, one process can handle several sockets.
  we can make this socket by fcntl().

  > int fd;
  > int flags;
  > if ((flags = fcntl(fd_F_GETFL,0)) == -1) // get fd property
  >{
  >     flags=0;   
  >}
  >fcntl(fd, F_SETFL, flags | O_NONBLOCK); // make nonblock socket
  
  then read() this socket will continue unless read() bring data, with return -1.
  if we want to check err, we must check errno.
  if errno value is EAGAIN or EWOULDBLOCK, this is return of unblock.
  
  >\#include <errno.h>
  >extern int errno;

  

