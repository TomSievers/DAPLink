/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_UART_H
#define _HARDWARE_UART_H

#include "regs/uart.h"
#include "bit_ops.h"
#include "core/stdint.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_uart
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/uart.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(UART_UARTDR_OFFSET) // UART_UARTDR
    // Data Register, UARTDR
    // 0x00000800 [11]    : OE (0): Overrun error
    // 0x00000400 [10]    : BE (0): Break error
    // 0x00000200 [9]     : PE (0): Parity error
    // 0x00000100 [8]     : FE (0): Framing error
    // 0x000000ff [7:0]   : DATA (0): Receive (read) data character
    io_rw_32 dr;

    _REG_(UART_UARTRSR_OFFSET) // UART_UARTRSR
    // Receive Status Register/Error Clear Register, UARTRSR/UARTECR
    // 0x00000008 [3]     : OE (0): Overrun error
    // 0x00000004 [2]     : BE (0): Break error
    // 0x00000002 [1]     : PE (0): Parity error
    // 0x00000001 [0]     : FE (0): Framing error
    io_rw_32 rsr;

    uint32_t _pad0[4];

    _REG_(UART_UARTFR_OFFSET) // UART_UARTFR
    // Flag Register, UARTFR
    // 0x00000100 [8]     : RI (0): Ring indicator
    // 0x00000080 [7]     : TXFE (1): Transmit FIFO empty
    // 0x00000040 [6]     : RXFF (0): Receive FIFO full
    // 0x00000020 [5]     : TXFF (0): Transmit FIFO full
    // 0x00000010 [4]     : RXFE (1): Receive FIFO empty
    // 0x00000008 [3]     : BUSY (0): UART busy
    // 0x00000004 [2]     : DCD (0): Data carrier detect
    // 0x00000002 [1]     : DSR (0): Data set ready
    // 0x00000001 [0]     : CTS (0): Clear to send
    io_ro_32 fr;

    uint32_t _pad1;

    _REG_(UART_UARTILPR_OFFSET) // UART_UARTILPR
    // IrDA Low-Power Counter Register, UARTILPR
    // 0x000000ff [7:0]   : ILPDVSR (0): 8-bit low-power divisor value
    io_rw_32 ilpr;

    _REG_(UART_UARTIBRD_OFFSET) // UART_UARTIBRD
    // Integer Baud Rate Register, UARTIBRD
    // 0x0000ffff [15:0]  : BAUD_DIVINT (0): The integer baud rate divisor
    io_rw_32 ibrd;

    _REG_(UART_UARTFBRD_OFFSET) // UART_UARTFBRD
    // Fractional Baud Rate Register, UARTFBRD
    // 0x0000003f [5:0]   : BAUD_DIVFRAC (0): The fractional baud rate divisor
    io_rw_32 fbrd;

    _REG_(UART_UARTLCR_H_OFFSET) // UART_UARTLCR_H
    // Line Control Register, UARTLCR_H
    // 0x00000080 [7]     : SPS (0): Stick parity select
    // 0x00000060 [6:5]   : WLEN (0): Word length
    // 0x00000010 [4]     : FEN (0): Enable FIFOs: 0 = FIFOs are disabled (character mode) that is, the FIFOs become...
    // 0x00000008 [3]     : STP2 (0): Two stop bits select
    // 0x00000004 [2]     : EPS (0): Even parity select
    // 0x00000002 [1]     : PEN (0): Parity enable: 0 = parity is disabled and no parity bit added to the data frame 1 =...
    // 0x00000001 [0]     : BRK (0): Send break
    io_rw_32 lcr_h;

    _REG_(UART_UARTCR_OFFSET) // UART_UARTCR
    // Control Register, UARTCR
    // 0x00008000 [15]    : CTSEN (0): CTS hardware flow control enable
    // 0x00004000 [14]    : RTSEN (0): RTS hardware flow control enable
    // 0x00002000 [13]    : OUT2 (0): This bit is the complement of the UART Out2 (nUARTOut2) modem status output
    // 0x00001000 [12]    : OUT1 (0): This bit is the complement of the UART Out1 (nUARTOut1) modem status output
    // 0x00000800 [11]    : RTS (0): Request to send
    // 0x00000400 [10]    : DTR (0): Data transmit ready
    // 0x00000200 [9]     : RXE (1): Receive enable
    // 0x00000100 [8]     : TXE (1): Transmit enable
    // 0x00000080 [7]     : LBE (0): Loopback enable
    // 0x00000004 [2]     : SIRLP (0): SIR low-power IrDA mode
    // 0x00000002 [1]     : SIREN (0): SIR enable: 0 = IrDA SIR ENDEC is disabled
    // 0x00000001 [0]     : UARTEN (0): UART enable: 0 = UART is disabled
    io_rw_32 cr;

    _REG_(UART_UARTIFLS_OFFSET) // UART_UARTIFLS
    // Interrupt FIFO Level Select Register, UARTIFLS
    // 0x00000038 [5:3]   : RXIFLSEL (0x2): Receive interrupt FIFO level select
    // 0x00000007 [2:0]   : TXIFLSEL (0x2): Transmit interrupt FIFO level select
    io_rw_32 ifls;

    _REG_(UART_UARTIMSC_OFFSET) // UART_UARTIMSC
    // Interrupt Mask Set/Clear Register, UARTIMSC
    // 0x00000400 [10]    : OEIM (0): Overrun error interrupt mask
    // 0x00000200 [9]     : BEIM (0): Break error interrupt mask
    // 0x00000100 [8]     : PEIM (0): Parity error interrupt mask
    // 0x00000080 [7]     : FEIM (0): Framing error interrupt mask
    // 0x00000040 [6]     : RTIM (0): Receive timeout interrupt mask
    // 0x00000020 [5]     : TXIM (0): Transmit interrupt mask
    // 0x00000010 [4]     : RXIM (0): Receive interrupt mask
    // 0x00000008 [3]     : DSRMIM (0): nUARTDSR modem interrupt mask
    // 0x00000004 [2]     : DCDMIM (0): nUARTDCD modem interrupt mask
    // 0x00000002 [1]     : CTSMIM (0): nUARTCTS modem interrupt mask
    // 0x00000001 [0]     : RIMIM (0): nUARTRI modem interrupt mask
    io_rw_32 imsc;

    _REG_(UART_UARTRIS_OFFSET) // UART_UARTRIS
    // Raw Interrupt Status Register, UARTRIS
    // 0x00000400 [10]    : OERIS (0): Overrun error interrupt status
    // 0x00000200 [9]     : BERIS (0): Break error interrupt status
    // 0x00000100 [8]     : PERIS (0): Parity error interrupt status
    // 0x00000080 [7]     : FERIS (0): Framing error interrupt status
    // 0x00000040 [6]     : RTRIS (0): Receive timeout interrupt status
    // 0x00000020 [5]     : TXRIS (0): Transmit interrupt status
    // 0x00000010 [4]     : RXRIS (0): Receive interrupt status
    // 0x00000008 [3]     : DSRRMIS (0): nUARTDSR modem interrupt status
    // 0x00000004 [2]     : DCDRMIS (0): nUARTDCD modem interrupt status
    // 0x00000002 [1]     : CTSRMIS (0): nUARTCTS modem interrupt status
    // 0x00000001 [0]     : RIRMIS (0): nUARTRI modem interrupt status
    io_ro_32 ris;

    _REG_(UART_UARTMIS_OFFSET) // UART_UARTMIS
    // Masked Interrupt Status Register, UARTMIS
    // 0x00000400 [10]    : OEMIS (0): Overrun error masked interrupt status
    // 0x00000200 [9]     : BEMIS (0): Break error masked interrupt status
    // 0x00000100 [8]     : PEMIS (0): Parity error masked interrupt status
    // 0x00000080 [7]     : FEMIS (0): Framing error masked interrupt status
    // 0x00000040 [6]     : RTMIS (0): Receive timeout masked interrupt status
    // 0x00000020 [5]     : TXMIS (0): Transmit masked interrupt status
    // 0x00000010 [4]     : RXMIS (0): Receive masked interrupt status
    // 0x00000008 [3]     : DSRMMIS (0): nUARTDSR modem masked interrupt status
    // 0x00000004 [2]     : DCDMMIS (0): nUARTDCD modem masked interrupt status
    // 0x00000002 [1]     : CTSMMIS (0): nUARTCTS modem masked interrupt status
    // 0x00000001 [0]     : RIMMIS (0): nUARTRI modem masked interrupt status
    io_ro_32 mis;

    _REG_(UART_UARTICR_OFFSET) // UART_UARTICR
    // Interrupt Clear Register, UARTICR
    // 0x00000400 [10]    : OEIC (0): Overrun error interrupt clear
    // 0x00000200 [9]     : BEIC (0): Break error interrupt clear
    // 0x00000100 [8]     : PEIC (0): Parity error interrupt clear
    // 0x00000080 [7]     : FEIC (0): Framing error interrupt clear
    // 0x00000040 [6]     : RTIC (0): Receive timeout interrupt clear
    // 0x00000020 [5]     : TXIC (0): Transmit interrupt clear
    // 0x00000010 [4]     : RXIC (0): Receive interrupt clear
    // 0x00000008 [3]     : DSRMIC (0): nUARTDSR modem interrupt clear
    // 0x00000004 [2]     : DCDMIC (0): nUARTDCD modem interrupt clear
    // 0x00000002 [1]     : CTSMIC (0): nUARTCTS modem interrupt clear
    // 0x00000001 [0]     : RIMIC (0): nUARTRI modem interrupt clear
    io_rw_32 icr;

    _REG_(UART_UARTDMACR_OFFSET) // UART_UARTDMACR
    // DMA Control Register, UARTDMACR
    // 0x00000004 [2]     : DMAONERR (0): DMA on error
    // 0x00000002 [1]     : TXDMAE (0): Transmit DMA enable
    // 0x00000001 [0]     : RXDMAE (0): Receive DMA enable
    io_rw_32 dmacr;
} uart_hw_t;

#define uart0_hw ((uart_hw_t *const)UART0_BASE)
#define uart1_hw ((uart_hw_t *const)UART1_BASE)

// PICO_CONFIG: PICO_UART_ENABLE_CRLF_SUPPORT, Enable/disable CR/LF translation support, type=bool, default=1, group=hardware_uart
#ifndef PICO_UART_ENABLE_CRLF_SUPPORT
#define PICO_UART_ENABLE_CRLF_SUPPORT 1
#endif

// PICO_CONFIG: PICO_UART_DEFAULT_CRLF, Enable/disable CR/LF translation on UART, type=bool, default=0, depends=PICO_UART_ENABLE_CRLF_SUPPORT, group=hardware_uart
#ifndef PICO_UART_DEFAULT_CRLF
#define PICO_UART_DEFAULT_CRLF 0
#endif

// PICO_CONFIG: PICO_DEFAULT_UART, Define the default UART used for printf etc, min=0, max=1, group=hardware_uart
// PICO_CONFIG: PICO_DEFAULT_UART_TX_PIN, Define the default UART TX pin, min=0, max=29, group=hardware_uart
// PICO_CONFIG: PICO_DEFAULT_UART_RX_PIN, Define the default UART RX pin, min=0, max=29, group=hardware_uart

// PICO_CONFIG: PICO_DEFAULT_UART_BAUD_RATE, Define the default UART baudrate, max=921600, default=115200, group=hardware_uart
#ifndef PICO_DEFAULT_UART_BAUD_RATE
#define PICO_DEFAULT_UART_BAUD_RATE 115200   ///< Default baud rate
#endif

/** \file hardware/uart.h
 *  \defgroup hardware_uart hardware_uart
 *
 * Hardware UART API
 *
 * RP2040 has 2 identical instances of a UART peripheral, based on the ARM PL011. Each UART can be connected to a number
 * of GPIO pins as defined in the GPIO muxing.
 *
 * Only the TX, RX, RTS, and CTS signals are
 * connected, meaning that the modem mode and IrDA mode of the PL011 are not supported.
 *
 * \subsection uart_example Example
 * \addtogroup hardware_uart
 *
 *  \code
 *  int main() {
 *
 *     // Initialise UART 0
 *     uart_init(uart0, 115200);
 *
 *     // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
 *     gpio_set_function(0, GPIO_FUNC_UART);
 *     gpio_set_function(1, GPIO_FUNC_UART);
 *
 *     uart_puts(uart0, "Hello world!");
 * }
 * \endcode
 */

// Currently always a pointer to hw but it might not be in the future
typedef struct uart_inst uart_inst_t;

/** The UART identifiers for use in UART functions.
 *
 * e.g. uart_init(uart1, 48000)
 *
 *  \ingroup hardware_uart
 * @{
 */
#define uart0 ((uart_inst_t * const)uart0_hw) ///< Identifier for UART instance 0
#define uart1 ((uart_inst_t * const)uart1_hw) ///< Identifier for UART instance 1

/** @} */

#if !defined(PICO_DEFAULT_UART_INSTANCE) && defined(PICO_DEFAULT_UART)
#define PICO_DEFAULT_UART_INSTANCE (__CONCAT(uart,PICO_DEFAULT_UART))
#endif

#ifdef PICO_DEFAULT_UART_INSTANCE
#define uart_default PICO_DEFAULT_UART_INSTANCE
#endif

/*! \brief Convert UART instance to hardware instance number
 *  \ingroup hardware_uart
 *
 * \param uart UART instance
 * \return Number of UART, 0 or 1.
 */
static inline uint uart_get_index(uart_inst_t *uart) {
    return uart == uart1 ? 1 : 0;
}

static inline uart_inst_t *uart_get_instance(uint instance) {
    return instance ? uart1 : uart0;
}

static inline uart_hw_t *uart_get_hw(uart_inst_t *uart) {
    uart_get_index(uart); // check it is a hw uart
    return (uart_hw_t *)uart;
}

/** \brief UART Parity enumeration
 *  \ingroup hardware_uart
 */
typedef enum {
    UART_PARITY_NONE_I = 0,
    UART_PARITY_ODD_I = 1,
    UART_PARITY_EVEN_I = 2
    
} uart_parity_t;

// ----------------------------------------------------------------------------
// Setup

/*! \brief Initialise a UART
 *  \ingroup hardware_uart
 *
 * Put the UART into a known state, and enable it. Must be called before other
 * functions.
 *
 * \note There is no guarantee that the baudrate requested will be possible, the nearest will be chosen,
 * and this function will return the configured baud rate.
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param baudrate Baudrate of UART in Hz
 * \return Actual set baudrate
 */
uint uart_init(uart_inst_t *uart, uint baudrate);

/*! \brief DeInitialise a UART
 *  \ingroup hardware_uart
 *
 * Disable the UART if it is no longer used. Must be reinitialised before
 * being used again.
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 */
void uart_deinit(uart_inst_t *uart);

/*! \brief Set UART baud rate
 *  \ingroup hardware_uart
 *
 * Set baud rate as close as possible to requested, and return actual rate selected.
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param baudrate Baudrate in Hz
 * \return Actual set baudrate
 */
uint uart_set_baudrate(uart_inst_t *uart, uint baudrate);

/*! \brief Set UART flow control CTS/RTS
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param cts If true enable flow control of TX  by clear-to-send input
 * \param rts If true enable assertion of request-to-send output by RX flow control
 */
static inline void uart_set_hw_flow(uart_inst_t *uart, bool cts, bool rts) {
    hw_write_masked(&uart_get_hw(uart)->cr,
                   (bool_to_bit(cts) << UART_UARTCR_CTSEN_LSB) | (bool_to_bit(rts) << UART_UARTCR_RTSEN_LSB),
                   UART_UARTCR_RTSEN_BITS | UART_UARTCR_CTSEN_BITS);
}

/*! \brief Set UART data format
 *  \ingroup hardware_uart
 *
 * Configure the data format (bits etc() for the UART
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param data_bits Number of bits of data. 5..8
 * \param stop_bits Number of stop bits 1..2
 * \param parity Parity option.
 */
static inline void uart_set_format(uart_inst_t *uart, uint data_bits, uint stop_bits, uart_parity_t parity) {
    invalid_params_if(UART, data_bits < 5 || data_bits > 8);
    invalid_params_if(UART, stop_bits != 1 && stop_bits != 2);
    invalid_params_if(UART, parity != UART_PARITY_NONE_I && parity != UART_PARITY_EVEN_I && parity != UART_PARITY_ODD_I);
    hw_write_masked(&uart_get_hw(uart)->lcr_h,
                   ((data_bits - 5u) << UART_UARTLCR_H_WLEN_LSB) |
                   ((stop_bits - 1u) << UART_UARTLCR_H_STP2_LSB) |
                   (bool_to_bit(parity != UART_PARITY_NONE_I) << UART_UARTLCR_H_PEN_LSB) |
                   (bool_to_bit(parity == UART_PARITY_EVEN_I) << UART_UARTLCR_H_EPS_LSB),
                   UART_UARTLCR_H_WLEN_BITS |
                   UART_UARTLCR_H_STP2_BITS |
                   UART_UARTLCR_H_PEN_BITS |
                   UART_UARTLCR_H_EPS_BITS);
}

/*! \brief Setup UART interrupts
 *  \ingroup hardware_uart
 *
 * Enable the UART's interrupt output. An interrupt handler will need to be installed prior to calling
 * this function.
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param rx_has_data If true an interrupt will be fired when the RX FIFO contains data.
 * \param tx_needs_data If true an interrupt will be fired when the TX FIFO needs data.
 */
static inline void uart_set_irq_enables(uart_inst_t *uart, bool rx_has_data, bool tx_needs_data) {
    // Both UARTRXINTR (RX) and UARTRTINTR (RX timeout) interrupts are
    // required for rx_has_data. RX asserts when >=4 characters are in the RX
    // FIFO (for RXIFLSEL=0). RT asserts when there are >=1 characters and no
    // more have been received for 32 bit periods.
    uart_get_hw(uart)->imsc = (bool_to_bit(tx_needs_data) << UART_UARTIMSC_TXIM_LSB) |
                              (bool_to_bit(rx_has_data) << UART_UARTIMSC_RXIM_LSB) |
                              (bool_to_bit(rx_has_data) << UART_UARTIMSC_RTIM_LSB);
    if (rx_has_data) {
        // Set minimum threshold
        hw_write_masked(&uart_get_hw(uart)->ifls, 0 << UART_UARTIFLS_RXIFLSEL_LSB,
                        UART_UARTIFLS_RXIFLSEL_BITS);
    }
    if (tx_needs_data) {
        // Set maximum threshold
        hw_write_masked(&uart_get_hw(uart)->ifls, 0 << UART_UARTIFLS_TXIFLSEL_LSB,
                        UART_UARTIFLS_TXIFLSEL_BITS);
    }
}

/*! \brief Test if specific UART is enabled
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \return true if the UART is enabled
 */
static inline bool uart_is_enabled(uart_inst_t *uart) {
    return !!(uart_get_hw(uart)->cr & UART_UARTCR_UARTEN_BITS);
}

/*! \brief Enable/Disable the FIFOs on specified UART
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param enabled true to enable FIFO (default), false to disable
 */
static inline void uart_set_fifo_enabled(uart_inst_t *uart, bool enabled) {
    hw_write_masked(&uart_get_hw(uart)->lcr_h,
                   (bool_to_bit(enabled) << UART_UARTLCR_H_FEN_LSB),
                   UART_UARTLCR_H_FEN_BITS);
}


// ----------------------------------------------------------------------------
// Generic input/output

/*! \brief Determine if space is available in the TX FIFO
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \return false if no space available, true otherwise
 */
static inline bool uart_is_writable(uart_inst_t *uart) {
    return !(uart_get_hw(uart)->fr & UART_UARTFR_TXFF_BITS);
}

/*! \brief Wait for the UART TX fifo to be drained
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 */
static inline void uart_tx_wait_blocking(uart_inst_t *uart) {
    while (uart_get_hw(uart)->fr & UART_UARTFR_BUSY_BITS) tight_loop_contents();
}

/*! \brief Determine whether data is waiting in the RX FIFO
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \return 0 if no data available, otherwise the number of bytes, at least, that can be read
 *
 * \note HW limitations mean this function will return either 0 or 1.
 */
static inline bool uart_is_readable(uart_inst_t *uart) {
    // PL011 doesn't expose levels directly, so return values are only 0 or 1
    return !(uart_get_hw(uart)->fr & UART_UARTFR_RXFE_BITS);
}

/*! \brief  Write to the UART for transmission.
 *  \ingroup hardware_uart
 *
 * This function will block until all the data has been sent to the UART
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param src The bytes to send
 * \param len The number of bytes to send
 */
static inline void uart_write_blocking(uart_inst_t *uart, const uint8_t *src, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        while (!uart_is_writable(uart))
            tight_loop_contents();
        uart_get_hw(uart)->dr = *src++;
    }
}

/*! \brief  Read from the UART
 *  \ingroup hardware_uart
 *
 * This function will block until all the data has been received from the UART
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param dst Buffer to accept received bytes
 * \param len The number of bytes to receive.
 */
static inline void uart_read_blocking(uart_inst_t *uart, uint8_t *dst, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        while (!uart_is_readable(uart))
            tight_loop_contents();
        *dst++ = (uint8_t) uart_get_hw(uart)->dr;
    }
}

// ----------------------------------------------------------------------------
// UART-specific operations and aliases

/*! \brief  Write single character to UART for transmission.
 *  \ingroup hardware_uart
 *
 * This function will block until the entire character has been sent
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param c The character  to send
 */
static inline void uart_putc_raw(uart_inst_t *uart, char c) {
    uart_write_blocking(uart, (const uint8_t *) &c, 1);
}

/*! \brief  Write single character to UART for transmission, with optional CR/LF conversions
 *  \ingroup hardware_uart
 *
 * This function will block until the character has been sent
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param c The character  to send
 */
static inline void uart_putc(uart_inst_t *uart, char c) {
#if PICO_UART_ENABLE_CRLF_SUPPORT
    extern short uart_char_to_line_feed[NUM_UARTS];
    if (uart_char_to_line_feed[uart_get_index(uart)] == c)
        uart_putc_raw(uart, '\r');
#endif
    uart_putc_raw(uart, c);
}

/*! \brief  Write string to UART for transmission, doing any CR/LF conversions
 *  \ingroup hardware_uart
 *
 * This function will block until the entire string has been sent
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param s The null terminated string to send
 */
static inline void uart_puts(uart_inst_t *uart, const char *s) {
#if PICO_UART_ENABLE_CRLF_SUPPORT
    bool last_was_cr = false;
    while (*s) {
        // Don't add extra carriage returns if one is present
        if (last_was_cr)
            uart_putc_raw(uart, *s);
        else
            uart_putc(uart, *s);
        last_was_cr = *s++ == '\r';
    }
#else
    while (*s)
        uart_putc(uart, *s++);
#endif
}

/*! \brief  Read a single character to UART
 *  \ingroup hardware_uart
 *
 * This function will block until the character has been read
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \return The character read.
 */
static inline char uart_getc(uart_inst_t *uart) {
    char c;
    uart_read_blocking(uart, (uint8_t *) &c, 1);
    return c;
}

/*! \brief Assert a break condition on the UART transmission.
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param en Assert break condition (TX held low) if true. Clear break condition if false.
 */
static inline void uart_set_break(uart_inst_t *uart, bool en) {
    if (en)
        hw_set_bits(&uart_get_hw(uart)->lcr_h, UART_UARTLCR_H_BRK_BITS);
    else
        hw_clear_bits(&uart_get_hw(uart)->lcr_h, UART_UARTLCR_H_BRK_BITS);
}

/*! \brief Set CR/LF conversion on UART
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param translate If true, convert line feeds to carriage return on transmissions
 */
void uart_set_translate_crlf(uart_inst_t *uart, bool translate);

/*! \brief Wait for the default UART's TX FIFO to be drained
 *  \ingroup hardware_uart
 */
static inline void uart_default_tx_wait_blocking(void) {
#ifdef uart_default
    uart_tx_wait_blocking(uart_default);
#else
    assert(false);
#endif
}

/*! \brief Wait for up to a certain number of microseconds for the RX FIFO to be non empty
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param us the number of microseconds to wait at most (may be 0 for an instantaneous check)
 * \return true if the RX FIFO became non empty before the timeout, false otherwise
 */
bool uart_is_readable_within_us(uart_inst_t *uart, uint32_t us);

/*! \brief Return the DREQ to use for pacing transfers to/from a particular UART instance
 *  \ingroup hardware_uart
 *
 * \param uart UART instance. \ref uart0 or \ref uart1
 * \param is_tx true for sending data to the UART instance, false for receiving data from the UART instance
 */
static inline uint uart_get_dreq(uart_inst_t *uart, bool is_tx) {
    /*static_assert(DREQ_UART0_RX == DREQ_UART0_TX + 1, "");
    static_assert(DREQ_UART1_RX == DREQ_UART1_TX + 1, "");
    static_assert(DREQ_UART1_TX == DREQ_UART0_TX + 2, "");
    return DREQ_UART0_TX + uart_get_index(uart) * 2 + !is_tx;*/

    #pragma message "STUB FUNCTION"

    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
