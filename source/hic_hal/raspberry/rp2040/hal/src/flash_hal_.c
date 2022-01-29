#include "flash_hal_.h"
#include "rom.h"
#include "regs/addrmap.h"
#include <stdbool.h>

#define BOOT2_SIZE_WORDS 64

static uint32_t boot2_copyout[BOOT2_SIZE_WORDS];
static bool boot2_copyout_valid = false;

#define flash_enable_xip_boot2() ((void (*)(void))boot2_copyout+1)()

__force_inline static void __compiler_memory_barrier(void) {
    __asm__ volatile ("" : : : "memory");
}

void flash_copy_boot2_ram()
{
    if (boot2_copyout_valid)
        return;
    for (int i = 0; i < BOOT2_SIZE_WORDS; ++i)
        boot2_copyout[i] = ((uint32_t *)XIP_BASE)[i];
    __compiler_memory_barrier();
    boot2_copyout_valid = true;
}

int flash_range_erase(uint32_t adr, uint32_t count)
{
    if(count & (FLASH_SECTOR_SIZE - 1) || adr & (FLASH_SECTOR_SIZE - 1))
    {
        return 0;
    }
    connect_flash_t connect_flash = (connect_flash_t)rom_func_lookup(rom_func_code('I', 'F'));
    flash_exit_xip_t exit_xip = (flash_exit_xip_t)rom_func_lookup(rom_func_code('E', 'X'));
    flash_range_erase_t range_erase = (flash_range_erase_t)rom_func_lookup(rom_func_code('R', 'E'));
    flash_flush_cache_t flush_cache = (flash_flush_cache_t)rom_func_lookup(rom_func_code('F', 'C'));
    flash_copy_boot2_ram();

    __compiler_memory_barrier();

    connect_flash();
    exit_xip();
    range_erase(adr, count, 4096, 0xD8);
    flush_cache();
    flash_enable_xip_boot2();
    return 1;
}

int flash_range_program(uint32_t adr, const uint32_t* data, uint32_t count)
{
    if(count & (FLASH_PAGE_SIZE - 1) || adr & (FLASH_PAGE_SIZE - 1))
    {
        return 0;
    }
    connect_flash_t connect_flash = (connect_flash_t)rom_func_lookup(rom_func_code('I', 'F'));
    flash_exit_xip_t exit_xip = (flash_exit_xip_t)rom_func_lookup(rom_func_code('E', 'X'));
    flash_range_program_t range_program = (flash_range_program_t)rom_func_lookup(rom_func_code('R', 'P'));
    flash_flush_cache_t flush_cache = (flash_flush_cache_t)rom_func_lookup(rom_func_code('F', 'C'));
    flash_copy_boot2_ram();

    __compiler_memory_barrier();

    connect_flash();
    exit_xip();
    range_program(adr, (uint8_t*)data, count);
    flush_cache();
    flash_enable_xip_boot2();
    return 1;
}