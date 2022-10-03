#include "builtIns.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Methods for all built in shell commands
 */

int shell_exit(char* args[]) {
    
    exit(0);
}

int shell_cd(char* args[]) {

    return chdir(args[1]);
}

int shell_path(char* args[]) {
    printf("shell_path\n");
    return -1;
}

int shell_if(char* args[]) {
    printf("shell_if\n");
    return -1;
}