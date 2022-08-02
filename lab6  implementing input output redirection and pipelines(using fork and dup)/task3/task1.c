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
int ** createPipes(int nPipes);
void releasePipes(int **pipes, int nPipes);
int *leftPipe(int **pipes, cmdLine *pCmdLine);
int *rightPipe(int **pipes, cmdLine *pCmdLine);

int main() {

    char termination [] = "quit";
    char *last_command = NULL;
    int status;
    int pipe_number;
    int ** pipes;
    int *leftfrompipe;
    int *rightfrompipe;
    cmdLine* tmp;
    pid_t pid;

    do{
        char line[2048];
        pipe_number = 0;

        char path_name[PATH_MAX];
        getcwd(path_name, sizeof(path_name));
        printf("%s:", path_name);

        fgets(line,sizeof(line),stdin);
        line[strcspn(line, "\n")] = 0;

        if(strcmp (line, termination) == 0) {
            break;
        }

        if(strcmp(line,"prtrls") != 0) {
            last_command = malloc(sizeof(line));
            strcpy(last_command, line);
        }
        cmdLine* parsed_line;
        parsed_line = parseCmdLines(line);

        if(strcmp((parsed_line->arguments[0]),"cd") == 0){

            int value = chdir(parsed_line->arguments[1]);
            if(value < 0)
                perror("Error:");

            continue;
        }
        else if (strcmp((parsed_line->arguments[0]), "prtrls") == 0)
        {
            printf("%s\n", last_command);
            freeCmdLines(parsed_line);
            continue;
        }
        else {
            tmp = parsed_line->next;
            while (tmp != NULL) {
                pipe_number++;
                tmp = tmp->next;
            }
            tmp = parsed_line;
            pipes = createPipes(pipe_number);
            for (int i = 0; i < pipe_number + 1; ++i, parsed_line = parsed_line->next) {

                leftfrompipe = leftPipe(pipes, parsed_line);
                rightfrompipe = rightPipe(pipes, parsed_line);

                pid = fork();
                if(pid > 0){

                    if (leftfrompipe != NULL)
                    {
                        close(leftfrompipe[0]); /* Close the read end of the pipe. */
                    }
                    if (rightfrompipe != NULL)
                    {
                        close(rightfrompipe[1]); /* Close the write end of the pipe. */
                    }
                    {
                        do
                        {
                            if (waitpid(pid, &status, WUNTRACED | WCONTINUED) == -1)
                                perror("wait() error");
                        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                    }
                }
                else
                { /* Child code */
                    if(parsed_line->blocking == 1)
                    {
                        waitpid(pid, &status, 0);
                    }
                    if (leftfrompipe != NULL)
                    {
                        close(STDIN_FILENO);             /* Close the standard input. */
                        dup(leftfrompipe[0]); /* Duplicate the read-end of the pipe using dup */
                        close(leftfrompipe[0]);           /* Close the file descriptor that was duplicated.*/
                    }

                    if (rightfrompipe != NULL)
                    {
                        close(STDOUT_FILENO);             /* Close the standard output. */
                        dup(rightfrompipe[1]); /* Duplicate the write-end of the pipe using dup */
                        close(rightfrompipe[1]);           /* Close the file descriptor that was duplicated.*/
                    }

                    execute(parsed_line);
                }
            }
            releasePipes(pipes,pipe_number);
        }
        freeCmdLines(tmp);

    } while(1);

    return 0;
}



int execute(cmdLine *pCmdLine){
    int result = 0;
    Redirection(pCmdLine);
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


int ** createPipes(int nPipes){
    int** pipes;
    pipes=(int**) calloc(nPipes, sizeof(int));

    for (int i=0; i<nPipes;i++){
        pipes[i]=(int*) calloc(2, sizeof(int));
        pipe(pipes[i]);
    }
    return pipes;

}
void releasePipes(int **pipes, int nPipes){
    for (int i=0; i<nPipes;i++){
        free(pipes[i]);

    }
    free(pipes);
}
int *leftPipe(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->idx == 0) return NULL;
    return pipes[pCmdLine->idx -1];
}

int *rightPipe(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->next == NULL) return NULL;
    return pipes[pCmdLine->idx];
}