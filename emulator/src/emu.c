#include "emu.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ZERO 1
#define EQUAL 2
#define GREATER 4
#define LESSER 4
#define NOT 8
#define CARRY 16

int getreg(int reg) {
    return emu->reg[reg];
}

void setreg(int val, int reg) {
    emu->reg[reg] = val;
}

int getdatabus(void) {
    return emu->dataBus;
}

void getdatabusreg(int reg) {
    setreg(emu->dataBus, reg);
}

void cleardatabus(void) {
    emu->dataBus = 0;
}

void setdatabus(int val) {
    emu->dataBus = val;
}

int getaddressbus(void) {
    return emu->addressBus;
}

void getaddressbusreg(int reg) {
    setreg(emu->addressBus, reg);
}

void clearaddressbus(void) {
    emu->addressBus = 0;
}

void setaddressbus(int val) {
    emu->addressBus = val;
}

void bootEmulator(void) {
    int i = 0;

    // allocate memory for the emulator container itself
    emu = (struct rf8*) malloc(sizeof(struct rf8));

    for(; i < 10; i++)
        emu->reg[i] = 0;

    // allocate the memory required for the emulator's memory
    emu->memory = malloc(MEMORY_SIZE * sizeof(char));
    // allocate enough memory required for the emulator's system
    emu->sys = malloc(sizeof(struct system));
    emu->pc = PROGRAM_MEMORY_OFFSET;
    emu->dataBus = 0;
    emu->addressBus = 0;
    isRunning = 0;
}

void hexdump() {
    for(int i = 0; i < MEMORY_SIZE; i++) {
        if(i == 0)
            printf("\nDATA MEMORY\n===========================================\n");
        else if(i == STACK_MEMORY_OFFSET)
            printf("\nSTACK MEMORY\n===========================================\n");
        else if(i == PROGRAM_MEMORY_OFFSET)
            printf("\nPROGRAM MEMORY\n===========================================\n");

        // if(i % 10 == 0 && i != 0 && i != STACK_MEMORY_OFFSET && i != PROGRAM_MEMORY_OFFSET)
        //     printf("\n");

        printf("%d - %02x (%d)\n", i, emu->memory[i], emu->memory[i]);
    }
}

void loadROM(const char* path) {
    FILE* file = fopen(path, "r");
    if(!file) {
        printf("ERROR! Could not find the ROM!\n");
        exit(-1);
    }

    fseek(file, 0l, SEEK_END);
    int filesize = ftell(file);
    rewind(file);

    fread(emu->memory + PROGRAM_MEMORY_OFFSET, filesize, 1, file);

    // hexdump();
}

int grabOpcode() {
    int op, R, L; // opcode, left, right

    op = emu->memory[emu->pc];
    R = emu->memory[emu->pc + 1];
    L = emu->memory[emu->pc + 2];

    if(R < 0)
        R = R & 0x000000FF;
    else if(L < 0)
        L = L & 0x000000FF;

    int opcode = (op << 16) | (R << 8) | L;

    return opcode;
}

void runEmulation() {
    int opcode = 0;
    char isJump = 0;
    isRunning = 1;
    int i = 0;

    while(isRunning && emu->pc < PROGRAM_MEMORY_OFFSET + MAX_PROGRAM_LENGTH) {
        opcode = grabOpcode();
        isJump = 0;

        printf("\n0x%04x (%d) - %d - 0x%06x\n=========\nA: 0x%04x\nB: 0x%04x\nC: 0x%04x\nD: 0x%04x\nE: 0x%04x\nF: 0x%04x\nH: 0x%04x\nL: 0x%04x\n", 
            emu->pc,
            emu->pc,
            i, 
            opcode,
            getreg(a),
            getreg(b),
            getreg(c),
            getreg(d),
            getreg(e),
            getreg(f),
            getreg(h),
            getreg(l)
        );

        switch((opcode & 0xff0000) >> 16) {
            case 0x01: { // lda <addr>
                setreg(emu->memory[opcode & 0x00ffff], a);
            }
            break;
            case 0x11: { // ldb <addr>
                setreg(emu->memory[opcode & 0x00ffff], b);
            }
            break;
            case 0x12: { // ldc <addr>
                setreg(emu->memory[opcode & 0x00ffff], c);
            }
            break;
            case 0x13: { // ldd <addr>
                setreg(emu->memory[opcode & 0x00ffff], d);
            }
            break;
            case 0x14: { // ldh <addr>
                setreg(emu->memory[opcode & 0x00ffff], h);
            }
            break;
            case 0x15: { // ldl <addr>
                setreg(emu->memory[opcode & 0x00ffff], l);
            }
            break;
            case 0x02: { // lia <val>
                setreg(opcode & 0x00ffff, a);
            }
            break;
            case 0x21: { // lib <val>
                setreg(opcode & 0x00ffff, b);
            }
            break;
            case 0x22: { // lic <val>
                setreg(opcode & 0x00ffff, c);
            }
            break;
            case 0x23: { // lid <val>
                setreg(opcode & 0x00ffff, d);
            }
            break;
            case 0x24: { // lih <val>
                setreg(opcode & 0x00ffff, h);
            }
            break;
            case 0x25: { // lil <val>
                setreg(opcode & 0x00ffff, l);
            }
            break;
            case 0x03: { // add
                int result = getreg(h) + getreg(l);
                setreg(result, h);
            }
            break;
            case 0x31: { // sub
                int result = getreg(h) - getreg(l);
                setreg(result, h);
            }
            break;
            case 0x32: { // div
                int result = getreg(h) / getreg(l);
                setreg(result, h);
            }
            case 0x33: { // mul
                int result = getreg(h) * getreg(l);
                setreg(result, h);
            }
            break;
            break;
            case 0x34: { // dec <reg>
                int r = opcode & 0x0000FF;
                setreg(getreg(r) - 1, r);
            }
            break;
            case 0x35: { // inc <reg>
                int r = opcode & 0x0000FF;

                // if(getreg(r) + 1 > 65535) {
                //     setreg(0, r);
                //     setreg(CARRY, f);
                //     break;
                // }

                setreg(getreg(r) + 1, r);
            }
            break;
            case 0x04: { // jmp <addr>
                int R, L;

                R = opcode & 0x00ff00;
                L = opcode & 0x0000ff;
                printf("0x%02x%02x\n", (R >> 8), L);

                emu->pc = R | L;
                isJump = 1;
            }
            break;
            case 0x41: { // jnz <addr>
                if(getreg(f) != (NOT | ZERO)) {
                    emu->pc = opcode & 0x00ffff;
                    isJump = 1;
                }
            }
            break;
            case 0x42: { // jz <addr>
                if(getreg(f) == ZERO) {
                    emu->pc = opcode & 0x00ffff;
                    isJump = 1;
                }
            }
            break;
            case 0x43: { // jeq <addr>
                if(getreg(f) == EQUAL) {
                    emu->pc = opcode & 0x00ffff;
                    isJump = 1;
                }
            }
            break;
            case 0x44: { // jne <addr>
                if(getreg(f) == (NOT | EQUAL)) {
                    emu->pc = opcode & 0x00ffff;
                    isJump = 1;
                }
            }
            break;
            case 0x45: { // jgt <addr>
                if(getreg(f) == GREATER) {
                    emu->pc = opcode & 0x00ffff;
                    isJump = 1;
                }
            }
            break;
            case 0x46: { // jlt <addr>
                if(getreg(f) == LESSER) {
                    emu->pc = opcode & 0x00ffff;
                    isJump = 1;
                }
            }
            break;
            case 0x47: { // jge <addr>
                if(getreg(f) == (GREATER | EQUAL)) {
                    emu->pc = opcode & 0x00ffff;
                    isJump = 1;
                }
            }
            break;
            case 0x48: { // jle <addr>
                if(getreg(f) == (LESSER | EQUAL)) {
                    emu->pc = opcode & 0x00ffff;
                    isJump = 1;
                }
            }
            break;
            case 0x05: { // sta <addr>
                emu->memory[opcode & 0x00ffff] = getreg(a);
            }
            break;
            case 0x51: { // stb <addr>
                emu->memory[opcode & 0x00ffff] = getreg(b);
            }
            break;
            case 0x52: { // stc <addr>
                emu->memory[opcode & 0x00ffff] = getreg(c);
            }
            break;
            case 0x53: { // std <addr>
                emu->memory[opcode & 0x00ffff] = getreg(d);
            }
            break;
            case 0x54: { // sth <addr>
                emu->memory[opcode & 0x00ffff] = getreg(h);
            }
            break;
            case 0x55: { // stl <addr>
                emu->memory[opcode & 0x00ffff] = getreg(l);
            }
            break;
            case 0x56: { // ste <addr>
                emu->memory[opcode & 0x00ffff] = getreg(e);
            }
            break;
            case 0x06: { // hlt
                isRunning = 0;
            }
            break;
            case 0x07: { // cmp <reg/reg>
                int r1, r2; // registers used for comparing
                r1 = (opcode & 0x00FF00) >> 8;
                r2 = (opcode & 0x0000FF);
                int r1Val = getreg(r1), r2Val = getreg(r2);

                if(r1Val != r2Val)
                    setreg(NOT | EQUAL, f);
                else if(r1Val == r2Val)
                    setreg(EQUAL, f);
                else if(r1Val > r2Val)
                    setreg(GREATER, f);
                else if(r1Val < r2Val)
                    setreg(LESSER, f);
                else if(r1Val >= r2Val)
                    setreg(GREATER | EQUAL, f);
                else if(r1Val <= r2Val)
                    setreg(LESSER | EQUAL, f);
            }
            break;
            case 0x71: { // czr <reg>
                int reg = opcode & 0x0000FF;
                if(getreg(reg) == 0)
                    setreg(ZERO, f);
                else
                    setreg(NOT | ZERO, f);
            }
            break;
            case 0x08: { // mov <reg/reg>
                int r1, r2;
                r1 = (opcode & 0x00FF00) >> 8;
                r2 = (opcode & 0x0000FF);

                setreg(r1, getreg(r2));
            }
            break;
        }

        if(!isJump)
            emu->pc += 3;

        i++;
        sleep((1 / CLOCK_CYCLE) * 1000); // convert Hz to seconds
    }
}

void closeEmulator(void) {
    // destroy the emulator memory as well as the emulator itself
    free(emu->memory);
    free(emu);
}