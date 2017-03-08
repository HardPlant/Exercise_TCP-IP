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

* Use mutex instead of semaphore in thread.

  because mutex clarfies code than semaphore.
  semaphore uses arrays, must care of access and haves
  mutex has pthread_ prefix, and easy to use.

* Create mutex

  First, create mutex object.

  > int pthread_mutex_init(pthread_mutex_t * mutex
  >     , const pthread_mutex_attr *attr);
  > // mutex : a mutex object to init.
  > // attr : attribute. NULL to use default.
  > // return 0 if success, or non-zero.

  if mutex don't be used anymore, destory it.

  > int pthread_mutex_destroy(pthread_mutex_t *mutex);

* lock and unlock mutex

  > int pthread_mutex_lock(pthread_mutex_t *mutex);
  > int pthread_mutex_unlock(pthread_mutex_t *mutex);

* conditional variable for pthread

  pthread provides signal system, wait and awake.

  First, make conditional variable object.
  > int pthread_cont_init(pthread_cond_t *cond
        , const pthread_cond_attr *attr);
  > // cond : condVar object.
  > // attr : attribute. NULL to default.

  if no more use, destroy it.
  > int pthread_cond_destroy(pthread_cont_t *cond);

  - Wait for thread

  provides pthread_cond_wait(), pthread_cond_timedwait()

  > int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
  > int pthread_cont_timedwait(pthread_cond_t *cond,
  >    pthread_mutex_t *mutex, const struct timespec *abstime);
  > // cond : condVar object.
  > // mutex : mutex object.
  > // abstime : set wait time.
  > return 0 if success, else return nonzero.
  > if pthread_cond_timedwait fails with timeout, errno will set to 'ETIMEOUT'

  condVar is used with several thread, so condVar may be protected with mutex.
  if two or more thread waits for condVar, we cannot know which thread
  will awakes, because we don't know what thread will get mutex lock.

  if a thread get mutex lock before pthread_cond_wait(), still another thread 
  can wait for condVar. because pthread_cond_wait() automatically unlocks mutex.

  if main thread signals to condVar, one of waiter thread will get mutex lock
  , and awakes.

  - Awake thread
  
  > int pthread_cond_broadcast(pthread_cond_t *cond);
  > int pthread_cond_signal (pthread_cond_t *cond);

  signal to condVar, awake one of waiter thread.
  we can't know which thread will awakes. broadcast() will awake all waiters.

  


