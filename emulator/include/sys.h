#ifndef SYS_H
#define SYS_H

#define STD_IN 0x16
#define STD_OUT 0x26

struct system {
    int dataout : 16;
    int datain : 16;

    char* stdout;
    char* stdin;
};

void sys_process();

void sys_getdatain(int data);
int sys_getdataout();

#endif // SYS_H