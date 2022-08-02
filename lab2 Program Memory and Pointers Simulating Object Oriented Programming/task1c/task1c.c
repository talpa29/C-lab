#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main (){
 	int iarray[3] = {1,2,3};
	float farray[3] = {1,2,3};
	double darray[3] = {1,2,3};
	char carray[3] = {'1','2','3'};
	printf("%p\n", &iarray[0]);
	printf("%p\n", &iarray[1]);
	printf("%p\n", &iarray[2]);
	printf("%p\n", &iarray[0]);
	printf("%p\n", &farray[0]);
	printf("%p\n", &darray[0]);
	printf("%p\n", &darray[1]);
	printf("%p\n", &carray[0]);
	printf("%p\n", &carray[1]);

	return 0;
}


