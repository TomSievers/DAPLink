#ifndef BIT_OPS_H_
#define BIT_OPS_H_

#include <stdint.h>
#include "regs/addrmap.h"

#ifndef __force_inline
#define __force_inline
#endif

#define hw_alias_check_addr(addr) ((uintptr_t)(addr))

static __force_inline uint32_t xip_alias_check_addr(const void *addr) {
    uint32_t rc = (uintptr_t)addr;
    return rc;
}

#define hw_set_alias_untyped(addr) ((void *)(REG_ALIAS_SET_BITS | hw_alias_check_addr(addr)))
#define hw_clear_alias_untyped(addr) ((void *)(REG_ALIAS_CLR_BITS | hw_alias_check_addr(addr)))
#define hw_xor_alias_untyped(addr) ((void *)(REG_ALIAS_XOR_BITS | hw_alias_check_addr(addr)))
#define xip_noalloc_alias_untyped(addr) ((void *)(XIP_NOALLOC_BASE | xip_alias_check_addr(addr)))
#define xip_nocache_alias_untyped(addr) ((void *)(XIP_NOCACHE_BASE | xip_alias_check_addr(addr)))
#define xip_nocache_noalloc_alias_untyped(addr) ((void *)(XIP_NOCACHE_NOALLOC_BASE | xip_alias_check_addr(addr)))

// Typed conversion alias pointer generation macros
#define hw_set_alias(p) ((typeof(p))hw_set_alias_untyped(p))
#define hw_clear_alias(p) ((typeof(p))hw_clear_alias_untyped(p))
#define hw_xor_alias(p) ((typeof(p))hw_xor_alias_untyped(p))
#define xip_noalloc_alias(p) ((typeof(p))xip_noalloc_alias_untyped(p))
#define xip_nocache_alias(p) ((typeof(p))xip_nocache_alias_untyped(p))
#define xip_nocache_noalloc_alias(p) ((typeof(p))xip_nocache_noalloc_alias_untyped(p))

/*! \brief Atomically set the specified bits to 1 in a HW register
 *  \ingroup hardware_base
 *
 * \param addr Address of writable register
 * \param mask Bit-mask specifying bits to set
 */
__force_inline static void hw_set_bits(io_rw_32 *addr, uint32_t mask) {
    *(io_rw_32 *) hw_set_alias_untyped((volatile void *) addr) = mask;
}

/*! \brief Atomically clear the specified bits to 0 in a HW register
 *  \ingroup hardware_base
 *
 * \param addr Address of writable register
 * \param mask Bit-mask specifying bits to clear
 */
__force_inline static void hw_clear_bits(io_rw_32 *addr, uint32_t mask) {
    *(io_rw_32 *) hw_clear_alias_untyped((volatile void *) addr) = mask;
}

/*! \brief Atomically flip the specified bits in a HW register
 *  \ingroup hardware_base
 *
 * \param addr Address of writable register
 * \param mask Bit-mask specifying bits to invert
 */
__force_inline static void hw_xor_bits(io_rw_32 *addr, uint32_t mask) {
    *(io_rw_32 *) hw_xor_alias_untyped((volatile void *) addr) = mask;
}

/*! \brief Set new values for a sub-set of the bits in a HW register
 *  \ingroup hardware_base
 *
 * Sets destination bits to values specified in \p values, if and only if corresponding bit in \p write_mask is set
 *
 * Note: this method allows safe concurrent modification of *different* bits of
 * a register, but multiple concurrent access to the same bits is still unsafe.
 *
 * \param addr Address of writable register
 * \param values Bits values
 * \param write_mask Mask of bits to change
 */
__force_inline static void hw_write_masked(io_rw_32 *addr, uint32_t values, uint32_t write_mask) {
    hw_xor_bits(addr, (*addr ^ values) & write_mask);
}

static __force_inline void tight_loop_contents(void) {}

#endif //BIT_OPS_H_