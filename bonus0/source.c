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