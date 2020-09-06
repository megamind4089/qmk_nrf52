/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert
Copyright 2017 Biacco42
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

#pragma once

#include "custom_board.h"

#define ENABLE_STARTUP_ADV_NOLIST

#define BLE_NUS_MIN_INTERVAL 30
#define BLE_NUS_MAX_INTERVAL 30
#define BLE_HID_MAX_INTERVAL 50
#define BLE_HID_SLAVE_LATENCY 3

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6442
#define DEVICE_VER      0x0100
#define MANUFACTURER    Megamind
#define PRODUCT         Smatreus Keyboard
#define DESCRIPTION     Smatreus wireless keyboard

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 13

#define DIODE_DIRECTION ROW2COL

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    1

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

//#define RGBLIGHT_ANIMATIONS
//#define RGB_DI_PIN 8     // The pin the LED strip is connected to
//#define RGBLED_NUM 5     // Number of LEDs in your strip

#define PROGMEM
/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

#define PREVENT_STUCK_MODIFIERS

#define IS_LEFT_HAND  true

#define OLED_DISPLAY_64X128
#define OLED_IC             OLED_IC_SH1107
