//
// Created by Saadat Baig on 28.12.20.
//
#ifndef SPOOF_DYLIB_MACH_MACHINE_UNIFIED_H
#define SPOOF_DYLIB_MACH_MACHINE_UNIFIED_H


// --------------------*  imports  *--------------------
#include <stdint.h>     // int32_t, uint32_t, uint8_t


// --------------------*  fancy redefs for apple's types  *--------------------
typedef int32_t cpu_type_t;
typedef int32_t cpu_subtype_t;


// --------------------*  structs  *--------------------
typedef struct __mach_header {
    uint32_t	magic;		/* mach magic number identifier */
    cpu_type_t	cputype;	/* cpu specifier */
    cpu_subtype_t	cpusubtype;	/* machine specifier */
    uint32_t	filetype;	/* type of file */
    uint32_t	ncmds;		/* number of load commands */
    uint32_t	sizeofcmds;	/* the size of all the load commands */
    uint32_t	flags;		/* flags */
}mach_header_t;

typedef struct __mach_header_64 {
    uint32_t	magic;		/* mach magic number identifier */
    cpu_type_t	cputype;	/* cpu specifier */
    cpu_subtype_t	cpusubtype;	/* machine specifier */
    uint32_t	filetype;	/* type of file */
    uint32_t	ncmds;		/* number of load commands */
    uint32_t	sizeofcmds;	/* the size of all the load commands */
    uint32_t	flags;		/* flags */
    uint32_t	reserved;	/* reserved */
}mach_header_64_t;

typedef struct __fat_header {
    uint32_t	magic;		/* FAT_MAGIC or FAT_MAGIC_64 */
    uint32_t	nfat_arch;	/* number of structs that follow */
}fat_header_t;

typedef struct __fat_arch {
    cpu_type_t	cputype;	/* cpu specifier (int) */
    cpu_subtype_t	cpusubtype;	/* machine specifier (int) */
    uint32_t	offset;		/* file offset to this object file */
    uint32_t	size;		/* size of this object file */
    uint32_t	align;		/* alignment as a power of 2 */
}fat_arch_t;

typedef struct __load_command {
    uint32_t cmd;		/* type of load command */
    uint32_t cmdsize;	/* total size of command in bytes */
}load_command_t;

typedef struct __uuid_command {
    uint32_t	cmd;		/* LC_UUID */
    uint32_t	cmdsize;	/* sizeof(struct uuid_command) */
    uint8_t	uuid[16];	/* the 128-bit uuid */
}uuid_command_t;


// --------------------*  defs  *--------------------
#define	MH_MAGIC	0xfeedface	/* the mach magic number */
#define MH_CIGAM	0xcefaedfe	/* NXSwapInt(MH_MAGIC) */
#define MH_MAGIC_64 0xfeedfacf /* the 64-bit mach magic number */
#define MH_CIGAM_64 0xcffaedfe /* NXSwapInt(MH_MAGIC_64) */
#define FAT_MAGIC	0xcafebabe  /* the fat magic number */
#define FAT_CIGAM	0xbebafeca	/* NXSwapLong(FAT_MAGIC) */
#define LC_UUID		0x1b	/* the uuid */


// --------------------*  machine/machine.h defs  *--------------------
#define CPU_ARCH_ABI64          0x01000000      /* 64 bit ABI */
#define CPU_TYPE_X86            ((cpu_type_t) 7)


#endif //SPOOF_DYLIB_MACH_MACHINE_UNIFIED_H