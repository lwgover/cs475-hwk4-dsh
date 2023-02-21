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

/**
 * Works like split in python, except not really
 * 
 * @param line - the line you want to split
 * @param splitChar - a character array containing just the character you want to split on
 * @return - a *malloc*ed char** containing all the strings you made
 * ^ make sure to free!
*/
char** split(char* line, char* splitChar){
    int* size = (int*) malloc(sizeof(int));
    char** splits = splitWithSize(line,splitChar,size);
    free(size);
    return splits;
}

/**
 * Works like split in python, except not really
 * 
 * @param line - the line you want to split
 * @param splitChar - a character array containing just the character you want to split on
 * @param size - an int pointer that will be filled with the size of the array
 * @return - a *malloc*ed char** containing all the strings you made
 * ^ make sure to free!
*/
char** splitWithSize(char* line, char* splitChar,int* size){
    int numParts = 1;
    char linecpy[strlen(line)];
    strcpy(linecpy,line);
    strtok(linecpy,splitChar);
    while(strtok(NULL,splitChar)) numParts++;
    *size= numParts;
    char** parts = (char**) calloc(numParts,sizeof(char*));
    parts[0] = strtok(line,splitChar);
    for(int i = 1; i < numParts; i++) parts[i] = strtok(NULL,splitChar);
    return parts;
}
/**
 * Do this if there is a '/' character at the start of the command. 
 * basically just runs the file at the specified path
 * 
 * @param cmdline - command you wanna run
*/
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
            free(args);
            if(doWait) wait(NULL);
        }
    } else {
        printf("\033[31mError: %s not found!\033[0m\n",path);
    }
}

/**
 * Do this if theres no '/' character at the start of the command. 
 * This means we're gonna have to look for it in the PATH env variable.
 * executes the command if possible. 
 * 
 * @param cmdline - command you wanna run
*/
void modeTwo(char* cmdline){
    char* originalCmdline =(char*) malloc(strlen(cmdline)+1);
    strcpy(originalCmdline, cmdline);
    char home[256];
    if (getcwd(home, sizeof(home)) == NULL){
        printf("\033[31mgetcwd() error. Sorry!\033[0m\n");
        return;
    }
    if (access(strtok(cmdline," "), F_OK | X_OK) == 0){
        cmdline[strlen(cmdline)] = ' ';
        modeOne(cmdline);
        return;
    }
    char PATH[strlen(getenv("PATH"))];
    strcpy(PATH,getenv("PATH"));
    int* size = (int*) malloc(sizeof(int));
    char** splitPath = splitWithSize(PATH,":",size);
    char* fullPath = (char*) calloc(MAXBUF*2,sizeof(char));
    for(int i = 0; i < *size; i++){
        strcpy(fullPath,splitPath[i]);
        strcat(fullPath,"/");
        strcat(fullPath,cmdline);
        if(access(fullPath, F_OK | X_OK) == 0){
            strcpy(fullPath,splitPath[i]);
            strcat(fullPath,"/");
            strcat(fullPath,originalCmdline);
            modeOne(fullPath);
            free(size); free(fullPath);free(splitPath);free(originalCmdline);
            return;
        }
    }
    printf("\033[31mError: %s not found!\033[0m\n",cmdline);
    // obligatory free statements
    free(size); free(fullPath);free(splitPath);free(originalCmdline);
}

/**
 * Does DSH
 * 
 * @param cmdline the command line input
*/
void doDSH(char* cmdline){
    char* command = strtok(cmdline, " ");
    if(!strcmp(command,"cd")){
        char* dir = strtok(NULL," ");
        if(strcmp(dir,"~") == 0) dir = getenv("HOME");
        if(chdir(dir) != 0) printf("\033[31m%s: No such file or directory\033[0m\n",dir);
        return;
    }
    char wd[MAXBUF];
    if (getcwd(wd, sizeof(wd)) == NULL) printf("\033[31mgetcwd() error. Sorry!\033[0m\n");
    if(!strcmp(command,"pwd")){
        printf("%s\n",wd);
        return;
    }
    cmdline[strlen(cmdline)] = ' ';
    (cmdline[0] == '/') ? modeOne(cmdline) : modeTwo(cmdline);
}