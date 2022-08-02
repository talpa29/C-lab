#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void encoderer(FILE * input, FILE * output);

void encoderBC(FILE * input,FILE * output,FILE * output1,FILE * err_put,char minplus,int comp){

	char c;
	c = fgetc(input);
	int counter = 0;
	int numOfLet = 0;
	char toAdd = c;
	int i = comp;
	while(c != EOF)
	{
		if(c == '\n')
		{
			if(numOfLet == 0){
				fprintf(output1, "%s\n" , "-NONE-");
			}
			else{
				if(minplus == '+'){
					for(int j = 0; j<i; j++){
						fprintf(output1, "%c" , toAdd);
					}
				}
				char str[] = "the number of letter: ";
				fprintf(err_put,"%s",  str);
				fprintf(err_put, "%d\n", counter);
				fprintf(output, "\n");
				fflush(err_put);
				fflush(output);
				fprintf(output1, "\n");
				fflush(output1);
			}
			numOfLet = 0;
				
			}
		else
		{
			numOfLet = numOfLet + 1;
			if(minplus == '-'){
				for(int j = 0; j < i && comp > 0; j++){	
					if(c == '\n' && comp > 0){
						fprintf(output1, "%s\n","-NONE-");
						comp = -1;
					}
					else{
						c = fgetc(input);
						comp--;
						fprintf(err_put, "%d ", c);
						if(c <= 90 && c >= 65){
							c = '.';
							counter++;
						}
						fprintf(err_put,"%d\n", c);
						fprintf(output,"%c", c);
					}
				}
				if(c == '\n'&& comp == 0){
					fprintf(output1, "%s\n","-NONE-");
				}
			}
			else{
				fprintf(err_put, "%d ", c);
				if(c <= 90 && c >= 65)
					{
					c = '.';
					counter++;
					}
				fprintf(err_put,"%d\n", c);
				fprintf(output,"%c", c);
			}
			fprintf(output1,"%c", c);
			
		}
		if(c == '\n'){
			c = fgetc(input);
			toAdd = c;
			comp = i;
		}
		else{
			c = fgetc(input);
		}
			
	}

	
}
