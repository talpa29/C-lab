#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "LineParser.h"
#include <unistd.h>
#include "linux/limits.h"
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int execute(cmdLine *pCmdLine);
void Redirection( cmdLine* parsed_line);

int main() {

    char termination [] = "quit";
    int status;

    do{
        char line[2048];

        char path_name[PATH_MAX];
        getcwd(path_name, sizeof(path_name));
        printf("%s:", path_name);

        fgets(line,sizeof(line),stdin);
        line[strcspn(line, "\n")] = 0;

        if(strcmp (line, termination) == 0)
            break;


        cmdLine* parsed_line;
        parsed_line = parseCmdLines(line);



        if(strcmp((parsed_line->arguments[0]),"cd") == 0){

            int value = chdir(parsed_line->arguments[1]);
            if(value < 0)
                perror("Error:");

            continue;
        }

        int pipefd[2];
        int pid;
        if (pipe(pipefd) == -1) {
            printf("Unable to create pipe\n");
            return 1;
        }

        pid = fork();
        if(pid > 0){

            if(parsed_line->blocking == 1)
                waitpid(pid,&status,0);

            if(parsed_line->next != NULL){
                close(pipefd[1]);
                pid = fork();
                if (pid == 0) {
                    close(STDIN_FILENO);
                    dup(pipefd[0]);
                    close(pipefd[0]);
                    Redirection(parsed_line->next);
                    int result = execute(parsed_line->next);
                    if(result == -1){
                        perror("ERROR: ");
                        _exit(1);
                    }
                    _exit(0);
                }
                close(pipefd[0]);
                waitpid(pid,&status,0);
            }
            continue;
        }


        if(parsed_line->next != NULL){

            close(STDOUT_FILENO);
            dup(pipefd[1]);
            close(pipefd[1]);
        }


        Redirection(parsed_line);

        int call_execute = execute(parsed_line);
        if(call_execute == -1){
            perror("ERROR: ");
            _exit(1);
        }

        // loop

        freeCmdLines(parsed_line);

    } while(1);

    return 0;
}



int execute(cmdLine *pCmdLine){
    int result = 0;
    char * parmList[(pCmdLine->argCount) +1];
    for( int i = 0; i < pCmdLine->argCount; i++){
        parmList[i] =  pCmdLine->arguments[i];
    }
    parmList[pCmdLine->argCount] = NULL;

    result = execvp(pCmdLine->arguments[0],parmList);

    return result;

}
void Redirection( cmdLine* parsed_line){
    int fd1;
    int fd2;
    if(parsed_line->inputRedirect != NULL){
        fd1 = open(parsed_line->inputRedirect, O_RDONLY );
        if(fd1 < 0){
            fprintf(stderr,"cant redirect input file");
            exit(0);
        }
        dup2(fd1,STDIN_FILENO);


    }

    if(parsed_line->outputRedirect != NULL){
        fd2 = open(parsed_line->outputRedirect,O_RDWR | O_CREAT,0777);
        if(fd2 < 0){
            fprintf(stderr,"cant redirect output file");
            exit(0);
        }
        dup2(fd2,STDOUT_FILENO);

    }
}
