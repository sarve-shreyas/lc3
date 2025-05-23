#include <stdint.h>
#include "bit-utilities.h"

// Convert Big Endian to little endian
uint16_t swap16(uint16_t x) {
  return (x << 8) | (x >> 8);
}

/*
 * since we are using 16bits to do all the operation we need to extend bits lower that this to 16 so that we can do operations like add, bit etc
 * for positive integer we just need to add 0's to the start of the value but for negative number we can to that
 * ex. in 5 bits -1 is represented as 1 1111 but if we extend this with positive number method then this will be 0000 0000 0001 1111 which is = 31 != -1
 * so in negative number we will add 1's instead of 0's
 * ====================================
  5-bit Two's Complement Summary
====================================

- Two's complement is a method of representing signed integers in binary.
- In N-bit two's complement:
    • The MSB (most significant bit) is the sign bit.
    • If MSB = 0 → the value is non-negative.
    • If MSB = 1 → the value is negative, and you must take the two's complement to get the magnitude.

------------------------------------
  Example: Representing -1 in 5 bits
------------------------------------
Step 1: Start with binary of +1     → 00001
Step 2: Invert the bits (1's comp)  → 11110
Step 3: Add 1                       → 11111

→ Final 5-bit binary for -1         → 11111

------------------------------------
  Decoding 5-bit Two's Complement
------------------------------------
1. If MSB is 0 → read value normally.
2. If MSB is 1 → subtract 2^5 = 32 from the unsigned value.

Example:
  11111 (binary) = 31 (unsigned)
  31 - 32 = -1 → interpreted as -1

------------------------------------
  5-bit Signed Integer Range
------------------------------------
  Binary Range   → Decimal Range
  00000          →     0
  ...
  01111          →   +15
  10000          →   -16
  ...
  11111          →    -1
*/
/*
 * We need to prepend 1's to the number we can do this using left shifting 2^16-1 to bit_cout & taking bitwise or with it
 * 0xFFFF = 1111 1111 1111 1111 << 5 = 1111 1111 1110 0000
 *
 * 		-------------------------------
 * 		0000 0000 0001 1111 (5 bit -1)
 * 		1111 1111 1110 0000 ((2^16 -1) << 5)
 * 		-------------------------------
 * 		1111 1111 1111 1111 (16 bit -1)
 *
 */

uint16_t sign_extend(uint16_t x, int bit_count) {
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

