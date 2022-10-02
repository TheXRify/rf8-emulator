#ifndef EMU_H
#define EMU_H

#include "sys.h"

#define BIT_DEPTH 16 // bits
#define OPCODE_BIT_DEPTH 8 // bits
#define ADDRESS_BUS_BITS 16 // bits
#define DATA_BUS_BITS 16 // bits

#define MEMORY_SIZE 4096 // bytes
#define DATA_MEMORY_OFFSET 0 // bytes
#define STACK_MEMORY_OFFSET 1365 // bytes
#define PROGRAM_MEMORY_OFFSET 2731 // bytes
#define MAX_PROGRAM_LENGTH 1365 // bytes

#define CLOCK_CYCLE 250 // Hz (5 millisecond)

struct rf8 {
    int reg[10]; // registers
    
    int pc; // program counter
    char* memory;
    int dataBus : DATA_BUS_BITS;
    int addressBus : ADDRESS_BUS_BITS;
    struct system* sys;
};

static int isRunning = 0; // checking to see if the cpu is actively running
static struct rf8* emu;

static const int a = 0;
static const int b = 1;
static const int c = 2;
static const int d = 3;
static const int e = 4; // carry register
static const int f = 5; // flag register
static const int sp = 6;
static const int dp = 7;
static const int l = 8; // denominator in alu
static const int h = 9; // nominator in alu

int getreg(int reg);
void setreg(int val, int reg);

// bus functions
// // these functions manage any and all bus operations
int getdatabus(void); // return the current value of the data bus
void getdatabusreg(int reg); // store into a given register, the current data in the data bus.
void cleardatabus(void); // clear the data bus
void setdatabus(int val); // set the value of the data bus
void senddata(int flag, int data);

int getaddressbus(void); // return the current value of the address bus
void getaddressbusreg(int reg); // store into a given register, the current address in the address bus.
void clearaddressbus(void); // clear the address bus
void setaddressbus(int val); // set the value of the address bus
void sendaddress(int flag, int data);

// Boot the emulator and allocate enough memory to the host computer to run the emulator
void bootEmulator(void);
// Load a program into the emulators RAM
void loadROM(const char* path);
// Begin the emulation process
void runEmulation(void);

// Close the emulator and free all memory associated with the emulator.
void closeEmulator(void);

#endif // EMU_H