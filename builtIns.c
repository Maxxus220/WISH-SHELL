#include "builtIns.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Methods for all built in shell commands
 */

int shell_cd(char* args[]) {
    return chdir(args[0]);
}

// TODO: Possibly need to handle whether path needs / at end either error or add it automatically
int shell_path(char* args[], int argc, char **path, int *path_size) {
    path = realloc(path, argc * sizeof(char*));
    for(int i = 0; i < argc; i++) {
        path[i] = args[i];
    }
    *path_size = argc;
    return 0;
}

int shell_if(char* args[], int argc) {
    printf("shell_if\n");
    return -1;
}