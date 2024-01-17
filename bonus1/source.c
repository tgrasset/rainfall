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

//cc -fno-stack-protector  bonus1.c 