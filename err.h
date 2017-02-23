#ifndef ERR_H
#define ERR_H
#define err_one 1
#define err_two 2
#define err_fail -1
#define err_badOp 1
#define err_badOperand 2

int err(char* msg, int retn)
{
    fprintf(stderr,"%s",msg);
    exit(retn);
}

#endif