#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
	FILE * output = stdout;
	FILE * input = stdin;	
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
	fprintf(output, "^D\n");
	fclose(output);
	fclose(input);
	return 0;	
}
