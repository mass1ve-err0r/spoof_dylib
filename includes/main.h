//
// Created by Saadat Baig on 28.12.20.
//
#ifndef SPOOF_DYLIB_MAIN_H
#define SPOOF_DYLIB_MAIN_H

// debug flag for more verbose output
#define DEBUG 0
// spoof_dylib version constant
#define SPOOF_DYLIB_VERSION "1.0"


// --------------------*  imports  *--------------------
#include "../includes/mach+machine_unified.h"   // imports stdint.h | contains mach + machine defs
#include "../includes/log_utils.h"              // import stdio.h | contains log funcs
#include <stdlib.h>                             // atoi, exit, fopen, fread, fwrite, fseek
#include <string.h>                             // strlen
#include <errno.h>                              // errno


// --------------------*  structs + vars  *--------------------
enum SD_ERROS {
    NO_ERROR,
    INVALID_ARGUMENT_NUMBER,
    INVALID_GIVEN_UUID,
    DYLIB_OPEN_FAILURE,
    INVALID_MAGIC,
    SEGMENT_NOT_FOUND
};

uint8_t replacement_uuid[16] = {0};
FILE *dylib_handle = NULL;


// --------------------*  macros  *--------------------
#define IS_LITTLE_ENDIAN(x) ((x) == FAT_CIGAM || (x) == MH_CIGAM_64 || (x) == MH_CIGAM)
#define OS_INDEPENDENT_SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))
#define SWAP32(x, magic) (IS_LITTLE_ENDIAN(magic)? OS_INDEPENDENT_SWAP32(x): (x))


#endif //SPOOF_DYLIB_MAIN_H