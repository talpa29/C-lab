#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
	{
	char str[];
	int i;
	printf("enter the string: \n");
	scanf("%s",str);
	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] >=97 && str[i] <=122)
		{
			str[i] = '.';
		}
	printf("%s \n", str[i]);
		
	}
	return 0;

	}
