#ifndef _H_OP_CODE_
#define _H_OP_CODE_

/*
 * Instruction set defines CPU to do fundamental task such as addition, substraction.
 * Instruction set have opcode indicating what kind of task to be performed , paramerters which provided the imput to task being performed.
 * We will use 16 bit instruction set which will support total 16 opcodes (4bits opcode) other 12bits will be parameters
                +----------------+------------------------------+
                |   Opcode (4b)  |       Parameters (12b)       |
                +----------------+------------------------------+
                | Bits: 15 - 12  |         Bits: 11 - 0         |
                +----------------+------------------------------+

*/
enum {
    OP_BR = 0, /* branch */
    OP_ADD,    /* add */
    OP_LD,     /* load	*/
    OP_ST,     /* store */
    OP_JSR,    /* jump register */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load register */
    OP_STR,    /* store register */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump	*/
    OP_RES,    /* reserved */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap	*/
};
/* TRAP Codes
 * each trap instruction has a trap code assigned to it
 */
enum {
    TRAP_GETC = 0x20,  /* get character from keyboard, not echoed to console */
    TRAP_OUT = 0x21,   /* ouput a character */
    TRAP_PUTS = 0x22,  /* output a word string */
    TRAP_IN = 0x23,    /* get character from termainal, echoed to console */
    TRAP_PUTSP = 0x24, /* output a byte string */
    TRAP_HALT = 0x25,  /* halt the program */
};

#endif
