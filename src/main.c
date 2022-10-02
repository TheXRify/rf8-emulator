#include <stdio.h>
#include <stdlib.h>

#include "emu.h"

int main(void) {
    bootEmulator();

    int choice;
    printf("0.) Adder\n1.) Compare Test\n2.) Powers\n");
    scanf("%d", &choice);

    switch(choice)
    {
        case 0:
            loadROM("./bin/roms/adder.bin");
        break;
        case 1:
            loadROM("./bin/roms/comparetest.bin");
        break;
        case 2:
            loadROM("./bin/roms/powers.bin");
        break;
        default:
        {
            printf("THAT IS NOT A VALID OPTION!!\n");
            exit(-1);
        }
        break;
    }

    runEmulation();

    closeEmulator();

    return 0;
}