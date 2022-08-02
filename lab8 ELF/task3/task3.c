#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>
#include <unistd.h>

int Currentfd = -1;

void *map_start;            /* will point to the start of the memory mapped file */
struct stat fd_stat;        /* this is needed to  the size of the file */
Elf32_Ehdr *header;         /* this will point to the header structure */
int num_of_section_headers;

int main(int argc, char **argv){

    char fileName[256];
    printf("Please specify an ELF  file name\n");
    fgets(fileName,sizeof(fileName),stdin);
    fileName[strcspn(fileName, "\n")] = 0;
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
    //initialize
    header = (Elf32_Ehdr *) map_start;
    Elf32_Shdr* section_header  = map_start + header->e_shoff;
    int numberOfSections = header->e_shnum;
    Elf32_Shdr* strtab;

    //find sytable
    for(int i = 0; i < numberOfSections ; i++){
        if(section_header->sh_type == SHT_SYMTAB){
            strtab = section_header + 1;
            break;
        }
        section_header++;
    }

    int symtab_off = section_header->sh_offset;
    int strtab_off = strtab->sh_offset;

    Elf32_Sym* symbol_table = map_start + symtab_off;

    int x = section_header->sh_size;
    int numberOfSymbols = x / 16;

    //initialize again the sh offset
    section_header = map_start + header->e_shoff;

    //start the search for main
    for(int i = 0; i<numberOfSymbols; i++){

        //get the name of the symbole table -> go to the string table and add the index of the name
        char* st_name = map_start + strtab_off + symbol_table->st_name;
        char formatted_symbole_name[1024];
        snprintf(formatted_symbole_name, sizeof(formatted_symbole_name), "%-25s", st_name);

        //found the main
        if(strncmp("main",formatted_symbole_name,4) == 0) {

            //get the section header index of the symbole
            int sh_index = symbol_table->st_shndx;
            int symbole_size = symbol_table->st_size;
            int symbole_adress = symbol_table->st_value;
            Elf32_Shdr* relevent_section = section_header + sh_index;
            int section_header_adress = relevent_section->sh_addr;
            int func_offset = relevent_section->sh_offset + symbole_adress - section_header_adress;
            FILE *out;
            out = fopen("test.txt","wb");
            fwrite(map_start + func_offset, 1, symbole_size , out);

        }

        symbol_table++;
    }




    return 0;
}