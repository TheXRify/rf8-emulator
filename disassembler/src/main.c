#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// grabs a file extension from a path
char* grabFileExtension(char* path) {
    int len = strlen(path) + 1;
    char* ext = &path[len-4];
    return ext;
}

int grabOpcode(char* buffer, int i) {
    int op, r, l;
    op = buffer[i];
    r = buffer[i + 1];
    l = buffer[i + 2];

    if(r < 0) {
        r = (r & 0x000000ff);
    } else if(l < 0) {
        l = (l & 0x000000ff);
    }

    return ((op << 16) | (r << 8) | l);
}

char reg[10] = {
    'a', 'b', 'c', 'd', 'e', 'f', 's', 'd', 'l', 'h'
};

int main(int argc, char** argv) {
    // exit if there are no arguments provided
    if(argc < 1) {
        printf("No file passed for the disassembler!\n");
        return -1;
    }

    // check if the file extension is valid
    char* ext = grabFileExtension(argv[1]);
    if(strcmp(ext, "bin") != 0) {
        printf("File extension is not correct (.%s) should be .bin\n", ext);
        return -1;
    }

    // read the file contents into a buffer.
    FILE* file = fopen(argv[1], "r");

    // get file size
    fseek(file, 0l, SEEK_END);
    int filesize = ftell(file);
    rewind(file);

    char* buffer = malloc(filesize * sizeof(char));

    fread(buffer, filesize, 1, file);

    // process buffer into readable RF8 assembly
    for(int i = 0; i < filesize; i += 3) {
        // grab an opcode from the buffer
        int opcode = grabOpcode(buffer, i);

        if(opcode == 0x000000)
            break;

        // get the write assembly for the given opcode
        switch((opcode & 0xff0000) >> 16) {
            case 0x01: printf("lda 0x%04x\n", opcode & 0x00ffff); break;
            case 0x11: printf("ldb 0x%04x\n", opcode & 0x00ffff); break;
            case 0x12: printf("ldc 0x%04x\n", opcode & 0x00ffff); break;
            case 0x13: printf("ldd 0x%04x\n", opcode & 0x00ffff); break;
            case 0x14: printf("ldh 0x%04x\n", opcode & 0x00ffff); break;
            case 0x15: printf("ldl 0x%04x\n", opcode & 0x00ffff); break;
            case 0x02: printf("lia %d\n", opcode & 0x00ffff); break;
            case 0x21: printf("lib %d\n", opcode & 0x00ffff); break;
            case 0x22: printf("lic %d\n", opcode & 0x00ffff); break;
            case 0x23: printf("lid %d\n", opcode & 0x00ffff); break;
            case 0x24: printf("lih %d\n", opcode & 0x00ffff); break;
            case 0x25: printf("lil %d\n", opcode & 0x00ffff); break;
            case 0x03: printf("add\n"); break;
            case 0x31: printf("sub\n"); break;
            case 0x32: printf("div\n"); break;
            case 0x33: printf("mul\n"); break;
            case 0x34: printf("dec %c\n", reg[opcode & 0x0000ff]); break;
            case 0x35: printf("inc %c\n", reg[opcode & 0x0000ff]); break;
            case 0x04: printf("jmp 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x41: printf("jnz 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x42: printf("jz 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x43: printf("jeq 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x44: printf("jne 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x45: printf("jgt 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x46: printf("jlt 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x47: printf("jge 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x48: printf("jle 0x%04x\n", (opcode & 0x00ffff) - 2731); break;
            case 0x05: printf("sta 0x%04x\n", (opcode & 0x00ffff)); break;
            case 0x51: printf("stb 0x%04x\n", (opcode & 0x00ffff)); break;
            case 0x52: printf("stc 0x%04x\n", (opcode & 0x00ffff)); break;
            case 0x53: printf("std 0x%04x\n", (opcode & 0x00ffff)); break;
            case 0x54: printf("sth 0x%04x\n", (opcode & 0x00ffff)); break;
            case 0x55: printf("stl 0x%04x\n", (opcode & 0x00ffff)); break;
            case 0x56: printf("ste 0x%04x\n", (opcode & 0x00ffff)); break;
            case 0x06: printf("hlt\n"); break;
            case 0x07: printf("cmp %c, %c\n", reg[((opcode & 0x00ff00) >> 8)], reg[opcode & 0x0000ff]); break;
            case 0x71: printf("czr %c\n", reg[opcode & 0x0000ff]); break;
            case 0x08: printf("mov %c, %c\n", reg[((opcode & 0x00ff00) >> 8)], reg[opcode & 0x0000ff]); break;

            default: printf("%06x\n", opcode); break;
        }
    }

    free(buffer);

    return 0;
}