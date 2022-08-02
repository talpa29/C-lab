#include <unistd.h>
#include "util.h"
#include "stdlib.h"

extern int system_call();
#define SYS_WRITE 4
#define STDOUT 1
#define SEEK 19
#define CLOSE 6
#define OPEN 5

int main (int argc , char* argv[], char* envp[])
{
    int fd = system_call(OPEN,argv[1], 0x001, 0644);
    if(fd < 0){
        system_call(CLOSE,"/home/tal29/Arch_lab/lab4/task0/task0b/greeting", 0644);
        system_call(56);
    }
    system_call(SEEK,fd, 0x506, SEEK_CUR);
    system_call(SYS_WRITE,fd,argv[2],5);

    return 0;
}
