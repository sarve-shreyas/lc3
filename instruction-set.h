#ifndef _H_INSTRUCTION_SET_
#define _H_INSTRUCTION_SET_

#include<stdint.h>
#include "./core/opcode.h"

/*
* Perform Addition of parameters
* r0 is destination register which indicates where the result of this operation should be stored
* r0 ( 3bits after opcode of 4bits) right shiting this by 9 bits will result in bringing the r0 bits in least significant position ( rightmost side)
* taking bitwise & with 111 will give us r0 bits
* example -> instr = 0001 1010 0000 0000
* instr >> 9 = 0001 0000 0000 0101
* r0 = instr & 0x7 = 0000 0000 0000 0101 & 0000 0000 0000 0111 = 0000 0000 0000 0101
*/

/*
* First operant of the operation (SR1)
* similar to exmplanation of r0 we shift right by 6 bits ( remaining bits ) then take bitwise & with 0x7
*/

/*
* Immediate mode the second value is embedded in right-most 5 bits
* in immediate mode we sign extend the value embedded in right-most 5 bits & add that value to the first operand
* when immediate flag is not set then last 3 bits of instruction set is register bit ( which register we need to pick )
*/
uint16_t op_add(uint16_t instruction);


/* Perform bitwise ADN operation
* bitwise AND have two variation of instruction set similar to ADD
*	Variation 1
* +--------------------------------------------------+
* | 0101 | DR(3bit) | SR1(3bit) | 0 | 00 | SR2(3bit) |
* +--------------------------------------------------+
*
*	Variation 2
* +--------------------------------------------------+
* | 0101 | DR(3bit) | SR1(3bit) | 1 | imm5(5bit)     |
* +--------------------------------------------------+
*
* DR is destination register where we need to store the result of operation
* SR1 is first operant register of the operation
* if immediate mode set then last 5 bits represent imm5 value with which bitwise and operation need to be performed to do that we need to extend this to 16 bits
* if immediate bit is not set then last 3bits represent the second operant register
*/

uint16_t op_and(uint16_t instruction);

/* Bit-wise complement
* NOT DR, SR
* Encoding
* +----------------------------------------+
* | 1001 | DR(3bit) | SR(3bit) | 1 | 11111 |
* +----------------------------------------+
* opcode[15:12]
* DR[11:9] Destination register where the bitwise complement need to be stored
* SR[8:6] Register from which we need to pick value for operation
*/
uint16_t op_not(uint16_t instruction);

/* Conditional Branch
* Encoding
* +------------------------------------------------------+
* | 0000 | n(1bit) | z(1bit) | p(1bit) | PCoffset9(9bit) |
* +------------------------------------------------------+
* n -> indicates negative
* z -> indicates zero
* p -> indicates positive
* PCoffset[8:0] 9bit offset that we need to add to PC
*/
uint16_t op_branch(uint16_t instruction);

/* Jump instruction
* JMP BaseR
* used to set Program Counter (PC) to location in register
* Encoding
* +---------------------------------------+
* | 1100 | 000 | BaseR(3bit) | 000000     |
* +---------------------------------------+
* opcode[15:12]
* BaseR[8:6] register from which we need to set PC
*/
uint16_t op_jump(uint16_t instruction);

/*
* Jump to SubRoutine
* this has two variations
* Encoding JSR
* +--------------------------------------+
* | 0100 | 1 | PCoffset11(11bit)	  |
* +--------------------------------------+
* PCoffset11[10:0] -> indicates offset that need to be added to PC updating its value
*
* Encoding JSRR
* +--------------------------------------+
* | 0100 | 0 | 00 | BaseR(3bit) | 000000 |
* +--------------------------------------+
* BaseR[8:6] 3bit value indicating which register contains the next PC value
*
* NOTE: before doing operation we need to set R7 = PC so when subroutine is complete & we return we will have the previous PC
*
*/
uint16_t op_jump_to_subroutine(uint16_t instruction);

/*
* Load
* instruction set for this look like follow
* +---------------------------------------+
* | 0010 | DR (3bits) | PCoffset9 (9bits) |
* +---------------------------------------+
* DR is destination register where we need to store out result
* PCoffset9 is offset that we need to add to PC & pick value from that address from memory space but firstly we will need to convert that addr to 16 bit
*/
uint16_t op_load(uint16_t instruction);

/*
* Load Indirect used to load value from location in memory
* Here is how its encoding looks like
* +------------------------------------------+
* | 1010 | DR (3bits) | PCoffset9	      |
* +------------------------------------------+
* DR is destination Register where we need to load the value
* PCoffset9 is offset from PC counter from where we need to get the address of value & then pich the value from that address
* to ge add we need to cinvert PCoffset9 to 16 bit
* addr_of_addr = PCoffset9(16bit) + PC
* addr = mem[addr_of_addr]
* value = mem[addr]
* this is useful because the PCoffset9 can have upto 9bits of address only but out memoery space is of 16bit so to utilise that we store addr in memory & store the data in that addr to utlise the 16bits fully
*/
uint16_t op_load_indirect(uint16_t instruction);

/* Load Base + Offset
* LDR DR, BaseR, offset6
* Encoding
* +------------------------------------------+
* | 0110 | DR (3bit) | BaseR(3bit) | offset6 |
* +------------------------------------------+
* DR[11:9] is destination register where we need to store the result of the operation
* BaseR[8:6] is register from which we need to pick out initial point before adding the offset to it
* offset6[5:0] is a 6 bit offset value that we will add to value in BaseR & pick that value from mem
* so final addr = val(BaseR) + (16bit) offset6
*/
uint16_t op_load_base_offset(uint16_t instruction);

/* Load Effective Address
* LEA DR, LABEL
* In this operation the address is loaded into Destination Address not the value at the address
* Encoding
* +--------------------------------------------+
* | 1110 | DR(3bit) |     PCoffset9 (9bits)	|
* +--------------------------------------------+
* opcode[15:12]
* DR[11:9] is a destination register where we need to store the addr
* PCoffset9[8:0] 9bit offset which we need to add to PC before that we need to convert this to 16bit
*/
uint16_t op_load_effective(uint16_t instruction);

/* Store to memoery
* ST SR, LABEL
* Encoding
* +-------------------------------------+
* | 0011 | SR(3bit) | PCoffset9 (9bit)  |
* +-------------------------------------+
* opcode[15:12]
* SR[11:9] data from register which need to be stored to the label
* PCoffset[8:0] Offset from PC at which we need to store the data
*/
uint16_t op_store(uint16_t instruction);

/* Store Indirect -> storing value to addr of addr
* we will store at the address given in memory location which is at PC + offset
* STI SR, LABEL
* Encoding
* +-----------------------------------------+
* | 1011 | SR(3bit) | PCoffset9(9bit)       |
* +-----------------------------------------+
* SR[11:9] value that need to be stored to location
* PCoffset[8:0] Offset from Program counter , we will add this value to determine the addr after converting this to 16bit
*/
uint16_t op_store_indirect(uint16_t instruction);

/* Store Base + Offset
* STI SR, BaseR, offset6
* Encoding
* +-----------------------------------------------+
* | 0111 | SR(3bit) | BaseR(3bit) | offset6(6bit) |
* +-----------------------------------------------+
* opcode[15:12]
* SR[11:9] value at register which we want to store to memory
* BaseR[8:6] register where intial point from which we will add offset is stored
* offset6[5:0] offset that we will add to BaseR after converting it to 16bit
*/
uint16_t op_store_base_offset(uint16_t instruction);


/* trap instructions */
/*
* Trap Routines this are used to peform common task like I/O
* each TRAP routine is assigned a `trap code` which identifies it
* Encoding
* +------------------------------------------+
* | 1111 | 0000 |      trapvect8(8bit)       |
* +------------------------------------------+
*/

uint16_t op_trap(uint16_t instruction);

/*
 * Reading Single character from keyboard, the character is not echoed to console
 * The ASCII code is copied into R0. the high eight bits of R0 are also cleared.
*/
uint16_t op_trap_getc(uint16_t instruction);

/*
 * Writing character to the console
 * This instruction will write character in R0[7:0] to console
*/
uint16_t op_trap_out(uint16_t instruction);

/*
 * Put String to console
 * Write a string of ASCII characters to the console display. the characters are contained in consecutive memoery locations, one character per
 * memory location, Starting with the addr specified in R0. writing terminates when 0x0000 occures in a memoery that null terminated string occur.
 *
 * If we notice each ASCII chacter is stored as single char in 16bit form, to display char to console we will ned to cast it to char
 */
uint16_t op_trap_puts(uint16_t instruction);

/* 
 * Print prompt on the screen & read single character from keyboard.
 * The character is echoed onto console monitor, and its ASCII code is copied into R0. the high bits are cleared
*/
uint16_t op_trap_in(uint16_t instruction);

/* Write string of ASCII character to the console. The characters are containerd in consecutive memory locations, two character per memory location
 * starting from memory address stored in R0
 * first we will print ASCII char in [7:0] then [15:8]
 * character string of odd length will have 0x00 in bits [15:8]
 * writting terminated with occurrences of 0x0000 in memory location
 */
uint16_t op_trap_putsp(uint16_t instruction);

/* halt the program */
uint16_t op_trap_halt(uint16_t instruction);

#endif


