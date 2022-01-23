/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met: 
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _HARDWARE_GPIO_H_
#define _HARDWARE_GPIO_H_

#include "addrmap.h"
#include "gpio_regs.h"

/** \file gpio.h
 *  \defgroup hardware_gpio hardware_gpio
 *
 * General Purpose Input/Output (GPIO) API
 *
 * RP2040 has 36 multi-functional General Purpose Input / Output (GPIO) pins, divided into two banks. In a typical use case,
 * the pins in the QSPI bank (QSPI_SS, QSPI_SCLK and QSPI_SD0 to QSPI_SD3) are used to execute code from an external
 * flash device, leaving the User bank (GPIO0 to GPIO29) for the programmer to use. All GPIOs support digital input and
 * output, but GPIO26 to GPIO29 can also be used as inputs to the chip’s Analogue to Digital Converter (ADC). Each GPIO
 * can be controlled directly by software running on the processors, or by a number of other functional blocks.
 *
 * The function allocated to each GPIO is selected by calling the \ref gpio_set_function function. \note Not all functions
 * are available on all pins.
 *
 * Each GPIO can have one function selected at a time. Likewise, each peripheral input (e.g. UART0 RX) should only be selected on
 * one _GPIO_ at a time. If the same peripheral input is connected to multiple GPIOs, the peripheral sees the logical OR of these
 * GPIO inputs. Please refer to the datasheet for more information on GPIO function select.
 *
 * ### Function Select Table
 *
 *  GPIO   | F1       | F2        | F3       | F4     | F5  | F6   | F7   | F8            | F9
 *  -------|----------|-----------|----------|--------|-----|------|------|---------------|----
 *  0      | SPI0 RX  | UART0 TX  | I2C0 SDA | PWM0 A | SIO | PIO0 | PIO1 |               | USB OVCUR DET
 *  1      | SPI0 CSn | UART0 RX  | I2C0 SCL | PWM0 B | SIO | PIO0 | PIO1 |               | USB VBUS DET
 *  2      | SPI0 SCK | UART0 CTS | I2C1 SDA | PWM1 A | SIO | PIO0 | PIO1 |               | USB VBUS EN
 *  3      | SPI0 TX  | UART0 RTS | I2C1 SCL | PWM1 B | SIO | PIO0 | PIO1 |               | USB OVCUR DET
 *  4      | SPI0 RX  | UART1 TX  | I2C0 SDA | PWM2 A | SIO | PIO0 | PIO1 |               | USB VBUS DET
 *  5      | SPI0 CSn | UART1 RX  | I2C0 SCL | PWM2 B | SIO | PIO0 | PIO1 |               | USB VBUS EN
 *  6      | SPI0 SCK | UART1 CTS | I2C1 SDA | PWM3 A | SIO | PIO0 | PIO1 |               | USB OVCUR DET
 *  7      | SPI0 TX  | UART1 RTS | I2C1 SCL | PWM3 B | SIO | PIO0 | PIO1 |               | USB VBUS DET
 *  8      | SPI1 RX  | UART1 TX  | I2C0 SDA | PWM4 A | SIO | PIO0 | PIO1 |               | USB VBUS EN
 *  9      | SPI1 CSn | UART1 RX  | I2C0 SCL | PWM4 B | SIO | PIO0 | PIO1 |               | USB OVCUR DET
 *  10     | SPI1 SCK | UART1 CTS | I2C1 SDA | PWM5 A | SIO | PIO0 | PIO1 |               | USB VBUS DET
 *  11     | SPI1 TX  | UART1 RTS | I2C1 SCL | PWM5 B | SIO | PIO0 | PIO1 |               | USB VBUS EN
 *  12     | SPI1 RX  | UART0 TX  | I2C0 SDA | PWM6 A | SIO | PIO0 | PIO1 |               | USB OVCUR DET
 *  13     | SPI1 CSn | UART0 RX  | I2C0 SCL | PWM6 B | SIO | PIO0 | PIO1 |               | USB VBUS DET
 *  14     | SPI1 SCK | UART0 CTS | I2C1 SDA | PWM7 A | SIO | PIO0 | PIO1 |               | USB VBUS EN
 *  15     | SPI1 TX  | UART0 RTS | I2C1 SCL | PWM7 B | SIO | PIO0 | PIO1 |               | USB OVCUR DET
 *  16     | SPI0 RX  | UART0 TX  | I2C0 SDA | PWM0 A | SIO | PIO0 | PIO1 |               | USB VBUS DET
 *  17     | SPI0 CSn | UART0 RX  | I2C0 SCL | PWM0 B | SIO | PIO0 | PIO1 |               | USB VBUS EN
 *  18     | SPI0 SCK | UART0 CTS | I2C1 SDA | PWM1 A | SIO | PIO0 | PIO1 |               | USB OVCUR DET
 *  19     | SPI0 TX  | UART0 RTS | I2C1 SCL | PWM1 B | SIO | PIO0 | PIO1 |               | USB VBUS DET
 *  20     | SPI0 RX  | UART1 TX  | I2C0 SDA | PWM2 A | SIO | PIO0 | PIO1 | CLOCK GPIN0   | USB VBUS EN
 *  21     | SPI0 CSn | UART1 RX  | I2C0 SCL | PWM2 B | SIO | PIO0 | PIO1 | CLOCK GPOUT0  | USB OVCUR DET
 *  22     | SPI0 SCK | UART1 CTS | I2C1 SDA | PWM3 A | SIO | PIO0 | PIO1 | CLOCK GPIN1   | USB VBUS DET
 *  23     | SPI0 TX  | UART1 RTS | I2C1 SCL | PWM3 B | SIO | PIO0 | PIO1 | CLOCK GPOUT1  | USB VBUS EN
 *  24     | SPI1 RX  | UART1 TX  | I2C0 SDA | PWM4 A | SIO | PIO0 | PIO1 | CLOCK GPOUT2  | USB OVCUR DET
 *  25     | SPI1 CSn | UART1 RX  | I2C0 SCL | PWM4 B | SIO | PIO0 | PIO1 | CLOCK GPOUT3  | USB VBUS DET
 *  26     | SPI1 SCK | UART1 CTS | I2C1 SDA | PWM5 A | SIO | PIO0 | PIO1 |               | USB VBUS EN
 *  27     | SPI1 TX  | UART1 RTS | I2C1 SCL | PWM5 B | SIO | PIO0 | PIO1 |               | USB OVCUR DET
 *  28     | SPI1 RX  | UART0 TX  | I2C0 SDA | PWM6 A | SIO | PIO0 | PIO1 |               | USB VBUS DET
 *  29     | SPI1 CSn | UART0 RX  | I2C0 SCL | PWM6 B | SIO | PIO0 | PIO1 |               | USB VBUS EN

 */

/*! \brief  GPIO function definitions for use with function select
 *  \ingroup hardware_gpio
 * \brief GPIO function selectors
 *
 * Each GPIO can have one function selected at a time. Likewise, each peripheral input (e.g. UART0 RX) should only be
 * selected on one GPIO at a time. If the same peripheral input is connected to multiple GPIOs, the peripheral sees the logical
 * OR of these GPIO inputs.
 *
 * Please refer to the datsheet for more information on GPIO function selection.
 */
enum gpio_function {
    GPIO_FUNC_XIP = 0,
    GPIO_FUNC_SPI = 1,
    GPIO_FUNC_UART = 2,
    GPIO_FUNC_I2C = 3,
    GPIO_FUNC_PWM = 4,
    GPIO_FUNC_SIO = 5,
    GPIO_FUNC_PIO0 = 6,
    GPIO_FUNC_PIO1 = 7,
    GPIO_FUNC_GPCK = 8,
    GPIO_FUNC_USB = 9,
    GPIO_FUNC_NULL = 0x1f,
};

#define GPIO_OUT 1
#define GPIO_IN 0

/*! \brief  GPIO Interrupt level definitions
 *  \ingroup hardware_gpio
 *  \brief GPIO Interrupt levels
 *
 * An interrupt can be generated for every GPIO pin in 4 scenarios:
 *
 * * Level High: the GPIO pin is a logical 1
 * * Level Low: the GPIO pin is a logical 0
 * * Edge High: the GPIO has transitioned from a logical 0 to a logical 1
 * * Edge Low: the GPIO has transitioned from a logical 1 to a logical 0
 *
 * The level interrupts are not latched. This means that if the pin is a logical 1 and the level high interrupt is active, it will
 * become inactive as soon as the pin changes to a logical 0. The edge interrupts are stored in the INTR register and can be
 * cleared by writing to the INTR register.
 */
enum gpio_irq_level {
    GPIO_IRQ_LEVEL_LOW = 0x1u,
    GPIO_IRQ_LEVEL_HIGH = 0x2u,
    GPIO_IRQ_EDGE_FALL = 0x4u,
    GPIO_IRQ_EDGE_RISE = 0x8u,
};

/*! Callback function type for GPIO events
 *  \ingroup hardware_gpio
 *
 * \param gpio Which GPIO caused this interrupt
 * \param events Which events caused this interrupt. See \ref gpio_set_irq_enabled for details.
 * \sa gpio_set_irq_enabled_with_callback()
 */
typedef void (*gpio_irq_callback_t)(uint gpio, uint32_t events);

enum gpio_override {
    GPIO_OVERRIDE_NORMAL = 0,      ///< peripheral signal selected via \ref gpio_set_function
    GPIO_OVERRIDE_INVERT = 1,      ///< invert peripheral signal selected via \ref gpio_set_function
    GPIO_OVERRIDE_LOW = 2,         ///< drive low/disable output
    GPIO_OVERRIDE_HIGH = 3,        ///< drive high/enable output
};

/*! \brief Slew rate limiting levels for GPIO outputs
 *  \ingroup hardware_gpio
 *
 * Slew rate limiting increases the minimum rise/fall time when a GPIO output
 * is lightly loaded, which can help to reduce electromagnetic emissions.
 * \sa gpio_set_slew_rate
 */
enum gpio_slew_rate {
    GPIO_SLEW_RATE_SLOW = 0,  ///< Slew rate limiting enabled
    GPIO_SLEW_RATE_FAST = 1   ///< Slew rate limiting disabled
};

/*! \brief Drive strength levels for GPIO outputs
 *  \ingroup hardware_gpio
 *
 * Drive strength levels for GPIO outputs.
 * \sa gpio_set_drive_strength
 */
enum gpio_drive_strength {
    GPIO_DRIVE_STRENGTH_2MA = 0, ///< 2 mA nominal drive strength
    GPIO_DRIVE_STRENGTH_4MA = 1, ///< 4 mA nominal drive strength
    GPIO_DRIVE_STRENGTH_8MA = 2, ///< 8 mA nominal drive strength
    GPIO_DRIVE_STRENGTH_12MA = 3 ///< 12 mA nominal drive strength
};

#define NUM_BANK0_GPIOS 30

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_pads_bank0
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/pads_bank0.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(PADS_BANK0_VOLTAGE_SELECT_OFFSET) // PADS_BANK0_VOLTAGE_SELECT
    // Voltage select
    // 0x00000001 [0]     : VOLTAGE_SELECT (0)
    io_rw_32 voltage_select;

    _REG_(PADS_BANK0_GPIO0_OFFSET) // PADS_BANK0_GPIO0
    // (Description copied from array index 0 register PADS_BANK0_GPIO0 applies similarly to other array indexes)
    //
    // Pad control register
    // 0x00000080 [7]     : OD (0): Output disable
    // 0x00000040 [6]     : IE (1): Input enable
    // 0x00000030 [5:4]   : DRIVE (1): Drive strength
    // 0x00000008 [3]     : PUE (0): Pull up enable
    // 0x00000004 [2]     : PDE (1): Pull down enable
    // 0x00000002 [1]     : SCHMITT (1): Enable schmitt trigger
    // 0x00000001 [0]     : SLEWFAST (0): Slew rate control
    io_rw_32 io[NUM_BANK0_GPIOS]; // 30
} padsbank0_hw_t;

#define padsbank0_hw ((padsbank0_hw_t *const)PADS_BANK0_BASE)

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_sio
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/sio.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(SIO_INTERP0_ACCUM0_OFFSET) // SIO_INTERP0_ACCUM0
    // (Description copied from array index 0 register SIO_INTERP0_ACCUM0 applies similarly to other array indexes)
    //
    // Read/write access to accumulator 0
    io_rw_32 accum[2];

    _REG_(SIO_INTERP0_BASE0_OFFSET) // SIO_INTERP0_BASE0
    // (Description copied from array index 0 register SIO_INTERP0_BASE0 applies similarly to other array indexes)
    //
    // Read/write access to BASE0 register
    io_rw_32 base[3];

    _REG_(SIO_INTERP0_POP_LANE0_OFFSET) // SIO_INTERP0_POP_LANE0
    // (Description copied from array index 0 register SIO_INTERP0_POP_LANE0 applies similarly to other array indexes)
    //
    // Read LANE0 result, and simultaneously write lane results to both accumulators (POP)
    io_ro_32 pop[3];

    _REG_(SIO_INTERP0_PEEK_LANE0_OFFSET) // SIO_INTERP0_PEEK_LANE0
    // (Description copied from array index 0 register SIO_INTERP0_PEEK_LANE0 applies similarly to other array indexes)
    //
    // Read LANE0 result, without altering any internal state (PEEK)
    io_ro_32 peek[3];

    _REG_(SIO_INTERP0_CTRL_LANE0_OFFSET) // SIO_INTERP0_CTRL_LANE0
    // (Description copied from array index 0 register SIO_INTERP0_CTRL_LANE0 applies similarly to other array indexes)
    //
    // Control register for lane 0
    // 0x02000000 [25]    : OVERF (0): Set if either OVERF0 or OVERF1 is set
    // 0x01000000 [24]    : OVERF1 (0): Indicates if any masked-off MSBs in ACCUM1 are set
    // 0x00800000 [23]    : OVERF0 (0): Indicates if any masked-off MSBs in ACCUM0 are set
    // 0x00200000 [21]    : BLEND (0): Only present on INTERP0 on each core
    // 0x00180000 [20:19] : FORCE_MSB (0): ORed into bits 29:28 of the lane result presented to the processor on the bus
    // 0x00040000 [18]    : ADD_RAW (0): If 1, mask + shift is bypassed for LANE0 result
    // 0x00020000 [17]    : CROSS_RESULT (0): If 1, feed the opposite lane's result into this lane's accumulator on POP
    // 0x00010000 [16]    : CROSS_INPUT (0): If 1, feed the opposite lane's accumulator into this lane's shift + mask hardware
    // 0x00008000 [15]    : SIGNED (0): If SIGNED is set, the shifted and masked accumulator value is sign-extended to 32 bits
    // 0x00007c00 [14:10] : MASK_MSB (0): The most-significant bit allowed to pass by the mask (inclusive)
    // 0x000003e0 [9:5]   : MASK_LSB (0): The least-significant bit allowed to pass by the mask (inclusive)
    // 0x0000001f [4:0]   : SHIFT (0): Logical right-shift applied to accumulator before masking
    io_rw_32 ctrl[2];

    _REG_(SIO_INTERP0_ACCUM0_ADD_OFFSET) // SIO_INTERP0_ACCUM0_ADD
    // (Description copied from array index 0 register SIO_INTERP0_ACCUM0_ADD applies similarly to other array indexes)
    //
    // Values written here are atomically added to ACCUM0
    // 0x00ffffff [23:0]  : INTERP0_ACCUM0_ADD (0)
    io_rw_32 add_raw[2];

    _REG_(SIO_INTERP0_BASE_1AND0_OFFSET) // SIO_INTERP0_BASE_1AND0
    // On write, the lower 16 bits go to BASE0, upper bits to BASE1 simultaneously
    io_wo_32 base01;
} interp_hw_t;

#define interp_hw_array ((interp_hw_t *)(SIO_BASE + SIO_INTERP0_ACCUM0_OFFSET))
#define interp0_hw (&interp_hw_array[0])
#define interp1_hw (&interp_hw_array[1])

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_sio
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/sio.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(SIO_CPUID_OFFSET) // SIO_CPUID
    // Processor core identifier
    io_ro_32 cpuid;

    _REG_(SIO_GPIO_IN_OFFSET) // SIO_GPIO_IN
    // Input value for GPIO pins
    // 0x3fffffff [29:0]  : GPIO_IN (0): Input value for GPIO0
    io_ro_32 gpio_in;

    _REG_(SIO_GPIO_HI_IN_OFFSET) // SIO_GPIO_HI_IN
    // Input value for QSPI pins
    // 0x0000003f [5:0]   : GPIO_HI_IN (0): Input value on QSPI IO in order 0
    io_ro_32 gpio_hi_in;

    uint32_t _pad0;

    _REG_(SIO_GPIO_OUT_OFFSET) // SIO_GPIO_OUT
    // GPIO output value
    // 0x3fffffff [29:0]  : GPIO_OUT (0): Set output level (1/0 -> high/low) for GPIO0
    io_rw_32 gpio_out;

    _REG_(SIO_GPIO_OUT_SET_OFFSET) // SIO_GPIO_OUT_SET
    // GPIO output value set
    // 0x3fffffff [29:0]  : GPIO_OUT_SET (0): Perform an atomic bit-set on GPIO_OUT, i
    io_wo_32 gpio_set;

    _REG_(SIO_GPIO_OUT_CLR_OFFSET) // SIO_GPIO_OUT_CLR
    // GPIO output value clear
    // 0x3fffffff [29:0]  : GPIO_OUT_CLR (0): Perform an atomic bit-clear on GPIO_OUT, i
    io_wo_32 gpio_clr;

    _REG_(SIO_GPIO_OUT_XOR_OFFSET) // SIO_GPIO_OUT_XOR
    // GPIO output value XOR
    // 0x3fffffff [29:0]  : GPIO_OUT_XOR (0): Perform an atomic bitwise XOR on GPIO_OUT, i
    io_wo_32 gpio_togl;

    _REG_(SIO_GPIO_OE_OFFSET) // SIO_GPIO_OE
    // GPIO output enable
    // 0x3fffffff [29:0]  : GPIO_OE (0): Set output enable (1/0 -> output/input) for GPIO0
    io_rw_32 gpio_oe;

    _REG_(SIO_GPIO_OE_SET_OFFSET) // SIO_GPIO_OE_SET
    // GPIO output enable set
    // 0x3fffffff [29:0]  : GPIO_OE_SET (0): Perform an atomic bit-set on GPIO_OE, i
    io_wo_32 gpio_oe_set;

    _REG_(SIO_GPIO_OE_CLR_OFFSET) // SIO_GPIO_OE_CLR
    // GPIO output enable clear
    // 0x3fffffff [29:0]  : GPIO_OE_CLR (0): Perform an atomic bit-clear on GPIO_OE, i
    io_wo_32 gpio_oe_clr;

    _REG_(SIO_GPIO_OE_XOR_OFFSET) // SIO_GPIO_OE_XOR
    // GPIO output enable XOR
    // 0x3fffffff [29:0]  : GPIO_OE_XOR (0): Perform an atomic bitwise XOR on GPIO_OE, i
    io_wo_32 gpio_oe_togl;

    _REG_(SIO_GPIO_HI_OUT_OFFSET) // SIO_GPIO_HI_OUT
    // QSPI output value
    // 0x0000003f [5:0]   : GPIO_HI_OUT (0): Set output level (1/0 -> high/low) for QSPI IO0
    io_rw_32 gpio_hi_out;

    _REG_(SIO_GPIO_HI_OUT_SET_OFFSET) // SIO_GPIO_HI_OUT_SET
    // QSPI output value set
    // 0x0000003f [5:0]   : GPIO_HI_OUT_SET (0): Perform an atomic bit-set on GPIO_HI_OUT, i
    io_wo_32 gpio_hi_set;

    _REG_(SIO_GPIO_HI_OUT_CLR_OFFSET) // SIO_GPIO_HI_OUT_CLR
    // QSPI output value clear
    // 0x0000003f [5:0]   : GPIO_HI_OUT_CLR (0): Perform an atomic bit-clear on GPIO_HI_OUT, i
    io_wo_32 gpio_hi_clr;

    _REG_(SIO_GPIO_HI_OUT_XOR_OFFSET) // SIO_GPIO_HI_OUT_XOR
    // QSPI output value XOR
    // 0x0000003f [5:0]   : GPIO_HI_OUT_XOR (0): Perform an atomic bitwise XOR on GPIO_HI_OUT, i
    io_wo_32 gpio_hi_togl;

    _REG_(SIO_GPIO_HI_OE_OFFSET) // SIO_GPIO_HI_OE
    // QSPI output enable
    // 0x0000003f [5:0]   : GPIO_HI_OE (0): Set output enable (1/0 -> output/input) for QSPI IO0
    io_rw_32 gpio_hi_oe;

    _REG_(SIO_GPIO_HI_OE_SET_OFFSET) // SIO_GPIO_HI_OE_SET
    // QSPI output enable set
    // 0x0000003f [5:0]   : GPIO_HI_OE_SET (0): Perform an atomic bit-set on GPIO_HI_OE, i
    io_wo_32 gpio_hi_oe_set;

    _REG_(SIO_GPIO_HI_OE_CLR_OFFSET) // SIO_GPIO_HI_OE_CLR
    // QSPI output enable clear
    // 0x0000003f [5:0]   : GPIO_HI_OE_CLR (0): Perform an atomic bit-clear on GPIO_HI_OE, i
    io_wo_32 gpio_hi_oe_clr;

    _REG_(SIO_GPIO_HI_OE_XOR_OFFSET) // SIO_GPIO_HI_OE_XOR
    // QSPI output enable XOR
    // 0x0000003f [5:0]   : GPIO_HI_OE_XOR (0): Perform an atomic bitwise XOR on GPIO_HI_OE, i
    io_wo_32 gpio_hi_oe_togl;

    _REG_(SIO_FIFO_ST_OFFSET) // SIO_FIFO_ST
    // Status register for inter-core FIFOs (mailboxes)
    // 0x00000008 [3]     : ROE (0): Sticky flag indicating the RX FIFO was read when empty
    // 0x00000004 [2]     : WOF (0): Sticky flag indicating the TX FIFO was written when full
    // 0x00000002 [1]     : RDY (1): Value is 1 if this core's TX FIFO is not full (i
    // 0x00000001 [0]     : VLD (0): Value is 1 if this core's RX FIFO is not empty (i
    io_rw_32 fifo_st;

    _REG_(SIO_FIFO_WR_OFFSET) // SIO_FIFO_WR
    // Write access to this core's TX FIFO
    io_wo_32 fifo_wr;

    _REG_(SIO_FIFO_RD_OFFSET) // SIO_FIFO_RD
    // Read access to this core's RX FIFO
    io_ro_32 fifo_rd;

    _REG_(SIO_SPINLOCK_ST_OFFSET) // SIO_SPINLOCK_ST
    // Spinlock state
    io_ro_32 spinlock_st;

    _REG_(SIO_DIV_UDIVIDEND_OFFSET) // SIO_DIV_UDIVIDEND
    // Divider unsigned dividend
    io_rw_32 div_udividend;

    _REG_(SIO_DIV_UDIVISOR_OFFSET) // SIO_DIV_UDIVISOR
    // Divider unsigned divisor
    io_rw_32 div_udivisor;

    _REG_(SIO_DIV_SDIVIDEND_OFFSET) // SIO_DIV_SDIVIDEND
    // Divider signed dividend
    io_rw_32 div_sdividend;

    _REG_(SIO_DIV_SDIVISOR_OFFSET) // SIO_DIV_SDIVISOR
    // Divider signed divisor
    io_rw_32 div_sdivisor;

    _REG_(SIO_DIV_QUOTIENT_OFFSET) // SIO_DIV_QUOTIENT
    // Divider result quotient
    io_rw_32 div_quotient;

    _REG_(SIO_DIV_REMAINDER_OFFSET) // SIO_DIV_REMAINDER
    // Divider result remainder
    io_rw_32 div_remainder;

    _REG_(SIO_DIV_CSR_OFFSET) // SIO_DIV_CSR
    // Control and status register for divider
    // 0x00000002 [1]     : DIRTY (0): Changes to 1 when any register is written, and back to 0 when QUOTIENT is read
    // 0x00000001 [0]     : READY (1): Reads as 0 when a calculation is in progress, 1 otherwise
    io_ro_32 div_csr;
    uint32_t _pad1;
    interp_hw_t interp[2];
} sio_hw_t;

#define sio_hw ((sio_hw_t *const)SIO_BASE)

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_io_bank0
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/io_bank0.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(IO_BANK0_GPIO0_STATUS_OFFSET) // IO_BANK0_GPIO0_STATUS
    // GPIO status
    // 0x04000000 [26]    : IRQTOPROC (0): interrupt to processors, after override is applied
    // 0x01000000 [24]    : IRQFROMPAD (0): interrupt from pad before override is applied
    // 0x00080000 [19]    : INTOPERI (0): input signal to peripheral, after override is applied
    // 0x00020000 [17]    : INFROMPAD (0): input signal from pad, before override is applied
    // 0x00002000 [13]    : OETOPAD (0): output enable to pad after register override is applied
    // 0x00001000 [12]    : OEFROMPERI (0): output enable from selected peripheral, before register override is applied
    // 0x00000200 [9]     : OUTTOPAD (0): output signal to pad after register override is applied
    // 0x00000100 [8]     : OUTFROMPERI (0): output signal from selected peripheral, before register override is applied
    io_ro_32 status;

    _REG_(IO_BANK0_GPIO0_CTRL_OFFSET) // IO_BANK0_GPIO0_CTRL
    // GPIO control including function select and overrides
    // 0x30000000 [29:28] : IRQOVER (0)
    // 0x00030000 [17:16] : INOVER (0)
    // 0x00003000 [13:12] : OEOVER (0)
    // 0x00000300 [9:8]   : OUTOVER (0)
    // 0x0000001f [4:0]   : FUNCSEL (0x1f): 0-31 -> selects pin function according to the gpio table
    io_rw_32 ctrl;
} iobank0_status_ctrl_hw_t;

typedef struct {
    _REG_(IO_BANK0_PROC0_INTE0_OFFSET) // IO_BANK0_PROC0_INTE0
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTE0 applies similarly to other array indexes)
    //
    // Interrupt Enable for proc0
    // 0x80000000 [31]    : GPIO7_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO7_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO7_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO7_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO6_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO6_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO6_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO6_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO5_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO5_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO5_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO5_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO4_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO4_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO4_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO4_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO3_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO3_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO3_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO3_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO2_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO2_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO2_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO2_LEVEL_LOW (0)
    // 0x00000080 [7]     : GPIO1_EDGE_HIGH (0)
    // 0x00000040 [6]     : GPIO1_EDGE_LOW (0)
    // 0x00000020 [5]     : GPIO1_LEVEL_HIGH (0)
    // 0x00000010 [4]     : GPIO1_LEVEL_LOW (0)
    // 0x00000008 [3]     : GPIO0_EDGE_HIGH (0)
    // 0x00000004 [2]     : GPIO0_EDGE_LOW (0)
    // 0x00000002 [1]     : GPIO0_LEVEL_HIGH (0)
    // 0x00000001 [0]     : GPIO0_LEVEL_LOW (0)
    io_rw_32 inte[4];

    _REG_(IO_BANK0_PROC0_INTF0_OFFSET) // IO_BANK0_PROC0_INTF0
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTF0 applies similarly to other array indexes)
    //
    // Interrupt Force for proc0
    // 0x80000000 [31]    : GPIO7_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO7_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO7_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO7_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO6_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO6_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO6_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO6_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO5_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO5_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO5_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO5_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO4_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO4_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO4_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO4_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO3_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO3_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO3_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO3_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO2_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO2_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO2_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO2_LEVEL_LOW (0)
    // 0x00000080 [7]     : GPIO1_EDGE_HIGH (0)
    // 0x00000040 [6]     : GPIO1_EDGE_LOW (0)
    // 0x00000020 [5]     : GPIO1_LEVEL_HIGH (0)
    // 0x00000010 [4]     : GPIO1_LEVEL_LOW (0)
    // 0x00000008 [3]     : GPIO0_EDGE_HIGH (0)
    // 0x00000004 [2]     : GPIO0_EDGE_LOW (0)
    // 0x00000002 [1]     : GPIO0_LEVEL_HIGH (0)
    // 0x00000001 [0]     : GPIO0_LEVEL_LOW (0)
    io_rw_32 intf[4];

    _REG_(IO_BANK0_PROC0_INTS0_OFFSET) // IO_BANK0_PROC0_INTS0
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTS0 applies similarly to other array indexes)
    //
    // Interrupt status after masking & forcing for proc0
    // 0x80000000 [31]    : GPIO7_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO7_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO7_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO7_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO6_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO6_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO6_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO6_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO5_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO5_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO5_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO5_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO4_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO4_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO4_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO4_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO3_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO3_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO3_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO3_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO2_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO2_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO2_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO2_LEVEL_LOW (0)
    // 0x00000080 [7]     : GPIO1_EDGE_HIGH (0)
    // 0x00000040 [6]     : GPIO1_EDGE_LOW (0)
    // 0x00000020 [5]     : GPIO1_LEVEL_HIGH (0)
    // 0x00000010 [4]     : GPIO1_LEVEL_LOW (0)
    // 0x00000008 [3]     : GPIO0_EDGE_HIGH (0)
    // 0x00000004 [2]     : GPIO0_EDGE_LOW (0)
    // 0x00000002 [1]     : GPIO0_LEVEL_HIGH (0)
    // 0x00000001 [0]     : GPIO0_LEVEL_LOW (0)
    io_ro_32 ints[4];
} io_irq_ctrl_hw_t;

/// \tag::iobank0_hw[]
typedef struct {
    iobank0_status_ctrl_hw_t io[NUM_BANK0_GPIOS]; // 30

    _REG_(IO_BANK0_INTR0_OFFSET) // IO_BANK0_INTR0
    // (Description copied from array index 0 register IO_BANK0_INTR0 applies similarly to other array indexes)
    //
    // Raw Interrupts
    // 0x80000000 [31]    : GPIO7_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO7_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO7_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO7_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO6_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO6_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO6_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO6_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO5_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO5_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO5_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO5_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO4_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO4_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO4_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO4_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO3_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO3_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO3_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO3_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO2_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO2_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO2_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO2_LEVEL_LOW (0)
    // 0x00000080 [7]     : GPIO1_EDGE_HIGH (0)
    // 0x00000040 [6]     : GPIO1_EDGE_LOW (0)
    // 0x00000020 [5]     : GPIO1_LEVEL_HIGH (0)
    // 0x00000010 [4]     : GPIO1_LEVEL_LOW (0)
    // 0x00000008 [3]     : GPIO0_EDGE_HIGH (0)
    // 0x00000004 [2]     : GPIO0_EDGE_LOW (0)
    // 0x00000002 [1]     : GPIO0_LEVEL_HIGH (0)
    // 0x00000001 [0]     : GPIO0_LEVEL_LOW (0)
    io_rw_32 intr[4];

    io_irq_ctrl_hw_t proc0_irq_ctrl;

    io_irq_ctrl_hw_t proc1_irq_ctrl;

    io_irq_ctrl_hw_t dormant_wake_irq_ctrl;
} iobank0_hw_t;

#define iobank0_hw ((iobank0_hw_t *const)IO_BANK0_BASE)

// ----------------------------------------------------------------------------
// Pad Controls + IO Muxing
// ----------------------------------------------------------------------------
// Declarations for gpio.c

/*! \brief Select GPIO function
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param fn Which GPIO function select to use from list \ref gpio_function
 */
void gpio_set_function(uint gpio, enum gpio_function fn);

/*! \brief Determine current GPIO function
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \return Which GPIO function is currently selected from list \ref gpio_function
 */
enum gpio_function gpio_get_function(uint gpio);

/*! \brief Select up and down pulls on specific GPIO
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param up If true set a pull up on the GPIO
 * \param down If true set a pull down on the GPIO
 *
 * \note On the RP2040, setting both pulls enables a "bus keep" function,
 * i.e. a weak pull to whatever is current high/low state of GPIO.
 */
void gpio_set_pulls(uint gpio, bool up, bool down);

/*! \brief Set specified GPIO to be pulled up.
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 */
static inline void gpio_pull_up(uint gpio) {
    gpio_set_pulls(gpio, true, false);
}

/*! \brief Determine if the specified GPIO is pulled up.
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \return true if the GPIO is pulled up
 */
static inline bool gpio_is_pulled_up(uint gpio) {
    return (padsbank0_hw->io[gpio] & PADS_BANK0_GPIO0_PUE_BITS) != 0;
}

/*! \brief Set specified GPIO to be pulled down.
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 */
static inline void gpio_pull_down(uint gpio) {
    gpio_set_pulls(gpio, false, true);
}

/*! \brief Determine if the specified GPIO is pulled down.
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \return true if the GPIO is pulled down
 */
static inline bool gpio_is_pulled_down(uint gpio) {
    return (padsbank0_hw->io[gpio] & PADS_BANK0_GPIO0_PDE_BITS) != 0;
}

/*! \brief Disable pulls on specified GPIO
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 */
static inline void gpio_disable_pulls(uint gpio) {
    gpio_set_pulls(gpio, false, false);
}

/*! \brief Set GPIO IRQ override
 *  \ingroup hardware_gpio
 *
 * Optionally invert a GPIO IRQ signal, or drive it high or low
 *
 * \param gpio GPIO number
 * \param value See \ref gpio_override
 */
void gpio_set_irqover(uint gpio, uint value);

/*! \brief Set GPIO output override
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param value See \ref gpio_override
 */
void gpio_set_outover(uint gpio, uint value);

/*! \brief Select GPIO input override
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param value See \ref gpio_override
 */
void gpio_set_inover(uint gpio, uint value);

/*! \brief Select GPIO output enable override
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param value See \ref gpio_override
 */
void gpio_set_oeover(uint gpio, uint value);

/*! \brief Enable GPIO input
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param enabled true to enable input on specified GPIO
 */
void gpio_set_input_enabled(uint gpio, bool enabled);

/*! \brief Enable/disable GPIO input hysteresis (Schmitt trigger)
 *  \ingroup hardware_gpio
 *
 * Enable or disable the Schmitt trigger hysteresis on a given GPIO. This is
 * enabled on all GPIOs by default. Disabling input hysteresis can lead to
 * inconsistent readings when the input signal has very long rise or fall
 * times, but slightly reduces the GPIO's input delay.
 *
 * \sa gpio_is_input_hysteresis_enabled
 * \param gpio GPIO number
 * \param enabled true to enable input hysteresis on specified GPIO
 */
void gpio_set_input_hysteresis_enabled(uint gpio, bool enabled);

/*! \brief Determine whether input hysteresis is enabled on a specified GPIO
 *  \ingroup hardware_gpio
 *
 * \sa gpio_set_input_hysteresis_enabled
 * \param gpio GPIO number
 */
bool gpio_is_input_hysteresis_enabled(uint gpio);


/*! \brief Set slew rate for a specified GPIO
 *  \ingroup hardware_gpio
 *
 * \sa gpio_get_slew_rate
 * \param gpio GPIO number
 * \param slew GPIO output slew rate
 */
void gpio_set_slew_rate(uint gpio, enum gpio_slew_rate slew);

/*! \brief Determine current slew rate for a specified GPIO
 *  \ingroup hardware_gpio
 *
 * \sa gpio_set_slew_rate
 * \param gpio GPIO number
 * \return Current slew rate of that GPIO
 */
enum gpio_slew_rate gpio_get_slew_rate(uint gpio);

/*! \brief Set drive strength for a specified GPIO
 *  \ingroup hardware_gpio
 *
 * \sa gpio_get_drive_strength
 * \param gpio GPIO number
 * \param drive GPIO output drive strength
 */
void gpio_set_drive_strength(uint gpio, enum gpio_drive_strength drive);

/*! \brief Determine current slew rate for a specified GPIO
 *  \ingroup hardware_gpio
 *
 * \sa gpio_set_drive_strength
 * \param gpio GPIO number
 * \return Current drive strength of that GPIO
 */
enum gpio_drive_strength gpio_get_drive_strength(uint gpio);

/*! \brief Enable or disable interrupts for specified GPIO
 *  \ingroup hardware_gpio
 *
 * \note The IO IRQs are independent per-processor. This configures IRQs for
 * the processor that calls the function.
 *
 * \param gpio GPIO number
 * \param events Which events will cause an interrupt
 * \param enabled Enable or disable flag
 *
 * Events is a bitmask of the following:
 *
 * bit | interrupt
 * ----|----------
 *   0 | Low level
 *   1 | High level
 *   2 | Edge low
 *   3 | Edge high
 */
void gpio_set_irq_enabled(uint gpio, uint32_t events, bool enabled);

/*! \brief Enable interrupts for specified GPIO
 *  \ingroup hardware_gpio
 *
 * \note The IO IRQs are independent per-processor. This configures IRQs for
 * the processor that calls the function.
 *
 * \param gpio GPIO number
 * \param events Which events will cause an interrupt. See \ref gpio_set_irq_enabled for details.
 * \param enabled Enable or disable flag
 * \param callback user function to call on GPIO irq. Note only one of these can be set per processor.
 *
 * \note Currently the GPIO parameter is ignored, and this callback will be called for any enabled GPIO IRQ on any pin.
 *
 */
void gpio_set_irq_enabled_with_callback(uint gpio, uint32_t events, bool enabled, gpio_irq_callback_t callback);

/*! \brief Enable dormant wake up interrupt for specified GPIO
 *  \ingroup hardware_gpio
 *
 * This configures IRQs to restart the XOSC or ROSC when they are
 * disabled in dormant mode
 *
 * \param gpio GPIO number
 * \param events Which events will cause an interrupt. See \ref gpio_set_irq_enabled for details.
 * \param enabled Enable/disable flag
 */
void gpio_set_dormant_irq_enabled(uint gpio, uint32_t events, bool enabled);

/*! \brief Acknowledge a GPIO interrupt
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param events Bitmask of events to clear. See \ref gpio_set_irq_enabled for details.
  *
 */
void gpio_acknowledge_irq(uint gpio, uint32_t events);

/*! \brief Initialise a GPIO for (enabled I/O and set func to GPIO_FUNC_SIO)
 *  \ingroup hardware_gpio
 *
 * Clear the output enable (i.e. set to input).
 * Clear any output value.
 *
 * \param gpio GPIO number
 */
void gpio_hal_init(uint gpio);

/*! \brief Initialise multiple GPIOs (enabled I/O and set func to GPIO_FUNC_SIO)
 *  \ingroup hardware_gpio
 *
 * Clear the output enable (i.e. set to input).
 * Clear any output value.
 *
 * \param gpio_mask Mask with 1 bit per GPIO number to initialize
 */
void gpio_init_mask(uint gpio_mask);
// ----------------------------------------------------------------------------
// Input
// ----------------------------------------------------------------------------

/*! \brief Get state of a single specified GPIO
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \return Current state of the GPIO. 0 for low, non-zero for high
 */
static inline bool gpio_get(uint gpio) {
    return !!((1ul << gpio) & sio_hw->gpio_in);
}

/*! \brief Get raw value of all GPIOs
 *  \ingroup hardware_gpio
 *
 * \return Bitmask of raw GPIO values, as bits 0-29
 */
static inline uint32_t gpio_get_all(void) {
    return sio_hw->gpio_in;
}

// ----------------------------------------------------------------------------
// Output
// ----------------------------------------------------------------------------

/*! \brief Drive high every GPIO appearing in mask
 *  \ingroup hardware_gpio
 *
 * \param mask Bitmask of GPIO values to set, as bits 0-29
 */
static inline void gpio_set_mask(uint32_t mask) {
    sio_hw->gpio_set = mask;
}

/*! \brief Drive low every GPIO appearing in mask
 *  \ingroup hardware_gpio
 *
 * \param mask Bitmask of GPIO values to clear, as bits 0-29
 */
static inline void gpio_clr_mask(uint32_t mask) {
    sio_hw->gpio_clr = mask;
}

/*! \brief Toggle every GPIO appearing in mask
 *  \ingroup hardware_gpio
 *
 * \param mask Bitmask of GPIO values to toggle, as bits 0-29
 */
static inline void gpio_xor_mask(uint32_t mask) {
    sio_hw->gpio_togl = mask;
}

/*! \brief Drive GPIO high/low depending on parameters
 *  \ingroup hardware_gpio
 *
 * \param mask Bitmask of GPIO values to change, as bits 0-29
 * \param value Value to set
 *
 * For each 1 bit in \p mask, drive that pin to the value given by
 * corresponding bit in \p value, leaving other pins unchanged.
 * Since this uses the TOGL alias, it is concurrency-safe with e.g. an IRQ
 * bashing different pins from the same core.
 */
static inline void gpio_put_masked(uint32_t mask, uint32_t value) {
    sio_hw->gpio_togl = (sio_hw->gpio_out ^ value) & mask;
}

/*! \brief Drive all pins simultaneously
 *  \ingroup hardware_gpio
 *
 * \param value Bitmask of GPIO values to change, as bits 0-29
 */
static inline void gpio_put_all(uint32_t value) {
    sio_hw->gpio_out = value;
}

/*! \brief Drive a single GPIO high/low
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param value If false clear the GPIO, otherwise set it.
 */
static inline void gpio_put(uint gpio, bool value) {
    uint32_t mask = 1ul << gpio;
    if (value)
        gpio_set_mask(mask);
    else
        gpio_clr_mask(mask);
}

/*! \brief Determine whether a GPIO is currently driven high or low
 *  \ingroup hardware_gpio
 *
 * This function returns the high/low output level most recently assigned to a
 * GPIO via gpio_put() or similar. This is the value that is presented outward
 * to the IO muxing, *not* the input level back from the pad (which can be
 * read using gpio_get()).
 *
 * To avoid races, this function must not be used for read-modify-write
 * sequences when driving GPIOs -- instead functions like gpio_put() should be
 * used to atomically update GPIOs. This accessor is intended for debug use
 * only.
 *
 * \param gpio GPIO number
 * \return true if the GPIO output level is high, false if low.
 */
static inline bool gpio_get_out_level(uint gpio) {
    return !!(sio_hw->gpio_out & (1u << gpio));
}

// ----------------------------------------------------------------------------
// Direction
// ----------------------------------------------------------------------------

/*! \brief Set a number of GPIOs to output
 *  \ingroup hardware_gpio
 *
 * Switch all GPIOs in "mask" to output
 *
 * \param mask Bitmask of GPIO to set to output, as bits 0-29
 */
static inline void gpio_set_dir_out_masked(uint32_t mask) {
    sio_hw->gpio_oe_set = mask;
}

/*! \brief Set a number of GPIOs to input
 *  \ingroup hardware_gpio
 *
 * \param mask Bitmask of GPIO to set to input, as bits 0-29
 */
static inline void gpio_set_dir_in_masked(uint32_t mask) {
    sio_hw->gpio_oe_clr = mask;
}

/*! \brief Set multiple GPIO directions
 *  \ingroup hardware_gpio
 *
 * \param mask Bitmask of GPIO to set to input, as bits 0-29
 * \param value Values to set
 *
 * For each 1 bit in "mask", switch that pin to the direction given by
 * corresponding bit in "value", leaving other pins unchanged.
 * E.g. gpio_set_dir_masked(0x3, 0x2); -> set pin 0 to input, pin 1 to output,
 * simultaneously.
 */
static inline void gpio_set_dir_masked(uint32_t mask, uint32_t value) {
    sio_hw->gpio_oe_togl = (sio_hw->gpio_oe ^ value) & mask;
}

/*! \brief Set direction of all pins simultaneously.
 *  \ingroup hardware_gpio
 *
 * \param values individual settings for each gpio; for GPIO N, bit N is 1 for out, 0 for in
 */
static inline void gpio_set_dir_all_bits(uint32_t values) {
    sio_hw->gpio_oe = values;
}

/*! \brief Set a single GPIO direction
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param out true for out, false for in
 */
static inline void gpio_set_dir(uint gpio, bool out) {
    uint32_t mask = 1ul << gpio;
    if (out)
        gpio_set_dir_out_masked(mask);
    else
        gpio_set_dir_in_masked(mask);
}

/*! \brief Check if a specific GPIO direction is OUT
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \return true if the direction for the pin is OUT
 */
static inline bool gpio_is_dir_out(uint gpio) {
    return !!(sio_hw->gpio_oe & (1u << (gpio)));
}

/*! \brief Get a specific GPIO direction
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \return 1 for out, 0 for in
 */
static inline uint gpio_get_dir(uint gpio) {
    return gpio_is_dir_out(gpio); // note GPIO_OUT is 1/true and GPIO_IN is 0/false anyway
}

#endif