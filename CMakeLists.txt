cmake_minimum_required(VERSION 2.8.9)
project (lc3)

set(SOURCE_FILES
    ./core/bit-utilities.c
    ./core/core.c
    ./core/input-buffering.c
    ./core/read-image.c
    instruction-set.c
    vm.c)

add_executable(lc3 ${SOURCE_FILES})
