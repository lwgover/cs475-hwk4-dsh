/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>

char** split(char* line, char* splitChar){
    int numGaps = 0;
    int onChar = 0;
    for(int i = 0; i < MAXBUF; i++){
        if(onChar && line[i] != splitChar[0]){
            onChar = 0;
            numGaps++;
        }
        if(line[i] == splitChar[0]){
            onChar = 1;
        }
    }
    char** args = (char**) malloc(numGaps); 
    args[0] = strtok(line, splitChar);
    for(int i = 1; i < numGaps+1; i++){
        args[i] = (char*) malloc(sizeof(char)); 
        args[i] = strtok(NULL,splitChar);
    }
    return args;
}

void modeOne(char* cmdline){
    int doWait = !(cmdline[strlen(cmdline)-1] == '&');
    char** args = split(cmdline," ");
    char* path = args[0];
    args = &args[0];
    if (access(path, F_OK | X_OK) == 0) {
    // File exists and is executable! Can run!
        if (0 == fork()) {
            // child process
            execv(path,args);
        } else { 
            if(doWait) wait(NULL);
        }
    } else {
        printf("\033[31mError: %s not found!\033[0m\n",path);
    }
}
void modeTwo(char* cmdline){
    char path[256];
    if (getcwd(path, sizeof(path)) == NULL){
        printf("\033[31mgetcwd() error. Sorry!\033[0m\n");
        return;
    }
    chdir(path);
    if (access(path, F_OK | X_OK) == 0){
        modeOne(path);
        return;
    }

}

/**
 * Does DSH
 * 
 * @param cmdline the command line input
 * 
 * also look at how beautiful this function is!!!! It's literally 1 line!
*/
void doDSH(char* cmdline){(cmdline[0] == '/') ? modeOne(cmdline) : modeTwo(cmdline);}