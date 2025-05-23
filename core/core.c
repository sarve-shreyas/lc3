#include<stdint.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/time.h>

#include "core.h"

uint16_t check_key() {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

/*
 * For reading data from addr space at given location
 * memory mapped registers make reading from memory a little complecated , we cant read & write to memory array directly
 *
*/
uint16_t mem_read(uint16_t address) {
    if (address == MR_KBSR) {
        if (check_key()) {
            memory[MR_KBSR] = (1 << 15);
            memory[MR_KBDR] = getchar();
        } else {
            memory[MR_KBSR] = 0;
        }
    }
    return memory[address];
}

/*
 * For Writing data to addr space at given location & what value need to be written
 */
void mem_write(uint16_t loc, uint16_t val) {
    memory[loc] = val;
}

/*
 * Any time value is written to register we need to update flags to indicate the sign of the register
 * left most bit 1 means the value is negative
 */
void update_flags(uint16_t r) {
    if (reg[r] == 0) {
        reg[R_COND] = FL_ZRO;
    } else if (reg[r] >> 15) {  // 1 in left-most bit indicates negative
        reg[R_COND] = FL_NEG;
    } else {
        reg[R_COND] = FL_POS;
    }
}

