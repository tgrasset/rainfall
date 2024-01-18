int language = 0;

void    greetuser(char *src)
{
  char dest[64];

  switch ( language )
  {
    case 1:
      strcpy(dest, "Hyvää päivää ");
      break;
    case 2:
      strcpy(dest, "Goedemiddag! ");
      break;
    case 0:
      strcpy(dest, "Hello ");
      break;
  }
  strcat(dest, src);
  puts(dest);
}

int main(int argc, char *argv[])
{
  char dest2[76];
  char *env_LANG = NULL;
  char dest[64];

  if ( argc != 3 )
    return 1;
  memset(dest, 0, 76);
  strncpy(dest, argv[1], 40);
  strncpy(dest + 40, argv[2], 32);
  env_LANG = getenv("LANG");
  if ( env_LANG )
  {
    if ( !memcmp(env_LANG, "fi", 2) )
    {
      language = 1;
    }
    else if ( !memcmp(env_LANG, "nl", 2) )
    {
      language = 2;
    }
  }
  //memcpy(dest2, dest, sizeof(dest2));
  greetuser(dest2);
  return (0);
}