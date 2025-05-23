#include<stdint.h>
#include<stdio.h>

#include "read-image.h"
#include "core.h"
#include "bit-utilities.h"

/*
 * When Program is conveted to machine code the result is file containing array of instructions and data
 * this can be loaded by just copying the contents right into an address in memoery
 * The First 16bits of the program file specify where the address in memoery where the program should start -> origin
 * this must be read first after rest of the file is loaded into memory starting from origin address
 */
void read_image_file(FILE* file) {
    uint16_t origin;
    fread(&origin, sizeof(origin), 1, file);

    origin = swap16(origin);

    uint16_t max_read = MEMORY_MAX - origin;
    uint16_t* p = memory + origin;
    size_t read = fread(p, sizeof(uint16_t), max_read, file);
        while(read-- >0){
        *p = swap16(*p);
        ++p;
    }
}
/*
 * Function to read image file
 */
uint16_t read_image(const char* image_path) {
    FILE* file = fopen(image_path, "rb");
    if (!file) {
        return 0;
    }
    read_image_file(file);
    fclose(file);
    return 1;
}


