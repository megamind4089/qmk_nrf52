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
    QWERTY,
    COLEMAK,
    DVORAK,
    LOWER,
    RAISE,
    ADJUST,
    //BACKLIT,
    //EISU,
    //KANA,
    //RGBRST
};

extern keymap_config_t keymap_config;

enum layer_number {
    _TWELVE = 0,
    _LOWER,
//    _RAISE,
};

const uint16_t PROGMEM
keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_TWELVE] =

LAYOUT( \
 LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), KC_UP, KC_DEL, KC_BSPC, \
 KC_LCTRL, KC_LSFT, KC_LEFT, KC_DOWN, KC_RIGHT, LT(_LOWER, KC_ENT) \


),
[_LOWER] =
LAYOUT( \
 AD_WO_L, ADV_ID0, ADV_ID1, DEL_ID0, BATT_LV, ENT_SLP, \
 ENT_DFU, USB_DIS, USB_EN, BLE_DIS, BLE_EN, _______ \

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
  } else if (!record->event.pressed) {
    switch (keycode) {
      case ENT_SLP:
        sleep_mode_enter();
        return false;
    }
  }
  return true;
};
