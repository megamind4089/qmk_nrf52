/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert
Copyright 2020 Sekigon

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

#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0xD061
#define DEVICE_VER      0x0001
#define MANUFACTURER    Sekigon-Gonnoc
#define PRODUCT         BLE-USB HID Bridge
#define DESCRIPTION     Convert BLE HIDs to USB HID

#define MATRIX_SCAN_MS 3

#define MATRIX_ROWS 2
#define MATRIX_COLS 1
#define THIS_DEVICE_ROWS 1
#define THIS_DEVICE_COLS 1
#define IS_LEFT_HAND true

#define MATRIX_ROW_PINS { 0xFF }
#define MATRIX_COL_PINS { 15 }
#define LED_PIN  45

#define DIODE_DIRECTION COL2ROW


/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    1

#define TAPPING_TERM 100

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

