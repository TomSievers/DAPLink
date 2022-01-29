#ifndef FLASH_HAL_H_
#define FLASH_HAL_H_

#include <stdint.h>
#include <stdlib.h>

#define FLASH_SECTOR_SIZE 4096
#define FLASH_PAGE_SIZE 256

typedef void (*connect_flash_t)(void); 

typedef void (*flash_exit_xip_t)(void); 

typedef void (*flash_range_erase_t)(uint32_t addr, size_t count, uint32_t block_size, uint8_t block_cmd); 

typedef void (*flash_range_program_t)(uint32_t addr, const uint8_t* data, size_t count);

typedef void (*flash_flush_cache_t)(void);

int flash_range_erase(uint32_t adr, uint32_t count);

int flash_range_program(uint32_t adr, const uint32_t* data, uint32_t count);

#endif //FLASH_HAL_H_