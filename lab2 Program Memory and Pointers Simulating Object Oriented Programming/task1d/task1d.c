#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main (){
    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr = &iarray[0];
    char* carrayPtr = &carray[0];

    printf("%p\n", &iarray[0]);
    printf("%p\n", &iarray[1]);
    printf("%p\n", &iarray[2]);
    printf("%p\n", &carray[0]);
    printf("%p\n", &carray[0]);
    printf("%p\n", &carray[1]);
    printf("%p\n", &carray[2]);
    printf("%p\n", &carray[0]);
    printf("%c\n", *(carrayPtr));
    printf("%c\n", *(carrayPtr + 1));


    return 0;
}


