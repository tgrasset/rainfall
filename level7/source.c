#include <stdio.h>
#include <stdlib.h>

char c[80] = "";

void m()
{
    printf("%s - %d\n", &c, time(NULL));
}

int main(int argc, char** argv)
{
    int* s1 = malloc(8);
    s1 = 1;
    s1[1] = malloc(8);
    int s2 = malloc(8);
    *s2 = 2;
    s2[1] = malloc(8);

    strcpy(s1[1], argv[1]);
    strcpy(s2[1], argv[2]);

    fgets(&c, 68, fopen("/home/user/level8/.pass", "r"));
    puts("~~");
    return (0);
}