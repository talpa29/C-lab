#include<stdio.h>
#include<stdlib.h>

void encoderer(FILE * input, FILE * output)
{
	char c = fgetc(input);
	while(c != EOF)
	{
		if(c == '\n')
			{
				fprintf(output, "\n");
				fflush(output);
			}
		else
		{
			if(c <= 90 && c >= 65)
			{
				c = '.';
			}
			fprintf(output,"%c", c);
		}
		c = fgetc(input);
				
	}	
}
