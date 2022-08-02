#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./LineParser.h"
#include <unistd.h>

#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>



#define MYSHELL_MAX_INPUT 2048
void execute(cmdLine *pCmdLine);
int debug_mode = 0;
int main(int argc, char** argv){
    while(1){
        if(argc > 1) {
            if(strcmp(argv[1],"-d") == 0)
                debug_mode = 1;
        }
        char currentPath[PATH_MAX];
        if(getcwd(currentPath,PATH_MAX) == 0x00)
            printf("problem in cwd func");
        printf("~%s/ ",currentPath);
        char input[MYSHELL_MAX_INPUT];
        if(fgets(input,MYSHELL_MAX_INPUT,stdin) == 0x00)
            printf("problem in the input from user");

        cmdLine* cmdInput = parseCmdLines(input);
        char* command = cmdInput->arguments[0];
        if(strcmp(command,"quit") == 0){
            return 0;
        }
        else if(strcmp(command,"cd") == 0){
            int result = chdir(cmdInput->arguments[1]);
            if(result  < 0){
                fprintf(stderr,"cd failed, error number : %d \n",result);
            }
        }
        else execute(cmdInput);

        freeCmdLines(cmdInput);
        printf("\n");
    }
    return 0;
}


void execute(cmdLine *pCmdLine){
    int pid = fork();
    int status = 0;
    if(pid == 0){
        char* command = pCmdLine->arguments[0];
        if(execvp(command,pCmdLine->arguments) < 0) {
            perror("the error is: ");
            _exit(1); /* error! */
        }

        exit(0);
    }
    else{
        if(debug_mode){
            fprintf(stderr,"The process id is %d\nThe executing command is %s\n",
                    pid,pCmdLine->arguments[0]);
        }
        if(pCmdLine->blocking != 0 )
            pid = waitpid(pid,&status,0);

    }


}