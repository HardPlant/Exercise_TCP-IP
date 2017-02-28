#ifndef ERR_H
#define ERR_H
#define err_one 1
#define err_two 2
#define err_fail -1
#define err_failed -1
#define err_badOp 1
#define err_badOperand 2

#include <stdio.h>
#include <stdlib.h>

int err(char* msg, int retn)
{
    fprintf(stderr,"%s",msg);
    exit(retn);
}

void perr(int errcode)
{
    perror("[ERROR] ");
    exit(errcode);
}
#endif