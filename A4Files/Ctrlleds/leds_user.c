#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <ctype.h>

void drive(int filedesc){
  char data[128];
  if(read(filedesc, data, 128) < 0)
     write(2, "An error occurred in the read.\n", 128);
  write(filedesc, "123\n", sizeof(int));
  write(filedesc, "23\n", sizeof(int));
  write(filedesc, "\n", sizeof(int));
}

int main(char* argv) {

	/* Open file */
  int filedesc = open("testfile.txt", O_WRONLY | O_APPEND);
  if(filedesc > 0)
    drive(file);
  
	close(filedesc);
	return 0;
}
