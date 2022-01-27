/**
 * @file    gpio.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
{

}
 you may
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

#include "DAP_config.h"
#include "gpio.h"

void gpio_init(void)
{
    gpio_hal_init(PIN_HID_LED);
    gpio_hal_init(PIN_CDC_LED);
    gpio_hal_init(PIN_MSC_LED);
    gpio_hal_init(nRESET_OUT_PIN);

    gpio_set_dir(PIN_HID_LED, 1);
    gpio_set_dir(PIN_CDC_LED, 1);
    gpio_set_dir(PIN_MSC_LED, 1);

    gpio_set_dir(nRESET_IN_PIN, 0);
    gpio_pull_up(nRESET_IN_PIN);
}

void gpio_set_board_power(bool powerEnabled)
{

}

void gpio_set_hid_led(gpio_led_state_t state)
{
    gpio_put(PIN_HID_LED, state);
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    gpio_put(PIN_CDC_LED, state);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    gpio_put(PIN_MSC_LED, state);
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return gpio_get(nRESET_IN_PIN);
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return 0;
}
