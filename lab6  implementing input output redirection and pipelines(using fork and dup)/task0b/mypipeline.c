#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "linux/limits.h"
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include<unistd.h>

int main(int argc, char** argv) {

    int debug_mode = 0;
    if(argc > 1 && strcmp(argv[1], "-d") == 0)
        debug_mode = 1;

    int pipefd[2];
    int pid;
    if ( pipe(pipefd) == -1) {
        printf("Unable to create pipe\n");
        return 1;
    }
    if(debug_mode)
        fprintf(stderr,"parent_process>forking…)\n");
    pid = fork();

    // Child process
    if (pid == 0) {
        close(STDOUT_FILENO);
        if(debug_mode)
            fprintf(stderr,"child1>redirecting stdout to the write end of the pipe\n");
        dup(pipefd[1]);
        close(pipefd[1]);
        char* cmd[] = {"ls", "-l" , NULL};
        if(debug_mode)
            fprintf(stderr,"child1>going to execute cmd:ls -l\n");
        int result = execvp(cmd[0],cmd);
        if(result == -1){
            perror("ERROR: ");
            _exit(1);
        }
        _exit(0);
    }

    else { //Parent process
        if(debug_mode)
            fprintf(stderr,"parent_process>created process with id: %d\n",pid);
        if(debug_mode)
            fprintf(stderr,"parent_process>waiting for child processes to terminate\n");
        wait(0);
        if(debug_mode)
            fprintf(stderr,"parent_process>closing the write end of the pipe…\n");
        close(pipefd[1]);
        pid = fork();
        if (pid == 0) {
            close(STDIN_FILENO);
            if(debug_mode)
                fprintf(stderr,"child2>redirecting stdin to the read end of the pipe…\n");
            dup(pipefd[0]);
            close(pipefd[0]);
            char* cmd[] = {"tail", "-n" ,"2", NULL};
            if(debug_mode)
                fprintf(stderr,"child2>going to execute cmd: tail -n 2\n");
            int result = execvp(cmd[0],cmd);
            if(result == -1){
                perror("ERROR: ");
                _exit(1);
            }
            _exit(0);
        }
        if(debug_mode)
            fprintf(stderr,"parent_process>closing the read end of the pipe\n");
        close(pipefd[0]);
        if(debug_mode)
            fprintf(stderr,"parent_process>waiting for child processes to terminate\n");
        wait(0);

    }
    if(debug_mode)
        fprintf(stderr,"parent_process>exiting\n");
    return 0;
}

