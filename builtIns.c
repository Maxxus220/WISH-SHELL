#include "builtIns.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

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

int shell_if(char* args[], int argc, char **path, int *path_size) {
    if(strcmp(args[argc-1],"fi") != 0) {
        return -1;
    }
    int equals = -1;

    // Find index of != or == TODO: Change to for loop to avoid segfault on bad input or just check for fi
    int i = 1;
    while(strcmp(args[i],"!=") != 0 && strcmp(args[i],"==") != 0) {
        i++;
    }

    // Determine operator
    if(strcmp(args[i],"!=") == 0) equals = 0;
    else equals = 1;

    // Get first command return
    int firstCommandReturn = -1;
    char* built_args[i];
    for(int j = 1; j < i; j++) {
        built_args[j-1] = malloc(strlen(args[j]));
        strcpy(built_args[j-1],args[j]);
    }
    if(strcmp(args[0],"if") == 0) {
        firstCommandReturn = shell_if(built_args, i, path, path_size);
    }
    else if(strcmp(args[0],"cd") == 0) {
        firstCommandReturn = shell_cd(built_args);
    }
    else if(strcmp(args[0],"path") == 0) {
        firstCommandReturn = shell_path(built_args, i, path, path_size);
    }
    // else if(strcmp(args[0],"exit") == 0) {
    //     exit(0);
    // }
    else {
        int fork_code = fork();
        int status;
        if(fork_code == -1) {
            return -1;
        }
        else if(fork_code > 0) {
            waitpid(fork_code, &status, 0);
            if(WIFEXITED(status)) {
                firstCommandReturn = WEXITSTATUS(status);
            }
        }
        else if(fork_code == 0) {

            // Check path for command
            char file[255];
            memset(file,0,sizeof(file));
            for(int k = 0; k < *path_size; k++) {
                strcat(file, path[k]);
                strcat(file, args[0]);
                if(access(file,X_OK) == 0) {
                    break;
                }
                memset(file,0,sizeof(file));
            }

            // User command not in path
            if(file[0] == '\0') {
                return -1;
            }

            // Create argv
            char* new_args[i+1];
            for(int j = 0; j < i; j++) {
                new_args[j] = malloc(strlen(args[j]));
                strcpy(new_args[j],args[j]);
            }
            new_args[i] = "\0";
            if(execv(file, new_args) == -1) {
                return -1;
            }
        }
    }

    // Get constant
    for(int j = 0; j < strlen(args[i+1]); j++) {
        if(!isdigit(args[i+1][j])) return -1;
    }
    int constant = atoi(args[i+1]);

    // Compare
    if(equals) {
        if(firstCommandReturn != constant) {
            return 0;
        }
    }
    else {
        if(firstCommandReturn == constant) {
            return 0;
        }
    }

    // Check for then
    if(strcmp(args[i+2],"then") != 0) {
        return -1;
    }

    // Run 2nd command
    built_args[argc-(i+2)];
    for(int j = 0; j < argc-(i+2); j++) {
        built_args[j] = malloc(strlen(args[i+4+j]));
        strcpy(built_args[j],args[i+4+j]);
    }
    if(strcmp(args[i+3],"if") == 0) {
        shell_if(built_args, argc-(i+2), path, path_size);
    }
    else if(strcmp(args[i+3],"cd") == 0) {
        shell_cd(built_args);
    }
    else if(strcmp(args[i+3],"path") == 0) {
        shell_path(built_args, argc-(i+2), path, path_size);
    }
    else {
        int fork_code = fork();
        int status;
        if(fork_code == -1) {
            return -1;
        }
        else if(fork_code > 0) {
            wait(NULL);
            return 0;
        }
        else if(fork_code == 0) {

            // Check path for command
            char file[255];
            memset(file,0,sizeof(file));
            for(int k = 0; k < *path_size; k++) {
                strcat(file, path[k]);
                strcat(file, args[i+3]);
                if(access(file,X_OK) == 0) {
                    break;
                }
                memset(file,0,sizeof(file));
            }

            // User command not in path
            if(file[0] == '\0') {
                return -1;
            }

            // Create argv
            char* new_args[argc-(i+2)+1];
            for(int j = 0; j < argc-(i+2); j++) {
                new_args[j] = malloc(strlen(args[i+3+j]));
                strcpy(new_args[j],args[i+3+j]);
            }
            new_args[argc-(i+2)] = "\0";
            if(execv(file, new_args) == -1) {
                return -1;
            }
        }
    }
    
    return -1;
}