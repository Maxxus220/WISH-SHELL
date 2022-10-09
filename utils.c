#include "utils.h"
#include <string.h>
#include <stdlib.h>

/**
 * Contains commands for various wish shell checks
 */

void cleanUp(void) {

}

/**
 * Takes a str and returns a pointer to an array filled with all tokens
*/
char** tokenize(char *str, int *sizeDest) {
    if(str == NULL) {
        return NULL;
    }

    int i = 0;
    int j = 0;
    int size = 0;
    char temp[255] = {0};
    char **arr = malloc(255 * sizeof(char*));
    short isToken = 0;
    while(str[i] != '\0') {
        if(isToken && (str[i] == ' ' || str[i] == '\t')) {
            temp[size] = '\0';
            arr[j] = malloc(strlen(temp) + 1);
            strcpy(arr[j], temp);
            arr[j][size] = '\0';
            isToken = 0;
            j++;
            size = 0;
        }
        else if(str[i] == '>') {
            temp[size] = '\0';
            arr[j] = malloc(strlen(temp) + 1);
            strcpy(arr[j], temp);
            j++;
            arr[j] = ">";
            j++;
            size = 0;
            isToken = 0;
        }
        else if(str[i] == '\n' || str[i] == '\r') {
            temp[size] = '\0';
            arr[j] = malloc(strlen(temp) + 1);
            strcpy(arr[j], temp);
            arr[j][size] = '\0';
            if(isToken) j++;
            break;
        }
        else if(str[i] != ' ' && str[i] != '\t'){
            isToken = 1;
            temp[size] = str[i];
            size++;
        }
        i++;
    }
    *sizeDest = j;
    return arr;
}