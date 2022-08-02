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
	FILE *writeTO;
	FILE *readFrom;
	int i;
	int iflag = 0;
	int oflag = 0;
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
			if(key[1] =='o' && key[0] =='-'){
				oflag = 1;
				char file[strlen(argv[i] - 1)];
				for(int j = 2; j < strlen(argv[i]);j++){
					file[j-2] = argv[i][j];
				}
				file[strlen(argv[i])] = '.';
				file[strlen(argv[i])+1] = 't';
				file[strlen(argv[i])+2] = 'x';
				file[strlen(argv[i])+3] = 't';
				writeTO = fopen(file,"w"); // w for read mode
				if(file == NULL){
					fprintf(err_put,"no file \n");
				}
					
			}
			else if(key[1] =='i' && key[0] =='-'){
				iflag = 1;
				char file[strlen(argv[i] - 1)];
				for(int j = 2; j < strlen(argv[i]);j++){
					file[j-2]=key[j];
				}
				readFrom = fopen(file,"r"); // r for read mode
				if(file == NULL){
					fprintf(err_put,"no file \n");
				}	
			}
			else if(key[1] =='D' && key[0] =='-'){
				dflag = 1;
			}
			else if(key[1] =='e' && (key[0] =='-' || key[0] =='+')){
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
		if(oflag == 1){
			if(eflag == 1){
				encoderC(input,writeTO,minplus,count);
			}
			else if(dflag == 1){
				encoderB(input,writeTO,err_put);
			}
			else{
				encoderer(input,writeTO);
			}
			fclose(writeTO);
			fprintf(output, "^D\n");
		}
		else if(iflag == 2){
			if(eflag == 1){
				encoderC(readFrom,output,minplus,count);
			}
			else if(dflag == 1){
				encoderB(readFrom,output,err_put);
			}
			else{
				encoderer(readFrom,output);
			}
			fclose(writeTO);
			fclose(readFrom);
			fprintf(output, "^D\n");
		}
		else if(iflag == 1 && oflag == 1){
			if(eflag == 1){
				encoderC(readFrom,writeTO,minplus,count);
			}
			else if(dflag == 1){
				encoderB(readFrom,writeTO,err_put);
			}
			else{
				encoderer(readFrom,writeTO);
			}
			fclose(writeTO);
			fclose(readFrom);
			fprintf(output, "^D\n");
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
