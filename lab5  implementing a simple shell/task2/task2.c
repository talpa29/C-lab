#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./LineParser.h"
#include <unistd.h>

#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct process{
    cmdLine* cmd;                     /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                       /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	          /* next process in chain */
} process;
#define TERMINATED  -1
#define SUSPENDED 0
#define RUNNING 1


#define MYSHELL_MAX_INPUT 2048

void execute(cmdLine *pCmdLine, process** proc_list);
void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);

void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);
void printProcessList(process** process_list);
void clearTerminatedProcesses(process** process_list);
void nap(cmdLine* cmd);
void stop(cmdLine* pCmdLine);

int debug_mode = 0;
int main(int argc, char** argv){
    char currentPath[PATH_MAX];
    process** proc_list = (process **)malloc(sizeof(process));
    if(argc > 1) {
        if(strcmp(argv[1],"-d") == 0)
            debug_mode = 1;
    }
    while(1){
        if(getcwd(currentPath,PATH_MAX) == 0x00)
            printf("problem in cwd func");
        printf("~%s/ ",currentPath);
        char input[MYSHELL_MAX_INPUT];
        if(fgets(input,MYSHELL_MAX_INPUT,stdin) == 0x00)
            printf("problem in the input from user");

        cmdLine* cmdInput = parseCmdLines(input);
        char* command = cmdInput->arguments[0];
        if(strcmp(command,"quit") == 0){
            freeProcessList(*proc_list);
            freeCmdLines(cmdInput);
            free(proc_list);
            return 0;
        }
        else if(strcmp(command,"cd") == 0){
            int result = chdir(cmdInput->arguments[1]);
            if(result  < 0){
                fprintf(stderr,"cd failed, error number : %d \n",result);
            }
            freeCmdLines(cmdInput);
        }
        else if(strcmp(command,"showprocs") == 0){
            printProcessList(proc_list);
            freeCmdLines(cmdInput);
        }
        else if(strcmp(command,"nap") == 0){
            nap(cmdInput);
            freeCmdLines(cmdInput);
        }
        else if(strcmp(command,"stop") == 0 ){
            stop(cmdInput);
            freeCmdLines(cmdInput);
        }
        else
            execute(cmdInput,proc_list);


        printf("\n");
    }
    return 0;
}


void execute(cmdLine *pCmdLine,process** proc_list){
    int pid = fork();
    int status = 0;
    if(pid == 0){
        char* command = pCmdLine->arguments[0];
        if(execvp(command,pCmdLine->arguments) < 0){
            freeCmdLines(pCmdLine);
            freeProcessList(*proc_list);
            free(proc_list);
            exit(1); /* error! */
        }
        exit(0);
    }
    else{
        if(debug_mode){
            fprintf(stderr,"The process id is %d\nThe executing command is %s\n",
                    pid,pCmdLine->arguments[0]);
        }
        addProcess(proc_list,pCmdLine,pid);
        if(pCmdLine->blocking != 0 )
            pid = waitpid(pid,&status,0);

    }


}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process* cur_proc = *process_list;
    if(cur_proc == NULL){
        *process_list = (process *)malloc(sizeof(process));
        cur_proc = *process_list;
    }
    else{
        while(cur_proc->next != NULL){
            cur_proc = cur_proc->next;
        }
        cur_proc->next = (process *)malloc(sizeof(process));
        cur_proc = cur_proc->next;
    }
    cur_proc->cmd = cmd;
    cur_proc->pid = pid;
    if(cmd->blocking == 0)
        cur_proc->status = RUNNING;
    else
        cur_proc->status = TERMINATED;

}


void printProcessList(process** process_list){
    process* cur_proc = *process_list;
    char *status;
    printf("PID \t Command \t STATUS \n");
    if(cur_proc == NULL)
        return;
    updateProcessList(process_list);
    while(cur_proc != NULL){
        if(cur_proc->status == -1) {
            status = "TERMINATED";
        }
        else if(cur_proc->status == 0) {
            status = "SUSPENDED";
        }
        else {
            status = "RUNNING";
        }
        printf("%d \t %s \t\t %s \b\n",cur_proc->pid,cur_proc->cmd->arguments[0],status);
        cur_proc = cur_proc->next;
    }
    clearTerminatedProcesses(process_list);
}

void freeProcessList(process* process_list){
    process* next;
    process* curr_process = process_list;
    while(curr_process != NULL){
        next = curr_process->next;
        freeCmdLines(curr_process->cmd);
        free(curr_process);
        curr_process = next;
    }
}

void updateProcessList(process **process_list){
    process* cur_proc = *process_list;
    while(cur_proc != NULL){
        int cur_proc_id = cur_proc->pid;
        int status = 0;
        int pid;
        pid = waitpid(cur_proc->pid,&status,WNOHANG | WUNTRACED | WCONTINUED);
        if(pid != 0)  { // there was a change
            if(WIFCONTINUED(status))
                updateProcessStatus(cur_proc,cur_proc_id,RUNNING);
            else if(WIFSTOPPED(status))
                updateProcessStatus(cur_proc,cur_proc_id,SUSPENDED);
            else if(WIFSIGNALED(status))
                updateProcessStatus(cur_proc,cur_proc_id,TERMINATED);


        }
        cur_proc = cur_proc->next;
    }
}

void updateProcessStatus(process* process_list, int pid, int status){
    process_list->pid = pid;
    process_list->status = status;
}
void clearTerminatedProcesses(process** process_list){
    process* cur_proc = *process_list;
    process* prev = NULL;
    process* temp = NULL;
    while(cur_proc != NULL){
        if(cur_proc->status == TERMINATED){
            if(prev != NULL){
                prev->next = cur_proc->next;
                temp = cur_proc->next;
                freeCmdLines(cur_proc->cmd);
                free(cur_proc);
                cur_proc = temp;
            }
            else{
                *process_list = cur_proc->next;
                freeCmdLines(cur_proc->cmd);
                free(cur_proc);
                cur_proc = *process_list;
            }
        }
        else{
            prev = cur_proc;
            cur_proc = cur_proc->next;
        }
    }
}

void nap(cmdLine* pCmdLine){
    int pid = fork();
    if(pid == 0){
        //Child Code
        if(pCmdLine->argCount != 3)
            exit(1);
        int nap_id = atoi(pCmdLine->arguments[2]);
        int sleep_time = atoi(pCmdLine->arguments[1]);
        if(kill(nap_id,SIGTSTP) < 0){
            fprintf(stderr,"Failed to nap process id = %d \n",nap_id);
            exit(1); /* error! */
        }
        sleep(sleep_time);
        if(kill(nap_id,SIGCONT) < 0){
            fprintf(stderr,"Failed to nap process id = %d \n",nap_id);
            exit(1); /* error! */
        }
        exit(0);
    }
}
void stop(cmdLine* pCmdLine){
    int pid = fork();
    if(pid == 0){
        //Child Code
        if(pCmdLine->argCount != 2)
            exit(1);
        int stop_id = atoi(pCmdLine->arguments[1]);
        if(kill(stop_id,SIGINT) < 0){
            fprintf(stderr,"Failed to stop process id = %d \n",stop_id);
            exit(1); /* error! */
        }
        exit(0);
    }
}