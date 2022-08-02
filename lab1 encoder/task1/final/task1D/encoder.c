#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void encoderer(FILE * input, FILE * output);
void encoderB(FILE * input,FILE * output,FILE * err_put);
void encoderC(FILE * input, FILE * output, char minplus, int comp);

int main(int argc,char** argv){
	FILE * input = stdin;
	FILE * output = stdout;
	FILE * err_put = stderr;
	FILE *fPointer;
	int i;
	int iflag = 0;
	int dflag = 0;
	int eflag = 0;
	char key[150];
	char minplus;
	int count;
	if(sizeof(argv) <= 1){
		encoderer(input, output);
	}
	else{
		for(i=1;i<argc;i++){
			strncpy(key,argv[i],strlen(argv[i]));
			if(key[1] =='i'){
				iflag = 1;
				char file[strlen(argv[i] - 1)];
				for(int j = 2; j < strlen(argv[i]);j++){
					file[j-2]=key[j];
				}
				fPointer = fopen(file,"r"); // r for read mode
				if(file == NULL){
					fprintf(err_put,"no file \n");
				}	
			}
			else if(key[1] =='D'){
				dflag = 1;
			}
			else if(key[1] =='e'){
				eflag = 1;
				minplus = key[0];
				count = key[2];
				if(count >= 58){
					count = count - 55;
				}
				else{
					count = count - 48;
				}
				
			}
		}
		if(iflag == 1){
			if(eflag == 1){
				encoderC(fPointer,output,minplus,count);
			}
			else if(dflag == 1){
				encoderB(fPointer,output,err_put);
			}
			else{
				encoderer(fPointer,output);
			}
			fclose(fPointer);
		}
		else{
			if(eflag == 1){
				encoderC(input,output,minplus,count);
			}
			else if(dflag == 1){
				encoderB(input,output,err_put);
			}
			else{
				encoderer(input,output);
			}
			fprintf(output, "^D\n");
		}
		
	}
			
	fclose(output);
	fclose(input);	
	fclose(err_put);	
	return 0;
		
}
