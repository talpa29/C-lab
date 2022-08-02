#include "util.h"

extern int system_call();
#define SYS_WRITE 4
#define STDOUT 1

int main (int argc , char* argv[], char* envp[])
{
    system_call(SYS_WRITE,STDOUT, "Hello marwany",13);
    return 0;
}
