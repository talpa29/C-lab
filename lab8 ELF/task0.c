#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>
#include <unistd.h>

int debug_mode = 0;
int Currentfd = -1;

void *map_start;            /* will point to the start of the memory mapped file */
struct stat fd_stat;        /* this is needed to  the size of the file */
Elf32_Ehdr *header;         /* this will point to the header structure */
int num_of_section_headers;


void Toggle_Debug_Mode(){
    if(debug_mode == 0){
        debug_mode = 1;
        printf("Debug flag now on\n");
    }
    else{
        debug_mode = 0;
        printf("Debug flag now off\n");
    }
}

void Examine_ELF_File(){
    char fileName[256];
    printf("enter ELF file name\n");
    fgets(fileName,sizeof(fileName),stdin);
    fileName[strcspn(fileName, "\n")] = 0;
    if(debug_mode){
        printf("Debug: file name set to %s\n", fileName);
    }
    if(Currentfd > 0)
        close(Currentfd);
    if( (Currentfd = open(fileName, O_RDWR)) < 0 ) {
        perror("error in open");
        exit(-1);
    }
    if( fstat(Currentfd, &fd_stat) != 0 ) {
        perror("stat failed");
        exit(-1);
    }

    if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, Currentfd, 0)) == MAP_FAILED ) {
        perror("mmap failed");
        close(Currentfd);
        exit(-4);
    }

    if(strncmp("ELF",map_start+1,3) != 0){
        printf("Error: not an ELF file\n");
        munmap(map_start, fd_stat.st_size);
        close(Currentfd);
        Currentfd = -1;
    }

    header = (Elf32_Ehdr *) map_start;

    fwrite(map_start,1,4,stdout);
    char dataEncoding = header->e_ident[EI_DATA];
    if(dataEncoding == 1)
        printf("\nThe data encoding scheme of the object file is 2's complement, little endian\n");
    else
        printf("\nThe data encoding scheme of the object file is 2's complement, big endian\n");
    printf("The entry point is %x\n", header->e_entry);
    printf("The file offset in which the section header table resides is %d\n", header->e_shoff);
    printf("The number of section header entries is %d\n", header->e_shnum);
    printf("The size of each section header entry is %d\n", header->e_shentsize);
    printf("The file offset in which the program header table resides is %d\n", header->e_phoff);
    printf("The number of program header entries is %d\n", header->e_phnum);
    printf("The size of each program header entry is %d\n", header->e_phentsize);

}

void Print_Section_Names(){
    printf("Not implemented yet\n");
}

void Quit(){

    munmap(map_start, fd_stat.st_size);
    if(Currentfd > 0)
        close(Currentfd);
    if(debug_mode){
        printf("quitting\n");
        exit(0);
    }
    exit(0);
}

void Print_Symbols(){

    printf("Not implemented yet\n");

}


struct fun_desc {
    char *name;
    void (*fun)();
};

struct fun_desc menu[] = {
        { "Toggle Debug Mode",Toggle_Debug_Mode },
        { "Examine ELF File",Examine_ELF_File },
        { "Print Section Names",Print_Section_Names },
        { "Print Symbols",Print_Symbols },
        { "Quit", Quit },
        { NULL, NULL } };


int main(int argc, char **argv){

    while (!feof(stdin)){

        printf("Please choose an action:\n");
        size_t menu_length = sizeof(menu)/sizeof(menu[0]);
        int bound = menu_length-2 ;

        for(int i = 0 ; i <= bound; i ++){
            printf("%d- %s\n" ,i, menu[i].name);
        }
        char choice[256];
        fgets(choice,sizeof(choice),stdin);
        int number = atoi (choice);
        if(number < 0 || number > bound)
            exit(0);
        menu[number].fun();
    }
    return 0;
}