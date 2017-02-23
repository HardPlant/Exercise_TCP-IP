#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h> // for gethostbyname()
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../err.h"

int main(int argc, char** argv)
{
    struct hostent *myHostEntries;

    if(argc != 2) err("usage : [domain name]\n", err_one);

    myHostEntries = gethostbyname(argv[1]);
    if(myHostEntries == NULL)
    {
        perror("[ERROR]");
        return 1;
    }
    printf("Host Name : %s\n", myHostEntries->h_name);

    while(*myHostEntries->h_addr_list != NULL)
    {
        printf("%s\n",
            inet_ntoa(*(struct in_addr*) *myHostEntries->h_addr_list));
        myHostEntries->h_addr_list++;
    }

    return 0;
}