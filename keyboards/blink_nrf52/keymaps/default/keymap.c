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

#include QMK_KEYBOARD_H

#include "app_ble_func.h"

enum layer_number {
    _QWERTY = 0,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { 0 };

#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation)
{
    return OLED_ROTATION_0;
}


void oled_task_user(void)
{
    char str[64] = {0};
    int vcc = get_vcc();

    sprintf(str, "Working");
    oled_write_ln_P(PSTR(str), false);
    oled_write_ln_P("\n\n", false);

    memset(str, 0, sizeof str);
    sprintf(vc, "%4dmV", vcc);
    sprintf(str, "Bat: %s ", vcc==0 ? "off   " : vc);
    oled_write_ln_P(PSTR(str), false);
}

#endif
