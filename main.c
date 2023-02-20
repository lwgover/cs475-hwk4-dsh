/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"
#include "draw.h"

//%%%%%%%%%%%%%% Helper Functions %%%%%%%%%%%%%%%%%%%%%%%
void moveNforewards(char* input, int n){
	for(int i = 0; i < strlen(input) - n; i++){
		input[i] = input[i+n];
	}
}

void cleanUpInput(char* input){
	int numSpaces = 0;
	for(int i = 0; i < strlen(input); i++){
		if(input[i] == ' ') numSpaces++;
		if(input[i] != ' '){
			moveNforewards(input,numSpaces);
			break;
		}
	}
	for(int i = 0; i < strlen(input); i++){
		if(input[i] == '\n') input[i] = ""[0];
	}
	for(int i = strlen(input)-1; i >=0; i--){
		if(input[i] == ' ' || input[i] == ""[0]){input[i] = ""[0];}else{break;}
	}
}

//%%%%%%%%%%%%%%% The Real Stuff %%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char **argv){
	char *line = (char*) malloc(MAXBUF); // stores user input from command line
	while(strcmp(line,"exit")){
		printf("dsh> ");
		fgets(line, MAXBUF, stdin);  // reads up to 256 characters into the buffer
		cleanUpInput(line);
		if(!strcmp(line,"exit")){break;}
		doDSH(line);
	}
	free(line);
	draw("david.txt");
	return 0;
}
