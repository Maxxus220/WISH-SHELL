// TODO: Write tests/theory craft tests (try to think blackbox)

// ---INCLUDES---

#include "builtIns.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


// ---DEFINES---

#define DEBUG 1


// ---SHELL ENTRY---

int main(int argc, char *argv[]) {

    // ---CONSTS---

    const char ERROR_MESSAGE[30] = "An error has occurred\n";
    const int STDOUT = 1;


    // ---SHELL STATE VARIABLES---

    char** path = malloc(1 * sizeof(char*));
    path[0] = "./bin/";
    int path_size = 1;

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
        while(token_count < 32 && (token = strsep(&buffer," \t")) != NULL && strcmp(token, "\n") != 0 && *token != '\0') {
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
                    continue;
                }
                redirIndex = i;
            }
        }

        FILE* out = NULL;
        if(redirIndex != -1) {
            if(redirIndex != token_count-2) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                continue;
            }
            
            if((out = fopen(tokens[token_count-1], "w")) == NULL) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                continue;
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
            shell_cd(&tokens[1]);
        }
        else if(strcmp(command, "path") == 0) {
            shell_path(&tokens[1]);
        }
        else if(strcmp(command, "if") == 0) {
            shell_if(&tokens[1]);
        }

        // User Commands
        else {

            // Check path for valid command
            char file[255];
            memset(file,0,sizeof(file));
            for(int i = 0; i < path_size; i++) {
                strcat(file, path[i]);
                strcat(file, command);
                if(access(file,X_OK) == 0) {
                    break;
                }
                memset(file,0,sizeof(file));
            }
            // User command not in path
            if(file[0] == '\0') {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                continue;
            }


            // Change stdout if redir
            int save_stdout = 0;
            if(redirIndex != -1) {
                save_stdout = dup(STDOUT);
                dup2(outFD, STDOUT);
            }       

            // Setup argv for program call
            char * exec_tokens[token_count];
            for(int i = 0; i < token_count-1; i++) {
                exec_tokens[i] = tokens[i+1];
            }
            exec_tokens[token_count-1] = NULL;

            // Fork shell to replace child with user process
            int fork_code = fork();
            if(fork_code == -1) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                continue;
            }
            else if(fork_code > 0) wait(NULL);
            else if(fork_code == 0) {
                if(execv(file, exec_tokens) == -1) {
                    write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                    exit(1);
                }
            }

            // Reset stdout
            if(redirIndex != -1) {
                dup2(save_stdout, STDOUT);
            }
        }


        // ---CLEAN-UP LOOP---
        // TODO: Make sure cleanup works on errors as well

        // Reset outFD and close redirect file
        if(redirIndex > -1 && redirIndex == token_count-2) {
            close(outFD);
            outFD = STDOUT;
        }
    }
}