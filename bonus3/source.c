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