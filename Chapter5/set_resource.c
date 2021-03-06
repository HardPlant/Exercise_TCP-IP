#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
// How to modify capablity of a OS can make sockets
int main(int argc, char **argv)
{
    struct rlimit rlim;

    getrlimit(RLIMIT_NOFILE, &rlim);
    printf("Open file %d : %d\n", (int)rlim.rlim_cur, rlim.rlim_max);

    rlim.rlim_cur += 1024;
    rlim.rlim_max += 1024;
    if(setrlimit(RLIMIT_NOFILE, &rlim) == -1) return 0;
    
    printf("Open file %d : %d\n", rlim.rlim_cur, rlim.rlim_max);
    return 0;

}