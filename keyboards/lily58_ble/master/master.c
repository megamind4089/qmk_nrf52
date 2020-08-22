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
#undef PACKED
#include "nrf.h"
#include "app_ble_func.h"
#include "wait.h"

#include "nrf_gpio.h"
#include "nrf_delay.h"

void unselect_rows(void);
void select_row(uint8_t row);
matrix_row_t read_cols(void);

static bool bootloader_flag = false;

extern void nrfmicro_init(void);
extern void nrfmicro_update(void);

void matrix_init_user() {
    nrfmicro_init();

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

void matrix_scan_user(void) {
    nrfmicro_update();
}
