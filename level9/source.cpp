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