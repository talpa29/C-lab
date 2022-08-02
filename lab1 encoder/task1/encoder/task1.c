#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	FILE *Fpointer;
	char data[3] = "tal";
	int length = strlen(data);
	int i;
	
	Fpointer = fopen("text.txt","w"); // w for write mode
	
	
		for(i = 0; i < length; i ++)
		{
			printf("writing the char %c \n",data[i]);
			fputc(data[i],Fpointer);
		}
	
	return 0;
		
}
