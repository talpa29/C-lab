#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void encoderer(FILE * input, FILE * output);

void encoderB(FILE * input,FILE * output,FILE * err_put){


	char c;
	c = fgetc(input);
	int counter = 0;
	while(c != EOF)
	{
		if(c == '\n')
		{
				char str[] = "the number of letter: ";
				fprintf(err_put,"%s",  str);
				fprintf(err_put, "%d\n", counter);
				fprintf(output, "\n");
				fflush(err_put);
				fflush(output);
			}
		else
		{
			fprintf(err_put, "%d ", c);
			if(c <= 90 && c >= 65)
			{
				c = '.';
				counter++;
			}
			fprintf(err_put,"%d\n", c);
			fprintf(output,"%c", c);
		}
		c = fgetc(input);
	
		
	}

	fprintf(output, "^D\n");
	
}
