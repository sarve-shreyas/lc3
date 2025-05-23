#ifndef _H_CORE_
#define _H_CORE_
#include<stdint.h>
/*
/// Memory Storage
/// Out vm supports total of 65,536 different address locations which is 2^16 bits each can store upto 16bit value
/// 2^16 x 16 bits = 128KB <- total memory of out vm
*/
#define MEMORY_MAX (1 << 16)
uint16_t memory[MEMORY_MAX];

/*
 * Register will be used by cpu to do arithmetic operations
 * VM has total of 10 registers 8 General Purpose (R0-R7), 1 Program Counter (PC), 1 Condition flags (COND)
 * each register is 16 bits
 */
enum {
    R_R0 = 0,
    R_R1,    // General Purpose 1
    R_R2,    // General Purpose 2
    R_R3,    // General Purpose 3
    R_R4,    // General Purpose 4
    R_R5,    // General Purpose 5
    R_R6,    // General Purpose 6
    R_R7,    // General Purpose 7
    R_PC,    // Program Counter
    R_COND,  // Condition flags
    R_COUNT  // Representing total registers count in vm
};
uint16_t reg[R_COUNT];

/* Memoery Mapped registers
 * Some special registers are not accessible from normal register table. Instead, a special address is reserved for them in memoery.
 * To read & write to this registers, you just read & write their memoery locations. these registers are used to interact with special hardware.
 *
 * LC-3 has two memoery mapped registers.
 * 1. keyboard status register (KBSR) -> whether the key has been pressed
 * 2. keyboard data register (KBDR) -> which key has been pressed
 */
enum {
    MR_KBSR = 0xFE00, /* keyboard status */
    MR_KBDR = 0xFE02, /* keyboard data */
};
/*
 * R_COND stores the conditions flags providing information about most recent executed calculation
 */

enum {
    FL_POS = 1 << 0, /* P */
    FL_ZRO = 1 << 1, /* Z */
    FL_NEG = 1 << 2, /* N */
};

uint16_t mem_read(uint16_t address);
void mem_write(uint16_t loc, uint16_t val);
/*
 * Any time value is written to register we need to update flags to indicate the sign of the register
 * left most bit 1 means the value is negative
 */
void update_flags(uint16_t r);

#endif
