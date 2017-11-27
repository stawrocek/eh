#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

char encrypted[] = "zxoolRaxoolRixoRixo";

int checkCharacter(char c, int idx){
	char c1 = c ^ 0xD;
	if(c1 != encrypted[idx])
		return 997;
	return 42;
}


int main(int argc, char* argv[]){
	if(argc < 2){
		printf("./xorme password\n");
		return -1;
	}
	if(strlen(argv[1]) != 19){
		printf("wrong.\n");
		return -1;
	}
	for(int idx = 0;idx < strlen(argv[1]); idx++){
		if(checkCharacter(argv[1][idx], idx) == 997){
			printf("Wrong! I knew you were trouble!\n");
			return -1;
		}
	}
	printf("Welcome aboard! %s\n", argv[1]);
	return 0;
}
