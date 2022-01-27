#ifndef RESET_H_
#define RESET_H_

#include "regs/reset.h"
#include "core/stdint.h"

/// \tag::resets_hw[]
typedef struct {
    _REG_(RESETS_RESET_OFFSET) // RESETS_RESET
    // Reset control
    // 0x01000000 [24]    : usbctrl (1)
    // 0x00800000 [23]    : uart1 (1)
    // 0x00400000 [22]    : uart0 (1)
    // 0x00200000 [21]    : timer (1)
    // 0x00100000 [20]    : tbman (1)
    // 0x00080000 [19]    : sysinfo (1)
    // 0x00040000 [18]    : syscfg (1)
    // 0x00020000 [17]    : spi1 (1)
    // 0x00010000 [16]    : spi0 (1)
    // 0x00008000 [15]    : rtc (1)
    // 0x00004000 [14]    : pwm (1)
    // 0x00002000 [13]    : pll_usb (1)
    // 0x00001000 [12]    : pll_sys (1)
    // 0x00000800 [11]    : pio1 (1)
    // 0x00000400 [10]    : pio0 (1)
    // 0x00000200 [9]     : pads_qspi (1)
    // 0x00000100 [8]     : pads_bank0 (1)
    // 0x00000080 [7]     : jtag (1)
    // 0x00000040 [6]     : io_qspi (1)
    // 0x00000020 [5]     : io_bank0 (1)
    // 0x00000010 [4]     : i2c1 (1)
    // 0x00000008 [3]     : i2c0 (1)
    // 0x00000004 [2]     : dma (1)
    // 0x00000002 [1]     : busctrl (1)
    // 0x00000001 [0]     : adc (1)
    io_rw_32 reset;

    _REG_(RESETS_WDSEL_OFFSET) // RESETS_WDSEL
    // Watchdog select
    // 0x01000000 [24]    : usbctrl (0)
    // 0x00800000 [23]    : uart1 (0)
    // 0x00400000 [22]    : uart0 (0)
    // 0x00200000 [21]    : timer (0)
    // 0x00100000 [20]    : tbman (0)
    // 0x00080000 [19]    : sysinfo (0)
    // 0x00040000 [18]    : syscfg (0)
    // 0x00020000 [17]    : spi1 (0)
    // 0x00010000 [16]    : spi0 (0)
    // 0x00008000 [15]    : rtc (0)
    // 0x00004000 [14]    : pwm (0)
    // 0x00002000 [13]    : pll_usb (0)
    // 0x00001000 [12]    : pll_sys (0)
    // 0x00000800 [11]    : pio1 (0)
    // 0x00000400 [10]    : pio0 (0)
    // 0x00000200 [9]     : pads_qspi (0)
    // 0x00000100 [8]     : pads_bank0 (0)
    // 0x00000080 [7]     : jtag (0)
    // 0x00000040 [6]     : io_qspi (0)
    // 0x00000020 [5]     : io_bank0 (0)
    // 0x00000010 [4]     : i2c1 (0)
    // 0x00000008 [3]     : i2c0 (0)
    // 0x00000004 [2]     : dma (0)
    // 0x00000002 [1]     : busctrl (0)
    // 0x00000001 [0]     : adc (0)
    io_rw_32 wdsel;

    _REG_(RESETS_RESET_DONE_OFFSET) // RESETS_RESET_DONE
    // Reset done
    // 0x01000000 [24]    : usbctrl (0)
    // 0x00800000 [23]    : uart1 (0)
    // 0x00400000 [22]    : uart0 (0)
    // 0x00200000 [21]    : timer (0)
    // 0x00100000 [20]    : tbman (0)
    // 0x00080000 [19]    : sysinfo (0)
    // 0x00040000 [18]    : syscfg (0)
    // 0x00020000 [17]    : spi1 (0)
    // 0x00010000 [16]    : spi0 (0)
    // 0x00008000 [15]    : rtc (0)
    // 0x00004000 [14]    : pwm (0)
    // 0x00002000 [13]    : pll_usb (0)
    // 0x00001000 [12]    : pll_sys (0)
    // 0x00000800 [11]    : pio1 (0)
    // 0x00000400 [10]    : pio0 (0)
    // 0x00000200 [9]     : pads_qspi (0)
    // 0x00000100 [8]     : pads_bank0 (0)
    // 0x00000080 [7]     : jtag (0)
    // 0x00000040 [6]     : io_qspi (0)
    // 0x00000020 [5]     : io_bank0 (0)
    // 0x00000010 [4]     : i2c1 (0)
    // 0x00000008 [3]     : i2c0 (0)
    // 0x00000004 [2]     : dma (0)
    // 0x00000002 [1]     : busctrl (0)
    // 0x00000001 [0]     : adc (0)
    io_ro_32 reset_done;
} resets_hw_t;

#define resets_hw ((resets_hw_t *const)RESETS_BASE)

typedef enum {
    kRESET_USB = 1 << 24,
    kRESET_UART1 = 1 << 23,
    kRESET_UART0 = 1 << 22,
    kRESET_Timer = 1 << 21,
    kRESET_TBManager = 1 << 20,
    kRESET_SysInfo = 1 << 19,
    kRESET_SysConf = 1 << 18,
    kRESET_SPI1 = 1 << 17,
    kRESET_SPI0 = 1 << 16,
    kRESET_RTC = 1 << 15,
    kRESET_PWM = 1 << 14,
    kRESET_PLLUSB = 1 << 13,
    kRESET_PLLSYS = 1 << 12,
    kRESET_PIO1 = 1 << 11,
    kRESET_PIO0 = 1 << 10,
    kRESET_QSPIPads = 1 << 9,
    kRESET_Bank0Pads = 1 << 8,
    kRESET_JTAG = 1 << 7,
    kRESET_IOBank1 = 1 << 6,
    kRESET_IOBank0 = 1 << 5,
    kRESET_I2C1 = 1 << 4,
    kRESET_I2C0 = 1 << 3,
    kRESET_DMA = 1 << 2,
    kRESET_BusCtrl = 1 << 1,
    kRESET_ADC = 1 << 0
} reset_block_t;

void reset_block(reset_block_t block);

#endif //RESET_H_