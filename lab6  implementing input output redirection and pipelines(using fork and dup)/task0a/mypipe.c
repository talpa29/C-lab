#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char buf;
    int p[2];
    pipe(p);

    if(fork() > 0) {
        close(p[1]);
        while (read(p[0], &buf, 1) != 0)
            putchar(buf);
        close(p[0]);
    }
    else {
        close(p[0]);
        write(p[1], "hello\n", sizeof("hello\n"));
        close(p[1]);
        wait(NULL);
    }
    return 0;
}
