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