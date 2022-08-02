#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
	{
	char str[100];
	int i;
	printf("enter the string: \n");
	scanf("%s",str);
	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] >=65 && str[i] <=90)
		{
			str[i] = '.';
		}
	printf("%c", str[i]);
		
	}
	return 0;
	}
