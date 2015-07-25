#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

void getValue(FILE *f);



int main(int argc, char *argv[]){
	FILE *outFile;	
	FILE *fullFile;
	FILE *chargeFile;
	time_t interval;
	time_t length;
	time_t start;
	time_t now;
	time_t elapsed;
	time_t end;
	struct timespec delay; 

	if (argc != 4){
		printf("Usage: %s output interval length\n", argv[0]);
		return 1;
	}
	interval = strtol(argv[2]);
	length = strtol(argv[3]);
	now = time(NULL);
	start = now;
	end = start + interval;


	return 0;
}

