#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct fun_desc {
    char *name;
    char (*fun)(char);
};

char censor(char c);
char encrypt(char c);
char decrypt(char c);
char dprt(char c);
char cprt(char c);
char my_get(char c);
char quit(char c);
char* map(char *array, int array_length, char (*f) (char));


int main(){
    char c;
    int base_len = 5;
    char* carray = NULL;
    int numberOfChars = 5;
    carray = (char*)calloc(numberOfChars + 1, sizeof(char));
    struct fun_desc menu[] = {{"cencor",censor},{"Encrypt",encrypt},
    			      {"Decrypt",decrypt},{"Print dec",dprt},
    			      {"Print string",cprt},{"Get string",my_get},
    			      {"Quit",quit},{NULL,NULL}};
    
    
    while(1 != 0){
        printf("Please choose a function: \n");
        for(int i = 0; i < 7; i ++){
            printf("%d) %s\n",i ,menu[i].name);
        }
        printf("Options: ");
        c = fgetc(stdin);
        int h = c - 48;
        fgetc(stdin);
        if(h<=6 && h>=0){
            printf("Within bounds\n");
            char* carray1= map(carray, base_len, menu[h].fun);
            carray = carray1;
            printf("DONE.\n");
        }
        else{
            puts("Not within bounds\n");
            free(carray);
            return 0;
        }
    }

    return 0;
}


char censor(char c) {
    if(c == '!')
        return '*';
    else
        return c;
}

char encrypt(char c) {
    if((c >=41 && c <= 122)){
        c = c + 2;
    }
    return c;
}

char decrypt(char c){
    if((c >=41 && c <= 122)){
        c = c - 2;
    }
    return c;
}

char dprt(char c){
    printf("%d\n", c);
    return c;
}

char cprt(char c){
    if(c >= 41 && c <= 122){
        printf("%c\n", c);
    }
    else{
        printf("%c\n", '*');
    }

    return c;
}

char my_get(char c){
    char a = fgetc(stdin);
    return a;
}


char* map(char *array, int array_length, char (*f) (char)){
    char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
    for(int j = 0; j < array_length; j++){
        *(mapped_array + j) = f(*(array + j));
        if(*(mapped_array + j) == '\n'){
        	j = array_length;
        }
    }
    return mapped_array;
}

char quit(char c){
    if(c == 113){
        exit(0);
    }
    else{
        return c;
    }
}




