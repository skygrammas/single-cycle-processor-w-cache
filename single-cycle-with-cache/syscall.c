/*
 * Gedare Bloom
 * Shumba Brown
 * Skylar Grammas
 * Deeshai Escoffery
 * Tashambra Williams
 * syscall.c
 *
 * Implementation of the system calls
 */

#include <stdlib.h>
#include <stdio.h>
#include "syscall.h"
#include "memory.h"
#include "cpu.h"

int syscall() {
    switch (cpu_ctx.GPR[2]){
        case 10:
            sys_exit();
            break;
        case 1:
            sys_int(cpu_ctx.GPR[4]);
            break;
        case 4:
            sys_string(cpu_ctx.GPR[4]);
            break;
    }
    
    return 0; 
}

int sys_exit() {
    printf("Exiting\n");
    
    for (int i = 0; i < 1024; i++ ) {
        printf("%x\n", data_memory[i]);
    }
    exit(0);
}

int sys_int(uint32_t reg) {
    printf("%d", reg);
    return 0;
}

int sys_string(uint32_t a0) {
    int index = a0;
    for (int i = 0; i < 8; i++) {
        int current = data_memory[index];
        for (int j = 0; j < 4; j++) {
            int new = current << (8*j);
            new = new >> 24;
            if (new == 0) return 0;
            printf("%c", new);
        }
        index++;
    }

    /*
    printf("printstring");

    for (int i = 0; i < 10; i++) {
        printf("%u", data_memory[i]);
    }
    */
    return 0;
}
