#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char **argv)
{
    char strcpy;
    gid_t id;
    uid_t uid;

    if (atoi(argv[1]) == 423) //0x1a7
    {
        strcpy = strdup("/bin/sh");
        id = getegid();
        uid = geteuid();
        setresgid(id, id, id);
        setresuid(uid, uid, uid);
        execve("/bin/sh", &strcpy, NULL);
    }
    else 
        fwrite("No !\n", 1, 5, stderr);
    return (0);
}