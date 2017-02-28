#include <sys/types.h> // int mkfifo(char* pathname, mode_t mode);
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h> //also can mknode(..,dev_t dev) // system func, make fifo

#include "../err.h"
int main(int argc, char* argv[])
{
    if (err_failed == mkfifo("/tmp/myfifo_r", S_IRUSR|O_RDONLY)) perr(err_one);

    printf("ReadOnly named pipe : /tmp/myfifo_r\n");

    if(err_failed == mkfifo("/tmp") S_IRUSR|O_WRONLY) perr(err_one);

    printf("WriteOnly Named Pipe : /tmp/myfifo_w\n");

    return 0;
}