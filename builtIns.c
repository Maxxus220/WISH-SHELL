#include "builtIns.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**
 * Methods for all built in shell commands
 */

int shell_cd(char* args[]) {
    return chdir(args[0]);
}

int shell_path(char* args[], int argc, char **path, int *path_size) {
    path = realloc(path, argc);
    for(int i = 0; i < argc; i++) {
        path[i] = malloc(strlen(args[i] + 1));
        strcpy(path[i], args[i]);
        strcat(path[i], "/");
    }
    *path_size = argc;
    return 0;
}

int shell_if(char* args[], int argc) {
    if(strcmp(args[argc-1],"fi") != 0) {
        return -1;
    }
    int i = 1;
    while(strcmp(args[i],"!=") != 0 && strcmp(args[i],"==") != 0) {
        i++;
    }
    if(strcmp(args[0],"if") == 0) {
        char* new_args[i] = {0};
        for(int j = 1; j < i; j++) {
            new_args[j-1] = malloc(strlen(args[j]));
            strcpy(new_args[j-1],args[j]);
        }
        shell_if(new_args, j);
    }
    return -1;
}