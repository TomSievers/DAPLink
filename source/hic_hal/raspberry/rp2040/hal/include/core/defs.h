/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_PLATFORM_DEFS_H
#define _HARDWARE_PLATFORM_DEFS_H

#include <stdint.h>
#include <stdbool.h>

#ifndef __force_inline
#define __force_inline
#endif

#ifndef _u
#ifdef __ASSEMBLER__
#define _u(x) x
#else
#define _u(x) x ## u
#endif
#endif

#define NUM_CORES _u(2)
#define NUM_DMA_CHANNELS _u(12)
#define NUM_DMA_TIMERS _u(4)
#define NUM_IRQS _u(32)
#define NUM_PIOS _u(2)
#define NUM_PIO_STATE_MACHINES _u(4)
#define NUM_PWM_SLICES _u(8)
#define NUM_SPIN_LOCKS _u(32)
#define NUM_UARTS _u(2)
#define NUM_I2CS _u(2)
#define NUM_SPIS _u(2)
#define NUM_TIMERS _u(4)
#define NUM_ADC_CHANNELS _u(5)

#define NUM_BANK0_GPIOS _u(30)
#define NUM_QSPI_GPIOS _u(6)

#define PIO_INSTRUCTION_COUNT _u(32)

// PICO_CONFIG: XOSC_MHZ, The crystal oscillator frequency in Mhz, type=int, default=12, advanced=true, group=hardware_base
#ifndef XOSC_MHZ
#define XOSC_MHZ _u(12)
#endif

#define _REG_(x)

#pragma message "STUB MACROS"

#define invalid_params_if(x, test)
#define valid_params_if(x, test)
#define hard_assert_if(x, test)
#define hard_assert(x)

#define check_hw_layout(type, member, offset)
#define check_hw_size(type, size)
#define assert(x)
#define static_assert(x, y)

#define bool_to_bit(x) (x == true) ? 1 : 0 


#define __unused __attribute__((unused))

typedef volatile uint32_t io_rw_32;
typedef const volatile uint32_t io_ro_32;
typedef volatile uint32_t io_wo_32;
typedef volatile uint16_t io_rw_16;
typedef const volatile uint16_t io_ro_16;
typedef volatile uint16_t io_wo_16;
typedef volatile uint8_t io_rw_8;
typedef const volatile uint8_t io_ro_8;
typedef volatile uint8_t io_wo_8;

typedef volatile uint8_t *const ioptr;
typedef ioptr const const_ioptr;

typedef unsigned int uint;

#endif

