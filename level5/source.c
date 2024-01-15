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