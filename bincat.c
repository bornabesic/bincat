#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#define true 1
#define false 0

#define MAX_FILES 4096
#define ALIGN "-a"
#define INPUT "-i"
#define OUTPUT "-o"

#define MEMORY_ERROR 77	//error codes
#define SYNTAX_ERROR 83


void end(char string[], int ec){
	puts(string);
	exit(ec);
}

int main(int argc, char* argv[]){
	char info[]=	" /\\_/\\\t-------------------------------------------------\n"
					"( o.o )\t|\tBINCAT v1.0 - Written by Borna Besic\t|\n"
					" > ^ <\t-------------------------------------------------\n";

	char syntax[]=	"\nOptions:\n"
					" -a <block size>\n"
					"\tAligns every input file so it starts at the byte divisible by the block size.\n"
					"\tThe remaining empty space within the block will be zero-filled.\n"
					"\tIt offers the ability to write different parts of a raw code to different sectors of the disk.\n"
					"\tCommonly used option while creating a disk image.\n"
					"\n"
					" -i <filename>\n"
					"\tSpecifies the input file. Can be used multiple times.\n"
					"\tInput files will be concatenated in order they are specified.\n"
					"\tExample: BINCAT -i file1.bin -i file2.bin ... -o result.flp\n"
					"\n"
					" -o <filename>\n"
					"\tSpecifies the name of the output file which will be created.";

	puts(info);

	FILE *in[MAX_FILES], *out=NULL;
	int i, file_num=0, bytes_read, BUFFER_SIZE=512, STANDARD = true;
	
	for(i=1; i<argc-1; i++){
		if(!strcmp(argv[i], ALIGN)){
			BUFFER_SIZE=atoi(argv[++i]);
			STANDARD=false;
		}
		else if(!strcmp(argv[i], INPUT)){
			if((in[file_num]=fopen(argv[++i], "rb"))==NULL) printf("[ %s ] The input file could not be read.\n", argv[i]);
			else file_num++;
		}
		else if(!strcmp(argv[i], OUTPUT)){
			if((out=fopen(argv[++i], "wb"))==NULL) printf("[ %s ] The output file could not be created.\n", argv[i]);
		}
	}


	if(file_num>0 && out!=NULL){
		if(BUFFER_SIZE<=0 || BUFFER_SIZE==INT_MIN || BUFFER_SIZE==INT_MAX){
				puts("WARNING: Invalid alignment. Standard concatenation (no alignment) will be used.");
				BUFFER_SIZE=512;
				STANDARD=true;
		}

		void *BUFFER=malloc(BUFFER_SIZE);
		if(BUFFER==NULL) end("Unable to allocate the buffer memory.", MEMORY_ERROR);
		memset(BUFFER, 0, BUFFER_SIZE);

		for(i=0; i<file_num; i++){
			while((bytes_read=fread(BUFFER, 1, BUFFER_SIZE, in[i]))>0) {
				fwrite(BUFFER, STANDARD?bytes_read:BUFFER_SIZE, 1, out);
				memset(BUFFER, 0, BUFFER_SIZE);
			}
			fclose(in[i]);
		}
		fclose(out);
	}
	else end(syntax, SYNTAX_ERROR);

	printf("%d file(s) successfully %s.", file_num, file_num==1?"copied":"concatenated");

	return 0;
}
