/*
   Copyright 2018 Sekigon

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   */

#include "matrix.h"

#include "wait.h"
#include "app_ble_func.h"
#include "bootloader.h"

#include "nrf_power.h"
#include "nrf.h"

#include "nrf_gpio.h"
#include "nrf_delay.h"


void unselect_rows(void);
void select_row(uint8_t row);
matrix_row_t read_cols(void);

static bool bootloader_flag = false;

int pins[]={ \
    PIN2, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, \
    PIN11, PIN12, PIN13, PIN14, PIN15, \
    PIN16, PIN17, PIN18, PIN19, PIN20, \
    TOP_PIN_1, TOP_PIN_5, TOP_PIN_6, \
    SIDE_PIN_4, SIDE_PIN_5, SIDE_PIN_6, SIDE_PIN_7
};

void set_pin(int pin, int set) {
    if (set) nrf_gpio_pin_set(pin);
    else nrf_gpio_pin_clear(pin);
}

void nrfmicro_power_enable(bool enable) { return; }


void matrix_init_user() {

    // blink on power on
    nrf_gpio_cfg_output(LED_PIN);

    // nrfmicro_charger_enable(true);
    // nrfmicro_power_enable(true);
    nrf_gpio_cfg_output(POWER_PIN);
    nrf_gpio_pin_write(POWER_PIN, 0);

    nrf_gpio_cfg_output(PROG_PIN);
    nrf_gpio_pin_clear(PROG_PIN);

    int pin_count = sizeof(pins)/sizeof(pins[0]);

    for (int j=0; j<pin_count; j++) nrf_gpio_cfg_output(pins[j]);

    nrf_gpio_cfg_output(PIN1);
    nrf_gpio_pin_set(PIN1);

    for (int i = 0; i < 50; i++) {
        nrf_gpio_pin_set(LED_PIN);
        nrf_delay_ms(100);

        nrf_gpio_pin_clear(LED_PIN);
        nrf_delay_ms(100);
    }

    for (int i = 0; i < 1000; i++) {
        for (int j=0; j<pin_count; j++) set_pin(pins[j], 1);
        nrf_delay_ms(100);

        for (int j=0; j<pin_count; j++) set_pin(pins[j], 0);
        nrf_delay_ms(100);
    }

    nrf_gpio_pin_set(LED_PIN);

    sleep_mode_enter();

    select_row(3);
    wait_us(50);
    matrix_row_t row = read_cols();
    unselect_rows();
    if (row == 0b111000) {
        delete_bonds();
    } else if (row == 0b10) {
        bootloader_flag = true;
    }

}

void matrix_scan_user() {
    static int cnt;
    if (bootloader_flag && cnt++==500) {
        bootloader_jump();
    }
}

