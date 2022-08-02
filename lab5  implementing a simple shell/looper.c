#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void sig_handler(int signum) {
    char* signame = strsignal(signum);
    printf("Looper handling %s\n", signame);
    if(signum == SIGTSTP) {
        signal(signum,SIG_DFL);
        raise(signum);
    }
    else if(signum == SIGCONT) {
        signal(signum,SIG_DFL);
        raise(signum);
    }
    else {
        signal(signum,SIG_DFL);
        raise(SIGINT);
    }
}

int main(int argc, char **argv){
    signal(SIGINT, sig_handler);
    signal(SIGTSTP, sig_handler);
    signal(SIGCONT, sig_handler);
	while(1) {

    }

	return 0;
}