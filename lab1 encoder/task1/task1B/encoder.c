#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void encoderer(FILE * input, FILE * output);

int main(int argc, char** argv){
	FILE * input = stdin;
	FILE * output = stdout;
	FILE * err_put = stderr;
	int i;
	char c;
	char flag ='x';
	for(i = 0; i<argc; i++){
		if(strcmp(argv[i],"-D") == 0){
			flag = 'D';
		}
	}	
	if(flag == 'D' ){
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
		
	}
	else{
		encoderer(input, output);
	}
	fprintf(output, "^D\n");
	fclose(output);
	fclose(input);	
	fclose(err_put);
	return 0;
}
