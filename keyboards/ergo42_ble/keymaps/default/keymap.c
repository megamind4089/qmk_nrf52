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

enum custom_keycodes {
    AD_WO_L = SAFE_RANGE, /* Start advertising without whitelist  */
    BLE_DIS,              /* Disable BLE HID sending              */
    BLE_EN,               /* Enable BLE HID sending               */
    USB_DIS,              /* Disable USB HID sending              */
    USB_EN,               /* Enable USB HID sending               */
    DELBNDS,              /* Delete all bonding                   */
    ADV_ID0,              /* Start advertising to PeerID 0        */
    ADV_ID1,              /* Start advertising to PeerID 1        */
    ADV_ID2,              /* Start advertising to PeerID 2        */
    ADV_ID3,              /* Start advertising to PeerID 3        */
    ADV_ID4,              /* Start advertising to PeerID 4        */
    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    ENT_DFU,              /* Start bootloader                     */
    ENT_SLP,              /* Deep sleep mode                      */
};


extern keymap_config_t keymap_config;

enum {
  BASE=0,
  META,
  SYMB,
  CTRL,
  ABS_MOUSE,
  GAME
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* BASE
   * ,------------------------------------------------.   ,------------------------------------------------.
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |  [   |   |  ]   |   Y  |   U  |   I  |   O  |   P  |  @   |
   * |------+------+------+------+------+------+------|   |-------------+------+------+------+------+------|
   * | Alt  |   A  |   S  |   D  |   F  |   G  |  (   |   |  )   |   H  |   J  |   K  |   L  |   ;  |  :   |
   * |------+------+------+------+------+------+------|   |------|------+------+------+------+------+------|
   * | Sft  |   Z  |   X  |   C  |   V  |   B  |  {   |   |  }   |   N  |   M  |   ,  |   .  |   /  |\/Sft |
   * |------+------+------+------+------+------+------|   |------+------+------+------+------+------+------|
   * | Ctrl | GUI  |  App |PrtSc |ESC/  |Space/|Tab/  |   |Back  |Enter/| Del  |PrtSc |=>GAME|=>SYMB|  \   |
   * |      |      |      |      |~SYMB |~CTRL |Shift |   |Space |~META |      |      |      |      |      |
   * `------------------------------------------------'   `------------------------------------------------'
   */
  [BASE] = LAYOUT( \
    KC_ESC,   KC_Q,    KC_W,   KC_E,    KC_R,             KC_T,          KC_RBRC,       KC_BSLS,    KC_Y,             KC_U,    KC_I,     KC_O,     KC_P,     KC_LBRC, \
    KC_TAB,   KC_A,    KC_S,   KC_D,    KC_F,             KC_G,          S(KC_8),       S(KC_9),    KC_H,             KC_J,    KC_K,     KC_L,     KC_SCLN,  KC_QUOT, \
    KC_LSFT,  KC_Z,    KC_X,   KC_C,    KC_V,             KC_B,          S(KC_RBRC),    S(KC_BSLS), KC_N,             KC_M,    KC_COMM,  KC_DOT,   KC_SLSH,  SFT_T(KC_RO), \
    KC_LCTRL, KC_LGUI, KC_APP, KC_LALT, LT(CTRL, KC_SPC), SFT_T(KC_SPC), SFT_T(KC_TAB), KC_BSPC,    LT(META, KC_ENT), KC_DELT, KC_PSCR,  TG(GAME), TG(SYMB), KC_JYEN \
  ),

  /* META
   * ,------------------------------------------------.   ,------------------------------------------------.
   * |   1  |   2  |   3  |   4  |   5  |   6  |  [   |   |  ]   |   7  |   8  |   9  |   0  |   -  |  ^   |
   * |------+------+------+------+------+------+------|   |-------------+------+------+------+------+------|
   * | Alt  |  F1  |      |Muhen | Henk |      |  (   |   |  )   | Left | Down |  Up  |Right |      |      |
   * |------+------+------+------+------+------+------|   |------|------+------+------+------+------+------|
   * | Sft  |  F2  |  F3  |  F4  |  F5  |  F6  |  {   |   |  }   |  F7  |  F8  |  F9  | F10  | F11  |\/Sft |
   * |------+------+------+------+------+------+------|   |------+------+------+------+------+------+------|
   * | Ctrl | GUI  |  App |PrtSc |ESC/  |Space/|Tab/  |   |Back  |Enter/| Del  |Reset |=>GAME|=>SYMB|  \   |
   * |      |      |      |      |~SYMB |RCtrl |Shift |   |Space |~META |      |      |      |      |      |
   * `------------------------------------------------'   `------------------------------------------------'
   */
  [META] = LAYOUT( \
    _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,             KC_7,    KC_8,    KC_9,  KC_0,    KC_MINS, KC_EQL,  _______, \
    _______, XXXXXXX, XXXXXXX, KC_MHEN, KC_HENK, XXXXXXX, _______,          _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, XXXXXXX, XXXXXXX, \
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,   KC_F9, KC_F10,  KC_F11,  KC_F12,  SFT_T(KC_RO), \
    _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, RESET, _______, _______, _______ \
  ),

  /* SYMB
   * ,------------------------------------------------.   ,------------------------------------------------.
   * |   !  |   "  |   #  |   $  |   %  |   &  |  [   |   |  ]   |   '  |   (  |   )  |   ~  |   =  |  ~   |
   * |------+------+------+------+------+------+------|   |-------------+------+------+------+------+------|
   * | Alt  |      |      |      |      |      |  (   |   |  )   |      |      |      |      |   +  |  *   |
   * |------+------+------+------+------+------+------|   |------|------+------+------+------+------+------|
   * | Sft  |      |      |      |      |      |  {   |   |  }   |      |      |   <  |   >  |   ?  |  \   |
   * |------+------+------+------+------+------+------|   |------+------+------+------+------+------+------|
   * | Ctrl | GUI  |  App |PrtSc |ESC/  |Space/|Tab/  |   |Back  |Enter/| Del  |PrtSc |=>GAME|=>SYMB|  \   |
   * |      |      |      |      |~SYMB |RCtrl |Shift |   |Space |~META |      |      |      |      |      |
   * `------------------------------------------------'   `------------------------------------------------'
   */
  [SYMB] = LAYOUT( \
    S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6), _______, _______, S(KC_7), S(KC_8), S(KC_9),    S(KC_0),   S(KC_MINS), S(KC_EQL), \
    _______, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  _______, _______, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,   S(KC_SCLN), S(KC_QUOT), \
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  _______, _______, XXXXXXX, XXXXXXX, S(KC_COMM), S(KC_DOT), S(KC_SLSH), S(KC_RO), \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,   _______,    _______ \
  ),

  /* GAME
   * ,------------------------------------------------.   ,------------------------------------------------.
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |  [   |   |  ]   |   Y  |   U  |   I  |   O  |   P  |  @   |
   * |------+------+------+------+------+------+------|   |-------------+------+------+------+------+------|
   * | Alt  |   A  |   S  |   D  |   F  |   G  |  (   |   |  )   |   H  |   J  |   K  |   L  |   ;  |  :   |
   * |------+------+------+------+------+------+------|   |------|------+------+------+------+------+------|
   * | Sft  |   Z  |   X  |   C  |   V  |   B  |  {   |   |  }   |   N  |   M  |   ,  |   .  |   /  |\/Sft |
   * |------+------+------+------+------+------+------|   |------+------+------+------+------+------+------|
   * | Ctrl | GUI  |  App |PrtSc | ESC  |Space |Tab   |   |Back  |Enter | Del  |PrtSc |=>GAME|=>SYMB|  \   |
   * |      |      |      |      |      |      |      |   |Space |      |      |      |      |      |      |
   * `------------------------------------------------'   `------------------------------------------------'
   */
  [GAME] = LAYOUT( \
    KC_TAB,   KC_Q,    KC_W,   KC_E,    KC_R,   KC_T,   KC_RBRC,    KC_BSLS,    KC_Y,   KC_U,    KC_I,     KC_O,    KC_P,    KC_LBRC, \
    KC_LALT,  KC_A,    KC_S,   KC_D,    KC_F,   KC_G,   S(KC_8),    S(KC_9),    KC_H,   KC_J,    KC_K,     KC_L,    KC_SCLN, KC_QUOT, \
    KC_LSFT,  KC_Z,    KC_X,   KC_C,    KC_V,   KC_B,   S(KC_RBRC), S(KC_BSLS), KC_N,   KC_M,    KC_COMM,  KC_DOT,  KC_SLSH, SFT_T(KC_RO), \
    KC_LCTRL, KC_LGUI, KC_APP, KC_PSCR, KC_ESC, KC_SPC, KC_TAB,     KC_BSPC,    KC_ENT, KC_DELT, KC_PSCR,  _______, _______, KC_JYEN \
  ),

  [CTRL] = LAYOUT (\
   AD_WO_L, ADV_ID1, ADV_ID2, ADV_ID3,  ADV_ID4, ENT_SLP,  ENT_DFU,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX, \
   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, USB_DIS,  USB_EN,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX, \
   BATT_LV, DEL_ID1, XXXXXXX, XXXXXXX,  XXXXXXX, BLE_DIS,  BLE_EN,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX, \
   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  _______, XXXXXXX,  XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX  \
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  char str[16];
  if (record->event.pressed) {
    switch (keycode) {
    case DELBNDS:
      delete_bonds();
      return false;
    case AD_WO_L:
      restart_advertising_wo_whitelist();
      return false;
    case USB_EN:
      set_usb_enabled(true);
      return false;
      break;
    case USB_DIS:
      set_usb_enabled(false);
      return false;
      break;
    case BLE_EN:
      set_ble_enabled(true);
      return false;
      break;
    case BLE_DIS:
      set_ble_enabled(false);
      return false;
      break;
    case ADV_ID0:
      restart_advertising_id(0);
      return false;
    case ADV_ID1:
      restart_advertising_id(1);
      return false;
    case ADV_ID2:
      restart_advertising_id(2);
      return false;
    case ADV_ID3:
      restart_advertising_id(3);
      return false;
    case ADV_ID4:
      restart_advertising_id(4);
      return false;
    case DEL_ID0:
      delete_bond_id(0);
      return false;
    case DEL_ID1:
      delete_bond_id(1);
      return false;
    case DEL_ID2:
      delete_bond_id(2);
      return false;
    case DEL_ID3:
      delete_bond_id(3);
      return false;
    case BATT_LV:
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
      return false;
    case ENT_DFU:
      bootloader_jump();
      return false;
    }
  }
  else if (!record->event.pressed) {
    switch (keycode) {
    case ENT_SLP:
      sleep_mode_enter();
      return false;
    }

  }
  return true;
}
;
