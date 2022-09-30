// ---INCLUDES---

#include "builtIns.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// ---DEFINES---

#define DEBUG 1


// ---SHELL ENTRY---

int main(int argc, char *argv[]) {

    // ---CONSTS---

    const char ERROR_MESSAGE[30] = "An error has occurred\n";
    const int STDOUT = 1;


    // ---SHELL STATE VARIABLES---

    int batchMode = 0;


    // ---SHELL INPUT SETUP---

    size_t buffer_size = 32;
    char* buffer = malloc(buffer_size * sizeof(char));
    if(buffer == NULL) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(1);
    }


    // ---READ SOURCE SETUP---

    // Batch file potentially provided
    if(argc == 2) {
        // Check for valid batch file
        if(freopen(argv[1], "r", stdin) != NULL) {
            batchMode = 1;
        }
        else {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
            exit(1);
        }
    }
    else if(argc > 2) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(1);
    }

    // if(DEBUG) {
    //     size_t size = 10;
    //     char* str = malloc(size);
    //     getline(&str,&size,stdin);
    //     printf("%s\n",str);
    // }


    // ---MAIN SHELL LOOP---

    while(1) {

        // ---WISH PROMPT---
        if(!batchMode) {
            printf("wish>");
        }


        // ---USER INPUT---

        // Check for EOF
        if(feof(stdin)) {
            exit(0);
        }

        getline(&buffer, &buffer_size, stdin);
        

        // Tokenize user input
        char* tokens[32];
        char* token;
        int token_count = 0;
        while(token_count < 32 && (token = strsep(&buffer," ")) != NULL) {
            tokens[token_count] = token;
            token_count++;
        }


        // ---CHECK FOR REDIRECT/DETERMINE OUTPATH (DEFAULT: stdout)---
        // TODO: Need to reset stdout after each loop
        int redirIndex = -1;
        for(int i = 0; i < token_count; i++) {
            if(strcmp(tokens[i], ">") == 0) {
                if(redirIndex != -1) {
                    write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                    exit(1);
                }
                redirIndex = i;
            }
        }

        if(redirIndex != -1) {
            if(redirIndex != token_count-2) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                exit(1);
            }
            if(freopen(tokens[token_count-1],"w",stdout) == NULL) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                exit(1);
            }
        }

        if(DEBUG) {
            for(int i = 0; i < token_count; i++) {
                printf("%s\n",tokens[i]);
            }
        }

        // TODO: Run corresponding command and check for valid command
            // TODO: Check for empty command

            // ---BUILT-IN COMMANDS---

                // TODO: exit command

                // TODO: cd command

                // TODO: path command

            // ---USER COMMANDS---


        // TODO: Handle code returned by command

        // if(DEBUG) {
        //     exit(0);
        // }
    }
}