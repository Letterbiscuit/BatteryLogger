#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>

uint64_t getValue(FILE *f);//using uint64_t just in case someone has a really 
			   //impressive battery
void writeCsv(FILE *outFile, time_t elapsed, uint64_t fullCharge,
	uint64_t currentCharge);


int main(int argc, char *argv[]){
	FILE *outFile;	
	FILE *fullFile;
	uint64_t fullCharge;
	time_t interval = 0;
	time_t length = 0;
	time_t start;
	time_t now;
	time_t elapsed;
	time_t end;
	struct timespec delay;


	if (argc != 4){
		printf("Usage: %s output interval length\n", argv[0]);
		return 2;
	}
	interval = strtol(argv[2], NULL, 10);
	length = strtol(argv[3], NULL, 10);
	if(errno || interval > length || interval == 0 || length == 0){
	//Ensure that the numeric args were ok
		puts("Invalid arguments");
		puts("Note: length must be greater than interval");
	return 2;
	}
	
	now = time(NULL);
	start = now;
	end = start + length;
	delay.tv_nsec = 0;//No values less than a second to be waited
	delay.tv_sec = interval;

	outFile = fopen(argv[1], "w");
	if(!outFile){
		perror("Invalid output file");
		return 2;
	}
	fullFile = fopen("/sys/class/power_supply/BAT0/energy_full", "r");
	if(!fullFile){
		perror("Error opening /sys/class/power_supply/BAT0/\
energy_full");
		return 1;
	}
	fullCharge = getValue(fullFile);

	if(!fullCharge){
		puts("Error: /sys/class/power_supply/BAT0/energy_full read as \
containing value 0");
		fclose(fullFile);
		fclose(outFile);
		return 1;
	}
	fclose(fullFile);//Transitioning to a local version in the while loop


	elapsed = 0;//Set to zero because otherwise is undefined in the first
		    //iteration
	while(difftime(now, end) < 0){
		FILE *currentFile = fopen("/sys/class/power_supply/BAT0/energy\
_now", "r");
		FILE *fullFile = fopen("/sys/class/power_supply/BAT0/energy\
_full", "r");
		uint64_t currentCharge = getValue(currentFile);
		uint64_t fullCharge = getValue(fullFile);
		writeCsv(outFile, elapsed, fullCharge, currentCharge);
		nanosleep(&delay, NULL);
		now = time(NULL);//Update time variables
		elapsed = now - start;
		fclose(currentFile);//Commit changes
		fclose(fullFile);
	}
	fclose(outFile);
	return 0;
}


uint64_t getValue(FILE *f){
	uint64_t value = 0;
	char current = fgetc(f);
	char *readStr = malloc(1);//One for \0
	memset(readStr, '\0', 1);
	while(current != EOF){
		readStr = realloc(readStr, strlen(readStr) + 2);//One for new
								// \0,
								//one for char
		strncat(readStr, &current, 1);
		current = fgetc(f);
	}
	value = strtol(readStr, NULL, 10);
	free(readStr);
	return value;
}

void writeCsv(FILE *outFile, time_t elapsed, uint64_t fullCharge,
	uint64_t currentCharge){

	fprintf(outFile, "%lu,%lu,%lu\n", elapsed, fullCharge, currentCharge);




}
