#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void encoderer(FILE * input, FILE * output);

void encoderC(FILE * input, FILE * output, char minplus, int comp){

	char c = fgetc(input);
	int numOfLet = 0;
	char toAdd = c;
	int i = comp;
	while(c != EOF){
				if(c == '\n')
				{
					if(numOfLet == 0){
						fprintf(output, "%s\n" , "-NONE-");
					}
					else{
						if(minplus == '+')
						{
							for(int j = 0; j<i; j++){
								fprintf(output, "%c" , toAdd);
							}
						}
						fprintf(output, "\n");
						fflush(output);
					}
					numOfLet = 0;
					
				}	
				else	
				{
				numOfLet = numOfLet + 1;
					if(minplus == '-')
					{
						for(int j = 0; j < i && comp > 0; j++){	
							if(c == '\n' && comp > 0){
								fprintf(output, "%s\n","-NONE-");
								comp = -1;
							}
							else{
								c = fgetc(input);
								comp--;
							}
						}
						if(c == '\n'&& comp == 0){
								fprintf(output, "%s\n","-NONE-");
							}
					}
					fprintf(output,"%c", c);
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
