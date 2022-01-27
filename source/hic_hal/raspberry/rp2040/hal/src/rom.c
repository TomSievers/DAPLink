#include "rom.h"

typedef void* (*rom_func_lookup_t)(uint16_t* table, uint32_t code);

#define rom_hword_as_ptr(rom_address) (void *)(uintptr_t)(*(uint16_t *)rom_address)

void* rom_func_lookup(uint32_t code)
{
    rom_func_lookup_t func = (rom_func_lookup_t)rom_hword_as_ptr(0x18);
    uint16_t *func_table = (uint16_t *) rom_hword_as_ptr(0x14);
    return func(func_table, code);
}