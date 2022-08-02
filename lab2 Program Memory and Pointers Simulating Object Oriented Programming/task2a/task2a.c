#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char censor(char c) {
    if(c == '!')
        return '*';
    else
        return c;
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
    char* arr2 = map(arr1, 4, censor);
    printf("%s\n", arr2);
    free(arr2);
}