#include "lexrf8.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* instructions[50] = {
    "lda", "ldb", "ldc", "ldd", "ldh", "ldl",
    "lia", "lib", "lic", "lid", "lih", "lil",
    "add", "sub", "mul", "div", "dec", "inc",
    "jmp", "jnz", "jz", "jeq", "jne", "jgt", "jlt", "jge", "jle",
    "sta", "stb", "stc", "std", "sth", "stl", "ste",
    "hlt",
    "cmp", "czr",
    "mov"
};

char singleCharTokens[50] = {
    ',', '\n', '+', '-', '*', '/', '(', ')', '"', '\'',
    'a', 'b', 'c', 'd', 'e', 's', 'd', 'h', 'l', 'f'
};

// creates a token for the parser
// "single" refers to if the value passed in is a single character
token_t* createToken(const char* value, int single) {
    token_t* tok = malloc(sizeof(token_t));

    if(single) {
        switch(*value) {
            case ',':
                tok->type = COMMA;
                tok->value.chr = ',';
            break;
            case 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 's' | 'd' | 'h' | 'l':
                tok->type = REGISTER;
                tok->value.chr = *value;
            break;
        }

        return tok;
    } else {
        tok->type = INSTRUCTION;
    }

    return NULL;
}

void lexFile(FILE* file, int fileSize) {
    // token array
    token_t* tokens[1024];
    char buffer[256];

    int tokens_i = 0;
    int buffer_i = 0;

    // begin reading the file's source
    char line[256]; // lines can be 256 lines long to conserve memory
    while(fgets(line, fileSize, file)) {
        
    }
}