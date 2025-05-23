#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "./core/core.h"
#include "./core/input-buffering.h"
#include "./core/read-image.h"
#include "instruction-set.h"

/*
 * Function to abort the program in between if any exception occured or some wrong instruction is being send
 */
void abort_program(uint16_t ret) {
    printf("Program Exitted with %d", ret);
    exit(ret);
}

int extecute() {
    int running = 1;
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t opcode = instr >> 12;
    // printf("Running loop opcode -> %d\n", opcode);
    switch (opcode) {
        case OP_BR: { /* 0000 -> 0 */
            running = op_branch(instr);
            break;
        }
        case OP_ADD: { /* 0001 -> 1 */
            running = op_add(instr);
            break;
        }
        case OP_LD: { /* 0010 -> 2 */
            running = op_load(instr);
            break;
        }
        case OP_ST: { /* 0011 -> 3 */
            running = op_store(instr);
            break;
        }
        case OP_JSR: { /* 0100 -> 4 */
            running = op_jump_to_subroutine(instr);
            break;
        }
        case OP_AND: { /* 0101 -> 5 */
            running = op_and(instr);
            break;
        }
        case OP_LDR: { /* 0110 -> 6 */
            running = op_load_base_offset(instr);
            break;
        }
        case OP_STR: { /* 0111 -> 7 */
            running = op_store_base_offset(instr);
            break;
        }
        case OP_NOT: { /* 1001 -> 9 */
            running = op_not(instr);
            break;
        }
        case OP_LDI: { /* 1010 -> 10 */
            running = op_load_indirect(instr);
            break;
        }
        case OP_STI: { /* 1011 -> 11 */
            running = op_store_indirect(instr);
            break;
        }
        case OP_JMP: { /* 1100 -> 12 */
            running = op_jump(instr);
            break;
        }
        case OP_LEA: { /* 1101 -> 14 */
            running = op_load_effective(instr);
            break;
        }
        case OP_TRAP: { /* 1111 -> 15 */
            running = op_trap(instr);
            break;
        }
        case OP_RES: /* 1000 -> 8 */
        case OP_RTI: /* 1101 -> 13 */
        default: {
            abort_program(1);
            break;
        }
    }
    return running;
}
void setup_vm() {
    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    // if (argc < 2) {
    //     /* show usage string */
    //     printf("lc3 [image-file1] ...\n");
    //     exit(2);
    // }
    const char* filename = "/Users/evendead/Downloads/2048.obj";
    if (!read_image(filename)) {
        printf("failed to load image: %s\n", filename);
        abort_program(1);
    }
    /* excatly one condition flag can be set at a given time intital value to Z*/
    reg[R_COND] = FL_ZRO;
    /* set PC to starting position */
    enum { PC_START = 0x3000 };
    reg[R_PC] = PC_START;
}

int main(int argc, const char* argv[]) {
    setup_vm();
    int running = 1;
    int instruction_number = 1;
    while (running) {
        running = extecute();
        instruction_number++;
    }
    restore_input_buffering();
    return 0;
}
