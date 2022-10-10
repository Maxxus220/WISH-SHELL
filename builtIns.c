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



    // Find index of != or ==
    int numIfs = 1;
    int numHits = 0;
    int i = -1;
    for(int j = 0; j < argc; j++) {
        if(strcmp(args[j],"if") == 0) {
            numIfs++;
        }
        else if(strcmp(args[j],"!=") == 0 || strcmp(args[j],"==") == 0) {
            i = j;
            numHits++;
        }
        if(numIfs == numHits) {
            break;
        }
    }
    if(i == -1) {
            return -1;
        }

    // Determine operator
    if(strcmp(args[i],"!=") == 0) equals = 0;
    else equals = 1;

    // Get first command return
    int firstCommandReturn = -1;
    char* built_args[i];
    for(int j = 1; j < i; j++) {
        built_args[j-1] = malloc(strlen(args[j]) + 1);
        strcpy(built_args[j-1],args[j]);
    }
    if(strcmp(args[0],"if") == 0) {
        firstCommandReturn = shell_if(built_args, i-1, path, path_size);
    }
    else if(strcmp(args[0],"cd") == 0) {
        firstCommandReturn = shell_cd(built_args);
    }
    else if(strcmp(args[0],"path") == 0) {
        firstCommandReturn = shell_path(built_args, i-1, path, path_size);
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
            new_args[i] = NULL;
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

    // Check there is a second command
    if(strcmp(args[i+3],"fi") == 0) {
        return 0;
    }

    // Redirect
    int redirIndex = -1;
    for(int k = 0; k < argc; k++) {
        if(strcmp(args[k], ">") == 0) {
            if(redirIndex != -1 || k == 0) {
                return -1;
            }
            redirIndex = k;
        }
    }

    FILE* out = NULL;
    int outFD;
    if(redirIndex != -1) {
        if(redirIndex != argc-3) {
            return -1;
        }
        
        if((out = fopen(args[argc-2], "w")) == NULL) {
            return -1;
        }
        outFD = fileno(out);
        
        argc -= 2;
        args[argc-1] = NULL;
    }

    // Change stdout
    int save_stdout = 0;
    if(redirIndex != -1) {
        save_stdout = dup(1);
        dup2(outFD, 1);
    }     

    // Run 2nd command
    int returnCode = -1;
    char *built2_args[argc-(i+5)];
    for(int j = 0; j < argc-(i+5); j++) {
        built2_args[j] = malloc(strlen(args[i+4+j]) + 1);
        strcpy(built2_args[j],args[i+4+j]);
    }
    if(strcmp(args[i+3],"if") == 0) {
        returnCode = shell_if(built2_args, argc-(i+5), path, path_size);
    }
    else if(strcmp(args[i+3],"cd") == 0) {
        returnCode = shell_cd(built2_args);
    }
    else if(strcmp(args[i+3],"path") == 0) {
        returnCode = shell_path(built2_args, argc-(i+5), path, path_size);
    }
    else {
        int fork_code = fork();
        int status;
        if(fork_code == -1) {
            return -1;
        }
        else if(fork_code > 0) {
            wait(NULL);
            returnCode = 0;
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
            char* new_args[argc-(i+3)];
            for(int j = 0; j < argc-(i+4); j++) {
                new_args[j] = malloc(strlen(args[i+3+j]) + 1);
                strcpy(new_args[j],args[i+3+j]);
            }
            new_args[argc-(i+4)] = NULL;
            if(execv(file, new_args) == -1) {
                return -1;
            }
        }
    }
    if(redirIndex != -1) {
        dup2(save_stdout, 1);
        close(outFD);
    }
    
    return returnCode;
}