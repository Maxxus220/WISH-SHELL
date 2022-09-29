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


    // ---SHELL READ BUFFER SETUP---

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

    if(DEBUG) {
        size_t size = 10;
        char* str = malloc(size);
        getline(&str,&size,stdin);
        printf("%s\n",str);
    }


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
        

        // TODO: Tokenize user input


        // TODO: Determine outpath (default: stdout)


        // TODO: Run corresponding command and check for valid command
            // TODO: Check for empty command

            // ---BUILT-IN COMMANDS---

                // TODO: exit command

                // TODO: cd command

                // TODO: path command

            // ---USER COMMANDS---


        // TODO: Handle code returned by command

        if(DEBUG) {
            exit(0);
        }
    }
}