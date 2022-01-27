#include "flash_hal.h"

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    //
    // No special init required
    //
    return (0);
}

uint32_t EraseSector(uint32_t adr)
{
    /*FLASH_EraseInitTypeDef erase_init;
    uint32_t error;
    uint32_t ret = 0;  // O.K.

    HAL_FLASH_Unlock();
    
    memset(&erase_init, 0, sizeof(erase_init));
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_init.PageAddress = adr;
    erase_init.NbPages = 1;
    if (HAL_FLASHEx_Erase(&erase_init, &error) != HAL_OK) {
        ret = 1;
    }

    HAL_FLASH_Lock();
    return ret;*/

    return 1;
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    /*uint32_t i;
    uint32_t ret = 0;  // O.K.

    HAL_FLASH_Unlock();

    util_assert(sz % 4 == 0);
    for (i = 0; i < sz / 4; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, adr + i * 4, buf[i]) != HAL_OK) {
            ret = 1;
            break;
        }
    }

    HAL_FLASH_Lock();
    return ret;*/
    return 1;
}