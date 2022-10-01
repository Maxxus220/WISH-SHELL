// TODO: Write tests/theory craft tests (try to think blackbox)

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
    int outFD = STDOUT;


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


    // ---MAIN SHELL LOOP---

    while(1) {

        // ---WISH PROMPT---
        if(!batchMode) {
            printf("wish>");
        }


        // ---USER INPUT---

        // Check for EOF
        // TODO: Make test for eof/figure out how to do that
        if(feof(stdin)) {
            exit(0);
        }

        getline(&buffer, &buffer_size, stdin);


        // Tokenize user input
        char* tokens[32];
        char* token;
        int token_count = 0;
        while(token_count < 32 && (token = strsep(&buffer," ")) != NULL && strcmp(token, "\n") != 0) {
            tokens[token_count] = token;
            token_count++;
        }

        // Skip to next prompt if empty line
        if(token_count == 0) continue;

        // Remove new line from tokens
        tokens[token_count-1][strcspn(tokens[token_count-1], "\n")] = 0;


        // ---CHECK FOR REDIRECT/DETERMINE OUTPATH (DEFAULT: stdout)---

        int redirIndex = -1;
        for(int i = 0; i < token_count; i++) {
            if(strcmp(tokens[i], ">") == 0) {
                if(redirIndex != -1 || i == 0) {
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
            
            FILE* out = NULL;
            if((out = fopen(tokens[token_count-1], "w")) == NULL) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                exit(1);
            }
            outFD = fileno(out);

        }


        // ---COMMAND HANDLING---
        char* command = tokens[0];
        int return_code = -1;
        // TODO: Run corresponding command and check for valid command
        
        // Built In Commands
        if(strcmp(command, "exit") == 0) {
            exit(0);
        }
        else if(strcmp(command, "cd") == 0) {
            return_code = shell_cd(&tokens[1]);
        }
        else if(strcmp(command, "path") == 0) {
            return_code = shell_path(&tokens[1]);
        }

        // User Commands
        else {
            // TODO: How to run user commands?
        }
        



        // TODO: Handle code returned by command


        // ---CLEAN-UP LOOP---

        // Reset outFD and close redirect file
        if(redirIndex > -1 && redirIndex == token_count-2) {
            close(outFD);
            outFD = STDOUT;
        }
    }
}