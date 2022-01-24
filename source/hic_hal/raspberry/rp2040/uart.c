/**
 * @file    uart.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cortex_m.h"
#include "uart_hal.h"
#include "gpio_hal.h"
#include "reset.h"
#include "rp2040.h"
#include "uart.h"
#include "gpio.h"
#include "util.h"
#include "circ_buf.h"
#include "IO_Config.h"
#include "irq.h"

#define UART_INSTANCE uart0

#define UART_TX 0
#define UART_RX 1

#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE         (512)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

static UART_Configuration configuration = {
    .Baudrate = 115200,
    .DataBits = UART_DATA_BITS_8,
    .Parity = UART_PARITY_NONE,
    .StopBits = UART_STOP_BITS_1,
    .FlowControl = UART_FLOW_CONTROL_NONE,
};

void UART_IRQ();

static void clear_buffers(void)
{
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}

int32_t uart_initialize(void)
{
    reset_block(kRESET_UART0);

    clear_buffers();

    uart_init(UART_INSTANCE, 115200);

    uart_set_format(UART_INSTANCE,
        (uint8_t)configuration.DataBits, 
        (configuration.StopBits == UART_STOP_BITS_1) ? 1 : 2, 
        (uart_parity_t)configuration.Parity);

    uart_set_baudrate(UART_INSTANCE, configuration.Baudrate);

    uart_set_hw_flow(UART_INSTANCE, false, false);

    uart_set_fifo_enabled(UART_INSTANCE, false);

    irq_set_exclusive_handler(UART0_IRQ, UART_IRQ);
    irq_set_enabled(UART0_IRQ, true);

    uart_set_irq_enables(UART_INSTANCE, true, true);

    gpio_set_function(UART_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_RX, GPIO_FUNC_UART); 
}

int32_t uart_uninitialize(void)
{
    uart_deinit(UART_INSTANCE);

    gpio_set_function(UART_RX, GPIO_FUNC_SIO);
    gpio_set_function(UART_TX, GPIO_FUNC_SIO);
}

int32_t uart_reset(void)
{
    uart_initialize();
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    if(config->DataBits == UART_DATA_BITS_16) {
        configuration.DataBits = UART_DATA_BITS_8;
    } else {
        configuration.DataBits = config->DataBits;
    }

    if(config->StopBits == UART_PARITY_MARK && config->StopBits == UART_PARITY_SPACE) {
        configuration.Parity = UART_PARITY_NONE;
    } else {
        configuration.Parity = config->Parity;
    }

    if(config->Parity == UART_STOP_BITS_1_5) {
        configuration.Parity = UART_STOP_BITS_2;
    } else {
        configuration.Parity = config->Parity;
    }

    
    uart_set_format(UART_INSTANCE,
        (uint8_t)configuration.DataBits, 
        (configuration.StopBits == UART_STOP_BITS_1) ? 1 : 2, 
        (uart_parity_t)configuration.Parity);

    uart_set_baudrate(UART_INSTANCE, configuration.Baudrate);

    return 1;
}

extern int32_t uart_get_configuration(UART_Configuration *config)
{
    config->Baudrate    = configuration.Baudrate;
    config->StopBits    = configuration.StopBits;
    config->Parity      = configuration.Parity;
    config->FlowControl = configuration.FlowControl;
    config->DataBits    = configuration.DataBits;

    return 1;
}

int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    cortex_int_state_t state = cortex_int_get_and_disable();

    bool busy = circ_buf_count_used(&write_buffer) > 0;
    int32_t cnt = circ_buf_write(&write_buffer, data, size);

    cortex_int_restore(state);

    if(!busy)
    {
        uart_putc(UART_INSTANCE, circ_buf_pop(&write_buffer));
    }
    
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

void uart_set_control_line_state(uint16_t ctrl_bmp)
{

}

void uart_software_flow_control(void)
{

}

void uart_enable_flow_control(bool enabled)
{

}

void UART_IRQ()
{
    if((uart_get_hw(UART_INSTANCE)->mis & UART_UARTMIS_RXMIS_BITS) > 0)
    {
        circ_buf_push(&read_buffer, uart_getc(UART_INSTANCE));
    }

    if((uart_get_hw(UART_INSTANCE)->mis & UART_UARTMIS_TXMIS_BITS) > 0)
    {
        uart_putc(UART_INSTANCE, circ_buf_pop(&write_buffer));
    }
}