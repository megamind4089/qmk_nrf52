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

extern void nrfmicro_power_enable(bool);
bool has_usb(void);

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
    MOUSE,
    MY_OS,
};


extern keymap_config_t keymap_config;

enum layer_number {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _MOUSE,
    _EMPTY
};

#define __QWERT TO(_QWERTY)
#define __RAISE MO(_RAISE)
#define __LOWER LT(_LOWER, KC_GRV)
#define __MOUSE TG(_MOUSE)

#define __S_CTL MT(MOD_LGUI, KC_SCLN)

#undef _______
#define _______ KC_NO
#define XXXXXXX KC_NO


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT(
                       KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,
                       KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    __S_CTL, KC_RBRC,
                       KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                     KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
                       KC_LALT, __LOWER, KC_TAB,  KC_LGUI, KC_LSFT, KC_SPC, KC_LCTL, KC_ENT,  KC_BSPC, __RAISE, KC_MINS, KC_QUOT, KC_EQL,  KC_DEL
                      ),

    [_RAISE] = LAYOUT(
                      ENT_SLP, KC_EXLM, KC_AT,   KC_UP,   KC_LCBR, KC_RCBR,                   KC_PGUP, KC_7,    KC_8,    KC_9,    KC_ASTR, KC_LPRN,
                      KC_HOME, KC_HASH, KC_LEFT, KC_DOWN, KC_RGHT, KC_DLR,                    KC_PGDN, KC_4,    KC_5,    KC_6,    KC_PLUS, KC_RPRN,
                      KC_END,  KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN, KC_AMPR,                   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS, KC_PIPE,
                      _______, KC_TRNS, __MOUSE, KC_LGUI, KC_LSFT, KC_BSPC, KC_LCTL, KC_ENT,  KC_SPC,  KC_TRNS, KC_DOT,  KC_0,    KC_EQL,  KC_TRNS
                     ),

    [_LOWER] = LAYOUT(
                      _______, KC_INS,  KC_HOME, KC_UP,   KC_END,  KC_PGUP,                   KC_UP,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  ENT_DFU,
                      _______, KC_DEL,  KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,                   KC_DOWN, KC_F4,   KC_F5,   KC_F6,   KC_F11,  DELBNDS,
                      _______, _______, KC_VOLU, _______, _______, _______,                   KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F12,  ENT_SLP,
                      _______, KC_TRNS, KC_VOLD, KC_LGUI, KC_LSFT, KC_BSPC, KC_LCTL, KC_ENT,  KC_SPC,  KC_TRNS, KC_PSCR, KC_SLCK, KC_PAUS, KC_TRNS
                     ),

    [_MOUSE] = LAYOUT(
                      _______, _______, KC_BTN1, KC_WH_U, KC_BTN2, _______,                   _______, _______, KC_MS_U, _______, _______, _______,
                      _______, _______, KC_WH_L, KC_WH_D, KC_WH_R, _______,                   _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______,
                      _______, _______, _______, _______, _______, _______,                   _______, KC_ACL0, KC_ACL1, KC_ACL2, _______, _______,
                      _______, _______, _______, _______, _______, _______, _______, _______, _______, __QWERT, _______, _______, _______, _______
                     ),

    [_EMPTY] = LAYOUT(
                      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
                     ),

};

void persistent_default_layer_set(uint16_t default_layer) {
    default_layer_set(default_layer);
    layer_state_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    char str[16];

    nrfmicro_power_enable(true);

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
            oled_clear();
            oled_render();
            oled_write_ln_P(PSTR("\n\nBOOTLOADER"), false);
            oled_render();
            bootloader_jump();
            return false;
        case RESET:
            oled_clear();
            oled_render();
            oled_write_ln_P(PSTR("\n\nRESET"), false);
            oled_render();
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


#ifdef OLED_DRIVER_ENABLE

#define get_highest_layer(state) biton32(state)

void oled_task_user(void) {
    char vc[16] = {0}, str[64] = {0};
    int vcc = get_vcc();

    sprintf(str, "%s", ble_connected() ? "   CONN" : "  DISCON");
    oled_write_ln_P(PSTR(str), false);
    oled_write_ln_P(PSTR("\n\n\n"), false);

    oled_write_ln_P(PSTR("Layer:\n"), false);
    switch (get_highest_layer(layer_state)) {
    case _QWERTY:
        sprintf(str, "Default");
        break;
    case _RAISE:
        sprintf(str, "Raise");
        break;
    case _LOWER:
        sprintf(str, "Lower");
        break;
    case _MOUSE:
        sprintf(str, "Mouse");
        break;
    default:
        sprintf(str, "UND");
        break;
    }
    oled_write_ln_P(PSTR(str), false);
    oled_write_ln_P(PSTR("\n\n\n\n"), false);

    sprintf(str, "USB: %s", has_usb()? "on":"off");
    oled_write_ln_P(PSTR(str), false);
    oled_write_ln_P(PSTR("\n\n"), false);

    sprintf(vc, "%4dmV", vcc);
    sprintf(str, "Bat:\n%s", vcc==0 ? "off   " : vc);
    oled_write_ln_P(PSTR(str), false);
    oled_write_ln_P(PSTR("\n\n\n"), false);

    sprintf(str, "%s %s", get_usb_enabled()? " USB ":"     ", get_ble_enabled()?" BT  ":"     ");
    oled_write_ln_P(PSTR(str), false);
}
#endif

