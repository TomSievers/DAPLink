/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_PLATFORM_DEFS_H
#define _HARDWARE_PLATFORM_DEFS_H

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

#ifndef PICO_STACK_SIZE
#define PICO_STACK_SIZE _u(0x800)
#endif

#ifndef PICO_HEAP_SIZE
#define PICO_HEAP_SIZE _u(0x800)
#endif

#pragma message "STUB MACROS"

#define invalid_params_if(x, test)
#define valid_params_if(x, test)
#define hard_assert_if(x, test)
#define hard_assert(x)

#define check_hw_layout(type, member, offset)
#define check_hw_size(type, size)
#define assert(x)
#define static_assert(x, y)

#define bool_to_bit(x) (x != 0) ? 1 : 0 

#ifndef __unused
#define __unused __attribute__((unused))
#endif

#endif

