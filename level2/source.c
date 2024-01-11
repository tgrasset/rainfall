#include <stdio.h>

void    p(void) {
    int buffer[16];

    fflush(stdout);
    gets(buffer);
    int ret = buffer[20];
    if ((ret & 0xb0000000) == 0xb0000000) {
        printf("(%p)\n", ret);
        exit(1);
    }
    puts(buffer);
    strdup(buffer);
}

int main(void) {
    p();
}

