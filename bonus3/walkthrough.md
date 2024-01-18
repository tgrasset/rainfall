# Bonus3

## Code
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char **argv, const char **envp)
{
  char buffer[132]; 
  FILE *pass; 

  pass = fopen("/home/user/end/.pass", "r");
  memset(buffer, 0, 132);
  if ( !pass || argc != 2 )
    return -1;
  fread(buffer, 1, 66, pass);
  buffer[65] = 0;
  buffer[atoi(argv[1])] = 0;
  fread(&buffer[66], 1, 65, pass);
  fclose(pass);
  if ( !strcmp(buffer, argv[1]) )
    execl("/bin/sh", "sh", 0);
  else
    puts(&buffer[66]);
  return 0;
}
```
## Explanation

- After having played with the executable and explored the disassembled version, we understood that
    the program opens the .pass file, copies it to a buffer and does some other stuff, and most
    importantly inserts a null byte in the buffer at the offset given by atoi(argv[1]).

- So we tried a lot of values and tried to understand what the program did every time, but soon
    realised that we wouldn't be able to go anywhere, and that the answer was actually very simple.

- man atoi reminded us that the function returned 0 if given non numeric values, so there was one
    solution to satisfy this condition and execute a shell :
```
if ( !strcmp(buffer, argv[1]) )
    execl("/bin/sh", "sh", 0);
```
- THE EMPTY STRING !

- By having argv[1] as "", atoi(argv[1]) returns 0, and therefore puts a '\0' at the beginning of the
    buffer, and the call to strcmp() on 2 empty strings returns 0. 

## Exploit
```
$   ./bonus3 ""
```
