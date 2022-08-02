#include <unistd.h>
#include "util.h"
#include "stdlib.h"
#include "stdio.h"
#include <limits.h>


extern int system_call();
#define SYS_WRITE 4
#define STDOUT 1
#define STDIN 0
#define SEEK 19
#define CLOSE 6
#define OPEN 5
#define READ 3
#define STDERR 2
void count(char* argv[], char* envp[]);
void countD(char* argv[], char* envp[]);
void printErr(int id, int ret);

int main (int argc , char* argv[], char* envp[])
{
    int i;
    int Dflag = 0;
    int oflag = 0;
    int iflag = 0;
    char ostr[50];
    char istr[50];
    char ifile[150];
    char ofile[150];
    char * writeTO;
    char * readFrom;
    if(argc <= 1){
        count(STDIN , STDOUT);
    }
    else{
        for(i = 1; i < argc ;i = i + 1){
            if(argv[i][1] =='o' && argv[i][0] =='-'){
                oflag = 1;
                strcat(ostr,argv[i] + 2);
                writeTO = (int)system_call(OPEN,ostr,101,0644);
                strcat(ostr,"");
            }
            else if(argv[i][1] =='i' && argv[i][0] =='-'){
                iflag = 1;
                strcat(istr,argv[i] + 2);
                readFrom = (int)system_call(OPEN,istr,000,0644);
                strcat(istr,"");
            }
            else if(argv[i][1] =='D' && argv[i][0] =='-'){
                Dflag = 1;
            }
        }
        if(Dflag == 1 && oflag == 0 && iflag == 0){
            countD(STDIN,STDOUT);
        }
        else if(Dflag == 0 && oflag == 1 && iflag == 0){
            count(STDIN,writeTO);

        }
        else if(Dflag == 0 && oflag == 0 && iflag == 1){
            count(readFrom,STDOUT);
            system_call(CLOSE,readFrom);
        }
        else if(Dflag == 0 && oflag == 1 && iflag == 1){
            count(readFrom,writeTO);
            system_call(CLOSE,readFrom);
            system_call(CLOSE,writeTO);
        }
        else if(Dflag == 1 && oflag == 1 && iflag == 1){
            countD(readFrom,writeTO);
            system_call(CLOSE,readFrom);
            system_call(CLOSE,writeTO);
        }
    }


    return 0;
}

void count(char* argv[], char* envp[]){
    int eof = 1;
    int c = 0;
    char * str = itoa(INT_MAX);
    system_call(READ,argv,&str, 1);
    while(eof != 0){
        while(str[0] != '\n') {
            if (str[0] == ' ') {
                while (str[0] == ' ' && str[0] != '\n') {
                    system_call(READ, argv, &str[0], 1);
                }
            }
            else {
                c = c + 1;
                while (str[0] != ' ' && str[0] != '\n') {
                    system_call(READ, argv, &str[0], 1);
                }
            }
        }
        system_call(SYS_WRITE,envp, itoa(c) ,1);
        system_call(SYS_WRITE,envp, "\n" ,1);
        eof = system_call(READ,argv,&str[0], 1);
        c = 0;
    }

}

void countD(char* argv[], char* envp[]){
    int eof = 1;
    int c = 0;
    int id;
    int ret;
    char * str = itoa(INT_MAX);
    ret = system_call(READ,argv,&str, 1);
    id = 3;
    printErr(id, ret);
    while(eof != 0){
        while(str[0] != '\n') {
            if (str[0] == ' ') {
                while (str[0] == ' ' && str[0] != '\n') {
                    ret = system_call(READ, argv, &str[0], 1);
                    id = 3;
                    printErr(id,ret);
                }
            }
            else {
                c = c + 1;
                while (str[0] != ' ' && str[0] != '\n') {
                    ret = system_call(READ, argv, &str[0], 1);
                    id = 3;
                    printErr(id,ret);
                }
            }
        }
        ret = system_call(SYS_WRITE,envp, itoa(c) ,1);
        id = 4;
        printErr(id,ret);
        eof = system_call(READ,argv,&str[0], 1);
        id = 3;
        printErr(eof,ret);
        c = 0;
    }

}

void printErr(int id, int ret){
    system_call(SYS_WRITE,STDERR, "\n",1);
    system_call(SYS_WRITE,STDERR, itoa(id),1);
    system_call(SYS_WRITE,STDERR, "\n",1);
    system_call(SYS_WRITE,STDERR, itoa(ret),1);
    system_call(SYS_WRITE,STDERR, "\n",1);
}
