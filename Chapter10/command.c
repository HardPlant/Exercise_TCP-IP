#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("--START--\n");
    execl("/bin/ls","ls",NULL); // execl(*path, *arg, ..);
    printf("--END--\n"); // never executes! execl overwrites current proc image
    return 1;
}