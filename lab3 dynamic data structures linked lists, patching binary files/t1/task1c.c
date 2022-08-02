#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;
typedef struct link link;
struct link {
    link *nextVirus;
    virus *vir;
};
struct fun_desc {
    char *name;
    link * (*fun)();
};

void printHex(unsigned char*,int);
void readVirus(virus* , FILE* );
void printVirus(virus* , FILE* );
void list_print(link *, FILE* );
link* list_append(link* , link* );
void list_free(link *);
link* map(link *,link * (*f) (link *));
link * load(link *);
link * quit(link *);
link * print(link *);
void detect_virus(char *, unsigned int , link *);


int main(){
    char c;
    link * head = (link*) malloc(sizeof(link));
    head->vir = NULL;
    head->nextVirus = NULL; //THIS WILL ALWAYS REMAIN NULL, AS NEW MEMBERS ARE ADDED BEHIND IT
    struct fun_desc menu[] = {{"Load signatures",load},{"Print signatures",print},
                              {"Detect viruses",NULL},{"Quit",quit},{NULL,NULL}};
    while(1){
        printf("Please choose a function: \n");
        for(int i = 1; i < 5; i ++){
            printf("%d) %s\n",i ,menu[i - 1].name);
        }
        printf("Options: ");
        c = fgetc(stdin) - 49;
        fgetc(stdin);
        if(c<=4 && c>=0){
            printf("Within bounds\n");
            if(c == 3){         
                //free(map(head,menu[c - 1].fun));
                //head = quit(head);
                quit(head);
                //free(head);
                exit(10);
            }
            else if(c == 2) {
                char buffer[10000] = "";
                char input[50];
                scanf("%s",input);
                fgetc(stdin);
                FILE *in;
                in= fopen(input,"rb");
                if (in != NULL) {
                    fseek(in, 0, SEEK_END);
                    int size = ftell(in);
                    fseek(in, 0, SEEK_SET);
                    fread(buffer, sizeof(char), size, in);
                    detect_virus(buffer, size, head);
                    printf("DONE.\n");
                }
                else printf("empty file\n");
                    
            }
            else{
                head = map(head,menu[(int)c].fun);
                //map(head,menu[c].fun);
                printf("DONE.\n");
            }
        }
        else{
            puts("Not within bounds\n");
            free(head);
            return 0;
        }
    }
    return 0;

}

void  detect_virus(char *buffer, unsigned int size, link *virus_list){
    unsigned int i = 0;
    virus *head = virus_list->vir;
    while(head != NULL){
        while(i <= size && head != NULL){
            if(memcmp(buffer + i,head->sig,head->SigSize) == 0){
                printf("The starting byte location in the suspected file: %d\n", i);
                printf("The virus name: %s\n", virus_list->vir->virusName);
                printf("The size of the virus signature: %d\n", head->SigSize);
                head = virus_list->nextVirus->vir;
                virus_list = virus_list->nextVirus;
                i = 0;
            }
            else if(i == size){
            	head = virus_list->nextVirus->vir;
                virus_list = virus_list->nextVirus;
                i = 0;
            }
            else{
                i++;
            }
        }
       // head = virus_list->nextVirus->vir;
     //   virus_list = virus_list->nextVirus;


    }

}
link * map(link * head ,link * (*f) (link *)){
    return f(head);
}

link * load(link *virusList){
    FILE *in;

    //creating new objcets which will be used to add viruses onto my list

    //toadd->nextVirus = NULL; //ournextvirus has to be NULL since we are the latest member

    long size;
    char input[50];
    scanf("%s" , input);
    fgetc(stdin);
    in = fopen(input ,"rb");
    if(in == NULL){
        printf("%s\n", "empty file");
    }
    else{
        //CHECKING THE FILE SIZE
        fseek(in,0,SEEK_END);
        size = ftell(in);
        fseek(in,0,SEEK_SET);
        long beg = 0;
        while(beg < size){
            //virus* vir = (virus*) malloc(sizeof(virus));
            link * toadd = (link*) malloc(sizeof(link));
            toadd->vir = (virus*) malloc(sizeof(virus));
            readVirus(toadd->vir,in);
            //toadd->vir = vir;
            virusList = list_append(virusList, toadd);
            beg = ftell(in);
            //free(vir);
            //free(toadd);
           // printVirus(vir,stdout);
        }
        fclose(in);
    }

    return virusList;

}


link* print(link *virusList){
    link *current_vir_member = virusList;
    while(current_vir_member->nextVirus != NULL){
//        printVirus(virusList->vir,stdout);
//        virusList->vir = virusList->nextVirus->vir;
//        virusList->nextVirus = virusList->nextVirus->nextVirus;
        printVirus(current_vir_member->vir,stdout);
        current_vir_member = current_vir_member->nextVirus;
    }
    return virusList;
}

link* quit(link *virusList){
//    if(virusList->vir != NULL){
    list_free(virusList);
//    }
    //exit(10);
    return virusList;
}


void printHex(unsigned char* out,int i){
    for(int j = 0; j < i; j++){
        printf("%02hhx ", out[j]);
    }
    printf("\n");

}

void readVirus_tal(virus* vir, FILE* input){
    fread(&vir->SigSize, 2, 1, input);
    vir->sig = (unsigned char*)calloc(vir->SigSize,sizeof(unsigned char ));
    fread(vir->sig,sizeof(char), vir->SigSize, input);
    fread(&vir->virusName,sizeof(char), 16, input);
}

void readVirus(virus* vir, FILE* input)
{
    //THIS IS THE SIGSIZE EXTRACTION
    unsigned short *sig_size_buff = (unsigned short *)calloc(2, sizeof(unsigned short));
    fread(sig_size_buff,sizeof(unsigned char),2,input);
    *sig_size_buff = (sig_size_buff[0] | sig_size_buff[1] << 8); //Big-endian to Little-endian conversion
    vir->SigSize = *sig_size_buff;

    //THIS IS THE SIG EXTRACTION
    unsigned char *sig_buff = (unsigned char *)calloc(vir->SigSize, sizeof(unsigned char));
    fread(sig_buff,sizeof(unsigned char),vir->SigSize,input);
    //memcpy(vir->sig,sig_buff,sizeof(unsigned char));
    vir->sig = (unsigned char *)calloc(vir->SigSize, sizeof(unsigned char));
    memcpy(vir->sig,sig_buff,vir->SigSize);


    //THIS IS THE VIRUSNAME EXTRACTION
    char *virusname_buff = (char *)calloc(16, sizeof(char));
    fread(virusname_buff,sizeof(unsigned char),16,input);
    memcpy(vir->virusName,virusname_buff,16);
//        for(int i=0;i<16;i++)
//        {
//            vir->virusName[i] = virusname_buff[i];
//        }

    //freeing back the memory
    free(sig_size_buff);
    free(virusname_buff);
    free(sig_buff);
    //free(vir->sig);
}

void printVirus_tal(virus* vir, FILE* output){
    fprintf(output,"Virus name: %s\n", vir->virusName);
    fprintf(output,"Virus size: %d\n", vir->SigSize);
    printHex(vir->sig, vir->SigSize);
//    //freeing the signature buffer, doing it here to free it for each virus individually
//    free(vir->sig);
}

void printVirus(virus* vir,FILE* output)
{
    //VIRUS NAME PRINT
    fprintf(output,"Virus name: ");
    for(int i=0;(i<sizeof(vir->virusName)) && (vir->virusName[i] != '\0');i++)
    {
        fprintf(output,"%c",vir->virusName[i]);
    }
    fprintf(output,"\n");

    //VIRUS SIZE PRINT
    fprintf(output,"Virus size: %d\n",vir->SigSize);

    //SIGNATURE PRINT
    fprintf(output,"signature: \n");
    for(int i=0;i<(vir->SigSize);i++)
    {
        fprintf(output,"%02hhX ",vir->sig[i]);
    }
    fprintf(output,"\n\n");

//    //freeing the signature buffer, doing it here to free it for each virus individually
//    free(vir->sig);
}



void list_print(link *virus_list, FILE* output){
    while(virus_list->nextVirus != NULL){
        printVirus(virus_list->vir, output);
        virus_list = virus_list->nextVirus;
    }

}

link* list_append(link* virus_list, link* to_add){
    to_add->nextVirus = virus_list;
    //memcpy(to_add->nextVirus,virus_list,sizeof())
    return to_add;
}

void list_free(link *virus_list){
//    virus * head = virus_list->vir;
    if(virus_list->nextVirus != NULL) {
        list_free(virus_list->nextVirus);
//        free(head->sig);
//        free(head->virusName);
//        free(virus_list->vir);
        free(virus_list->vir->sig);
        free(virus_list->vir);
        free(virus_list);
    }
    else free(virus_list);





}





