/*
    sscanf example
    send "hello world\n my name\n"
    buffer contains ["hello world\nmy"], [ name\n]
    sscanf(stream scanf) will handle delimiters(\n) or others (','..etc)
*/

#include <stdio.h>

struct uinfo
{
    int age;
    char name[16];
    char address[32];
    char nickname[16];
};

const char *doc_format = "%d,%[^,],%[^,],%[^,]"; // string format = regexp!?
int main(int argc, char* argv[])
{
    struct uinfo user_info;
    char *data = "19740208, yun sang bae, seoul, yundream"; // writer's info
    sscanf(data, doc_format, &user_info.age,
         user_info.name, user_info.address, user_info.nickname);
    printf("age : %d\n", user_info.age);
    printf("name : %s\n", user_info.name);
    printf("address : %s\n", user_info.address);
    printf("nickname : %s\n", user_info.nickname);

    return 0;
}
