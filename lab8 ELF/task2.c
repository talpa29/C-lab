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

char* indexToSectionIndex(int index){

    static char* formats[] = {"ABS", "UND", "LOPROC", "HIPROC","LOOS","HIOS","COMMON","XINDEX","UNKNOWN"};
    if(index == 0Xfff1 )
        return formats[0];
    if(index == 0)
        return formats[1];
    if(index == 0Xff00 )
        return formats[2];
    if(index == 0Xff1f )
        return formats[3];
    if(index == 0Xff20 )
        return formats[4];
    if(index == 0Xff3f )
        return formats[5];
    if(index == 0Xfff2 )
        return formats[6];
    if(index == 0Xffff )
        return formats[7];
    return formats[8];

}

char* index_to_type(int index) {
    static char* types[] = {"NULL", "PROGBITS", "SYMTAB", "STRTAB","RELA","HASH","DYNAMIC","NOTE","NOBITS","REL","SHLIB","DYNSYM","VERNEED","VERSYM"};
    if(index > 11){
        if(index == 0x6ffffffe)
            return types[12] ;
        if(index == 0x6fffffff)
            return types[13] ;
    }
    return types[index];

} 


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
    printf("Please specify an ELF  file name\n");
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


    Elf32_Shdr* section_headers  = map_start + header->e_shoff;
    int sh_string_table_index = header->e_shstrndx;
    section_headers = section_headers + sh_string_table_index;
    int sh_string_table_index_off = section_headers->sh_offset;
    int numOfsections = header->e_shnum;

    if(debug_mode) {
        printf("%d\n", sh_string_table_index_off);
    }

    printf("[index]\tsection_name\t\tsection_address\t\tsection_offset\tsection_size\tsection_type\n");
    section_headers = section_headers - sh_string_table_index;
    char* str_table_sh = map_start + sh_string_table_index_off;
    for(int i = 0; i < numOfsections; i++){
        char* type = index_to_type((int)section_headers->sh_type);
        char* sh_name = str_table_sh + section_headers->sh_name;
        printf("[%d]\t%-25s%08x\t\t%06x\t\t%06x\t\t%s\n",i,sh_name,section_headers->sh_addr,section_headers->sh_offset,section_headers->sh_size,type);
        section_headers++;
    }
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

    Elf32_Shdr* section_header  = map_start + header->e_shoff;
    //get the sh str table for later using the names
    int sh_string_index = header->e_shstrndx;
    section_header = section_header + sh_string_index;
    int shstrtab_off = section_header->sh_offset;


    //go back to start of the sh
    int numberOfSections = header->e_shnum;
    section_header = section_header - sh_string_index;
    Elf32_Shdr* strtab;

    //find the section of symtable and the symbol string table, which is just after the symtable
    for(int i = 0; i < numberOfSections ; i++){
        if(section_header->sh_type == SHT_SYMTAB){
            strtab = section_header + 1;
            break;
        }
        section_header++;
    }   
    int symtab_off = section_header->sh_offset;
    int strtab_off = strtab->sh_offset; 


    //start printing

    printf("[index]\tValue\t\tsection_index\t\tsection_name\tsymbol_name\n");

    //get the symtable offset
    Elf32_Sym* symbol_table = map_start + symtab_off;

    //each symbol is 16 bytes so we devide the size by 16
    int x = section_header->sh_size;
    int numberOfSymbols = x / 16;
    char* sh_name;
    for(int i = 0; i<numberOfSymbols; i++){
        //get the name of the symbole table -> go to the string table and add the index of the name
        char* st_name = map_start + strtab_off + symbol_table->st_name;

        //get the section header index of the name
        int sh_index = symbol_table->st_shndx;

        if(debug_mode) {
            int symb_size = symbol_table->st_size;
            printf("%d ",symb_size);
            printf("%d ", symbol_table->st_name);
            printf("%d ", sh_index);
        }

        //here we need to check if its a special section header
        if(sh_index > numberOfSections || sh_index == 0){
            sh_name = "special";
            char* section_index = indexToSectionIndex(sh_index);
            printf("[%d]\t%08x\t\t%s\t\t%-25s%-25s\n",i,symbol_table->st_value,section_index,sh_name,st_name);
        }
        else{
            //else we calculate normally the name -> go to str sh table and add the index

            Elf32_Shdr* section_entry = map_start + header->e_shoff ;
            section_entry = section_entry + sh_index;
            sh_name = map_start + section_entry->sh_name + shstrtab_off;
            printf("[%d]\t%08x\t\t%d\t\t%-25s%-25s\n",i,symbol_table->st_value,symbol_table->st_shndx,sh_name,st_name);
        }
        symbol_table++;
    }

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