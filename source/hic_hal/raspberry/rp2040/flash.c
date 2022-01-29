#include "flash_hal_.h"

#pragma message "STUB FLASH"

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    //
    // No special init required
    //
    return (0);
}

uint32_t EraseSector(uint32_t adr)
{
    if(flash_range_erase(adr, 4096)) {
        return 1;
    }
    
    return 0;
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    if(flash_range_program(adr, buf, sz)) {
        return 1;
    }
    return 0;
}