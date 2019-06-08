/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert
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

#ifndef CONFIG_H
#define CONFIG_H

#define BLE_NUS_MIN_INTERVAL 30
#define BLE_NUS_MAX_INTERVAL 70

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x3060
#define DEVICE_VER      0x0001
#define MANUFACTURER    Yushakobo
#define PRODUCT         Helix Beta
#define DESCRIPTION     A split keyboard for the cheap makers

/* key matrix size */
#if  HELIX_ROWS == 4
  #define MATRIX_ROWS 8
  #define LAYOUT LAYOUT_HELIX_4ROW
#elif HELIX_ROWS == 5
  #define MATRIX_ROWS 10
  #define LAYOUT LAYOUT_HELIX_5ROW
#endif
#define MATRIX_COLS 7

#define DIODE_DIRECTION COL2ROW

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

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

#define PROGMEM // arm-gcc does not interpret PROGMEM
#define RGB_DI_PIN 8     // The pin the LED strip is connected to
#ifdef RGBLED_BACK
  #if HELIX_ROWS == 4
    #define RGBLED_NUM 25
  #else
    #define RGBLED_NUM 32
  #endif
#else
  #define RGBLED_NUM 6
#endif

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

#endif
