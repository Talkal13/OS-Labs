#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	size_t size;

	if (argc!=3) {
		fprintf(stderr,"Usage: %s <file_name> <block_size>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);
	
	/* Check the block size is a number */
	for (size_t i = 0; i < strlen(argv[2]); i++) {
		if (!isdigit(argv[2][i]))
			err(3, "The block size must be an unsigned integer");
	}
	
	size = atoi(argv[2]);
	char block[size];
	
	/* Read file byte by byte */
	while (!feof(file)) {
		fread(block, size, 1, file);
		/* Print byte to stdout */
		fwrite(block, size, 1, stdout);
	}

	fclose(file);
	return 0;
}
