#include <string.h>
#include <stdlib.h>

void n() {
    system("/bin/cat /home/user/level7/.pass");
}

void m() {
    puts("Nope");
}

int main(int ac, char **av) {
    int *buffer;
    void (**f)(void);

    buffer = (char *)malloc(64);
    f = (void (**)(void))malloc(4);

    *f = m;
    strcpy(buffer, av[1]);
    (*f)();
}