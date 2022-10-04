#include "lexrf8.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void lexFile(FILE* file, int fileSize) {
    // token array
    token_t* tokens[1024];
    int tokens_i = 0;
    char* buffer;
    int buffer_i = 0;

    // begin reading the file's source
    char line[256]; // lines can be 256 lines long to conserve memory
    while(fgets(line, fileSize, file)) {
        buffer = strtok(line, " "); // separate the line into it's various segments

        while(buffer != NULL) {
            // TODO: process each segment into tokens

            buffer = strtok(NULL, " ");
        }
    }
}