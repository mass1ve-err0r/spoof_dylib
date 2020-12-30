//
// Created by Saadat Baig on 28.12.20.
//
#include "../includes/main.h"


__attribute__((noreturn)) void help(void) {
    LOG_INFO("spoof_dylib version %s", SPOOF_DYLIB_VERSION);
    LOG_INFO("Usage: spoof_dylib <absolute_dylib_path> <new_hex_uuid>\n");
    exit(INVALID_ARGUMENT_NUMBER);
}

void overwrite_uuid() {
    fseek(dylib_handle, -16, SEEK_CUR);
    for (int i = 0; i < 16; ++i) {
        uint8_t *nv = malloc(sizeof(uint8_t));
        *nv = replacement_uuid[i];
#if DEBUG
        LOG_DEBUG("idx: %d | nv: 0x%x", i, *nv);
#endif
        fwrite(nv, sizeof(uint8_t), 1, dylib_handle);
        free(nv);
    }
}

void verify_uuid() {
    uint8_t check[16] = {0};
    fseek(dylib_handle, -16, SEEK_CUR);
    for (int i = 0; i < 16; ++i) {
        uint8_t nv = 0;
        fread(&nv, sizeof(uint8_t), 1, dylib_handle);
        check[i] = nv;
    }
    LOG_INFO("New GUID / UUID: %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
             check[0], check[1], check[2], check[3],
             check[4], check[5],
             check[6], check[7],
             check[8], check[9],
             check[10], check[11], check[12], check[13], check[14], check[15]);
}

void parse_uuid(char* uuid_string) {
    if (strlen(uuid_string) != 32) {
        LOG_ERROR("Invalid UUID! Aborting...");
        exit(INVALID_GIVEN_UUID);
    }
    int idx1 = 0, idx2 = 1;
    for (int i = 0; i < 16; ++i) {
        char* temp = (char[]) {*(uuid_string+idx1), *(uuid_string+idx2), '\0'};
#if DEBUG
        LOG_DEBUG("char1: %c | char2: %c |=> str: %s", *(uuid_string+idx1), *(uuid_string+idx2), temp);
#endif
        replacement_uuid[i] = (uint8_t)atoi(temp);
        idx1 += 2;
        idx2 += 2;
    }
    LOG_SUCCESS("Parsed GUID / UUID: %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                replacement_uuid[0], replacement_uuid[1], replacement_uuid[2], replacement_uuid[3],
                replacement_uuid[4], replacement_uuid[5],
                replacement_uuid[6], replacement_uuid[7],
                replacement_uuid[8], replacement_uuid[9],
                replacement_uuid[10], replacement_uuid[11], replacement_uuid[12], replacement_uuid[13], replacement_uuid[14], replacement_uuid[15]);
}

void find_uuid_segment(size_t offset, cpu_type_t cpu) {
    fseek(dylib_handle, offset, SEEK_SET);
    mach_header_64_t hdr;
    if ((cpu & CPU_ARCH_ABI64) == 0x1000000) {
#if DEBUG
        LOG_DEBUG("slice is 64-bit.");
#endif
        fread(&hdr, sizeof(mach_header_64_t), 1, dylib_handle);
    } else {
#if DEBUG
        LOG_DEBUG("slice is 32-bit.");
#endif
        fread(&hdr, sizeof(mach_header_t), 1, dylib_handle);
    }
    uint32_t ncmdz = SWAP32(hdr.ncmds, hdr.magic);
    for (int i = 0; i < ncmdz; ++i) {
        load_command_t lc;
        fread(&lc, sizeof(lc), 1, dylib_handle);
        uint32_t cmd = SWAP32(lc.cmd, hdr.magic);
        uint32_t cmdsize = SWAP32(lc.cmdsize, hdr.magic);
#if DEBUG
        LOG_DEBUG("(%d / %d) cmd_type: 0x%x | cmd_sz: 0x%x / %d (bytes)", i+1, ncmdz, cmd, cmdsize, cmdsize);
#endif
        if (cmd == LC_UUID) {
            LOG_INFO("Found LC_UUID Segment! (LC %d of %d)", i+1, ncmdz);
#if DEBUG
            LOG_DEBUG("current_pos: 0x%lx", ftell(dylib_handle));
#endif
            fseek(dylib_handle, -8, SEEK_CUR);
            uuid_command_t dylib_uuid;
            fread(&dylib_uuid, sizeof(dylib_uuid), 1, dylib_handle);
            LOG_INFO("Old GUID / UUID: %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                     dylib_uuid.uuid[0], dylib_uuid.uuid[1], dylib_uuid.uuid[2], dylib_uuid.uuid[3],
                     dylib_uuid.uuid[4], dylib_uuid.uuid[5],
                     dylib_uuid.uuid[6], dylib_uuid.uuid[7],
                     dylib_uuid.uuid[8], dylib_uuid.uuid[9],
                     dylib_uuid.uuid[10], dylib_uuid.uuid[11], dylib_uuid.uuid[12], dylib_uuid.uuid[13], dylib_uuid.uuid[14], dylib_uuid.uuid[15]);
            overwrite_uuid();
            verify_uuid();
            fseek(dylib_handle, (cmdsize - sizeof(uuid_command_t)), SEEK_CUR);
        } else {
            fseek(dylib_handle, (cmdsize-8), SEEK_CUR);
        }
    }
}

void spoof_dylib(char* dylib_path) {
    int gtfo = 0;
    dylib_handle = fopen(dylib_path, "r+");
    if (dylib_handle == NULL) {
        char* error_msg = strerror(errno);
        LOG_ERROR("Could NOT open dylib: %s", error_msg);
        exit(DYLIB_OPEN_FAILURE);
    }
#if DEBUG
    LOG_DEBUG("opened dylib: %s", dylib_path);
#endif
    fseek(dylib_handle, 0, SEEK_SET);
    uint32_t magic;
    fread(&magic, sizeof (uint32_t), 1, dylib_handle);
    switch (magic) {
        case FAT_MAGIC:
        case FAT_CIGAM:
            LOG_INFO("Found FAT Magic!");
            fseek(dylib_handle, 0, SEEK_SET);
            fat_header_t fat_hdr;
            fread(&fat_hdr, sizeof(fat_hdr), 1, dylib_handle);
            uint32_t narchs = SWAP32(fat_hdr.nfat_arch, magic);
            fat_arch_t archs[2];
            fread(archs, sizeof(archs), 1, dylib_handle);
            for (int i = 0; i < narchs; ++i) {
                LOG_INFO("==> Processing Slice %d of %d", i+1, narchs);
                uint32_t offset = SWAP32(archs[i].offset, magic);
                find_uuid_segment(offset, SWAP32(archs[i].cputype, magic));
            }
            break;
        case MH_MAGIC_64:
        case MH_CIGAM_64:
            LOG_INFO("Found Mach 64-bit Magic!");
            find_uuid_segment(0, CPU_ARCH_ABI64);
            break;
        case MH_MAGIC:
        case MH_CIGAM:
            LOG_INFO("Found Mach 32-bit Magic!");
            find_uuid_segment(0, CPU_TYPE_X86);
            break;
        default:
            LOG_ERROR("Unknown Magic! Aborting...");
            gtfo = 1;
            break;
    }
    fclose(dylib_handle);

    if (gtfo == 1) {
        exit(INVALID_MAGIC);
    }
}


int main(int argc, char **argv) {
#if DEBUG
    LOG_DEBUG("argc: %d", argc);
#endif
    if (argc < 3) {
        help();
    }
    parse_uuid(argv[2]);
    spoof_dylib(argv[1]);
    LOG_SUCCESS("Successfully rewritten GUID / UUID!");
    return NO_ERROR;
}