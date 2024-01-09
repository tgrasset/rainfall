#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char **argv)
{
 
    char *arg[2];
    gid_t id;
    uid_t uid;

    if (atoi(argv[1]) == 423) //0x1a7
    {
        arg[0] = strdup("/bin/sh");
        arg[1] = NULL;
        id = getegid();
        uid = geteuid();
        setresgid(id, id, id);
        setresuid(uid, uid, uid);
        execv("/bin/sh", arg);
    }
    else 
        fwrite("No !\n", 1, 5, stderr);
    return (0);
}
