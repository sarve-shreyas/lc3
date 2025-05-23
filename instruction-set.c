#include "instruction-set.h"

#include <stdint.h>
#include <stdio.h>

#include "./core/bit-utilities.h"
#include "./core/core.h"

uint16_t op_add(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;

    uint16_t imm_flag = (instr >> 5) & 0x1;
    if (imm_flag) {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] + imm5;
    } else {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] + reg[r2];
    }
    update_flags(r0);
    return 1;
}

uint16_t op_and(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;

    uint16_t imm_flag = (instr >> 5) & 0x1;
    if (imm_flag) {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] & imm5;
    } else {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] & reg[r2];
    }
    update_flags(r0);
    return 1;
}

uint16_t op_not(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    reg[r0] = ~reg[r1];
    update_flags(r0);
    return 1;
}

uint16_t op_branch(uint16_t instr) {
    
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    uint16_t cond_flag = (instr >> 9) & 0x7;
    if (cond_flag & reg[R_COND]) {
        reg[R_PC] += pc_offset;
    }
    return 1;
}

uint16_t op_jump(uint16_t instr) {
    
    uint16_t r0 = (instr >> 6) & 0x7;
    reg[R_PC] = reg[r0];
    return 1;
}

uint16_t op_jump_to_subroutine(uint16_t instr) {
    
    uint16_t long_flag = (instr >> 11) & 0x1;
    reg[R_R7] = reg[R_PC];
    if (long_flag) {
        uint16_t pc_offset = sign_extend(instr & 0x7FF, 11);
        reg[R_PC] = reg[R_PC] + pc_offset; /* JSR */
    } else {
        uint16_t r1 = (instr >> 6) & 0x7;
        reg[R_PC] = reg[r1]; /* JSRR */
    }
    return 1;
}

uint16_t op_load(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    reg[r0] = mem_read(pc_offset + reg[R_PC]);
    update_flags(r0);
    return 1;
}

uint16_t op_load_indirect(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    reg[r0] = mem_read(mem_read(pc_offset + reg[R_PC]));
    update_flags(r0);
    return 1;
}

uint16_t op_load_base_offset(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t bR = (instr >> 6) & 0x7;
    uint16_t offset = sign_extend(instr & 0x3F, 6);
    reg[r0] = mem_read(reg[bR] + offset);
    update_flags(r0);
    return 1;
}

uint16_t op_load_effective(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    reg[r0] = reg[R_PC] + pc_offset;
    update_flags(r0);
    return 1;
}

uint16_t op_store(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    mem_write(reg[R_PC] + pc_offset, reg[r0]);
    return 1;
}

uint16_t op_store_indirect(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    mem_write(mem_read(reg[R_PC] + pc_offset), reg[r0]);
    return 1;
}

uint16_t op_store_base_offset(uint16_t instr) {
    
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    uint16_t offset = sign_extend(instr & 0x3F, 6);
    mem_write(reg[r1] + offset, reg[r0]);
    return 1;
}

/* trap instructions */

uint16_t op_trap(uint16_t instr) {
    switch (instr & 0xFF) {
        case TRAP_GETC: {
            return op_trap_getc(instr);
        }
        case TRAP_OUT: {
            return op_trap_out(instr);
        }
        case TRAP_PUTS: {
            return op_trap_puts(instr);
        }
        case TRAP_IN: {
            return op_trap_in(instr);
        }
        case TRAP_PUTSP: {
            return op_trap_putsp(instr);
        }
        case TRAP_HALT: {
            return op_trap_halt(instr);
        }
        default:
            return 0;
    }
    return 1;
}

uint16_t op_trap_getc(uint16_t instr) {
    reg[R_R0] = (uint16_t)getchar();
    return 1;
}

uint16_t op_trap_out(uint16_t instr) {
    putc((char)(reg[R_R0]), stdout);
    fflush(stdout);
    return 1;
}

uint16_t op_trap_puts(uint16_t instr) {
    uint16_t* c = memory + reg[R_R0];
    while (*c) {
        putc((char)*c, stdout);
        c++;
    }
    fflush(stdout);
    return 1;
}

uint16_t op_trap_in(uint16_t instr) {
    reg[R_R0] = (uint16_t) getchar();
    fflush(stdout);
    return 1;
}

uint16_t op_trap_putsp(uint16_t instr) {
    uint16_t* c = memory + reg[R_R0];
    while (*c) {
        char char1 = (*c) & 0xFF;
        putc((char)(char1), stdout);
        char char2 = (*c) >> 8;
        if (char2)
            putc((char)((*c) >> 8), stdout);
        ++c;
    }
    fflush(stdout);
    return 1;
}

uint16_t op_trap_halt(uint16_t instr) {
    puts("HALT");
    fflush(stdout);
    return 0;
}
