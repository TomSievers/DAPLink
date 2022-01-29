#ifndef ROM_H_
#define ROM_H_

#include <stdint.h>
#include "core/defs.h"

void* rom_func_lookup(uint32_t code);

uint32_t __force_inline rom_func_code(const char c1, const char c2)
{
    return c1 | (c2 << 8);
}

#endif