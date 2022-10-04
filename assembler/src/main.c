#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rf8.h"
#include "lexrf8.h"

char* grabExtension(char* path) {
    int len = strlen(path) + 1;
    char* ext = &path[len-4];
    return ext;
}

int main(int argc, char** argv) {
    // exit if no arguments passed
    if(argc < 1) {
        printf("You need to pass a file to this assembler!\n");
        return -1;
    }

    // check if the extension is valid
    char* ext = grabExtension(argv[1]);
    if(strcmp(ext, "asm") != 0) {
        printf("The file extension does not match (%s) should be .asm!\n", ext);
        return -1;
    }

    // load the file
    FILE* file = fopen(argv[1], "r");
    if(!file) {
        printf("Could not find the file specified!\n");
        return -1;
    }

    // get the file's size in bytes
    fseek(file, 0l, SEEK_END);
    int fileSize = ftell(file);
    rewind(file);

    // read the file's contents and lexalize them into lexemes and tokens
    lexFile(file, fileSize);

    return 0;
}