#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    }
    return mapped_array;
}

int main(int argc, char **argv){
    char arr1[] = {'H','e','y','!'};
   char* arr2 = map(arr1, 4, my_get);
   printf("%s\n", arr2);
   free(arr2);
    int base_len = 5;
    char arr1[base_len];
    char* arr2 = map(arr1, base_len, my_get);
    char* arr3 = map(arr2, base_len, encrypt);
    char* arr4 = map(arr3, base_len, dprt);
    char* arr5 = map(arr4, base_len, decrypt);
    char* arr6 = map(arr5, base_len, cprt);
    free(arr2);
    free(arr3);
    free(arr4);
    free(arr5);
    free(arr6);


}