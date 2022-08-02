#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void encoderer(FILE * input, FILE * output);

int main(int argc, char** argv){
	FILE * input = stdin;
	FILE * output = stdout;
	int i;
	char c;
	char str[3] = "";
	int counter;
	char minplus;
	char toAdd;
	int comp;
	if(sizeof(argv) >= 1){
		char * key = argv[1];
		strncpy(str,key,3);
		minplus = str[0];
		counter = str[2];
		if(counter >= 58){
			i = counter - 55;
		}
		else{
			i = counter - 48;
		}
		comp = i;
		// we gut in i the number of repititions and in minusplus if + or -
		if(str[1] =='e'){
			c = fgetc(input);
			int numOfLet = 0;
			toAdd = c;
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
		else{//standata input
			encoderer(input, output);
		}		

	}
	else{ //standard encoder
		encoderer(input, output);
	}
	fprintf(output, "^D\n");
	fclose(output);
	fclose(input);	
	return 0;
	 
}
