#include "reset.h"
#include "bit_ops.h"

void reset_block(reset_block_t block)
{
    resets_hw->reset = (uint32_t)block;
    resets_hw->reset = 0x0;
    while(~resets_hw->reset_done & (uint32_t)block)
        tight_loop_contents();
}