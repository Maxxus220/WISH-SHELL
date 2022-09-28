// ---INCLUDES---
#include "builtins.h"
#include "utils.h"
#include "stdio.h"


// ---SHELL ENTRY---

int main(int argc, char *argv[]) {

    // ---CONSTS---

    const char ERROR_MESSAGE[30] = "An error has occurred\n";
    const int STDOUT = 1;

    // ---SHELL STATE VARIABLES---

    int batchMode = 0;

    // TODO: Set up shell read source
    // ---READ SOURCE SETUP---

    // Batch file potentially provided
    if(argc == 2) {
        // Check for valid batch file
        if(checkBatch(argv[1])) {
            freopen(argv[1], "r", stdin);
            batchMode = 1;
        }
        else {
            write(stderr, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
            exit(1);
        }
    }
    else if(argc > 2) {
        write(stderr, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(1);
    }

    // ---MAIN SHELL LOOP---

    while(1) {
        // Wait for input


        // TODO: Get user input


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
    }
}