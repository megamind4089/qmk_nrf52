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

#include "io_expander.h"

#define IOEXPANDER_TYPES  {TCA9555, TCA9555}
#define IOEXPANDER_ADDRS  {0x26, 0x27}

const IOEXPANDER_TYPEDEF ioexpander_types[] = IOEXPANDER_TYPES;
const uint8_t ioexpander_addrs[] = IOEXPANDER_ADDRS;

void matrix_init_user() {
}

void matrix_scan_user() {
}
