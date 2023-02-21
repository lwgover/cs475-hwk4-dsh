/*
 * dsh.h
 *
 *  Created on: feb 20, 2022
 *      Author: Lucas Gover
 */

#define MAXBUF 256

void doDSH(char* path);
char** split(char* line,char* splitToken);
char** splitWithSize(char* line,char* splitToken,int* size);