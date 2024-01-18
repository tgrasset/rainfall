# Level0

## Code
```
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
```


## Explanation

- The binary executable in the home folder of level0 :
    - Segfaults with no arguments
    - prints "No!" with a random argument, and exits

- We decided to disassemble it with `objdump -d ./level0` and saw a few things :
    - A call to atoi made us think that the argument probably needed to be numbers
    - Right after it, a comparison between the return value of atoi with a hard coded value (cmp    $0x1a7,%eax), 0x1a7 in hexa corresponds to 423 in decimal

- So we launched the program again with `./level 0 423`, which `execv /bin/sh` with level1 privileges

- All that was left to do was : `cat /home/user/level1/.pass` to see the password


## Exploit
```
$   ./level 0 423

$   cat /home/user/level1/.pass
```