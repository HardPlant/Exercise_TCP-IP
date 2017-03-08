Linux Multithread Programming
====
* Create Thread (POSIX thread)
  > \#include <pthread.h>
  > int pthread_create(pthread_t *thread, pthread_attr_t *attr,
  >                      void* (*start_routine)(void*), void *arg);
  > // thread : thread id
  > // attr : property of thread, NULL to use default 
  > // start_routine : function pointer
  > // arg : function's argument

  fork() can be used without args, but thread is more complex.
  thread changes code with context, which mean function.

* Wait/fork Thread

  > \#include <pthread.h>
  > int pthread_join(pthread_t th, void **thread_return);
  > // th : thread id
  > // thread_return : saves thread exit value

  Parent process waits() for child's exit().
  Parent thread pthread_join() for worker thread.
  if thread doesn't pthread_join(), it consumes resource.

  this function also blocks, like wait().
  wait() meets signal, multithread meets pthread_detach to unblock.

  > int pthread_detach(pthread_t th);
  > th : thread id. detaches worker thread.

  this function means parent will not wait worker.
