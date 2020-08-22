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
extern bool has_usb(void);

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
    _QWERTY = 0,
    _LOWER,
    _RAISE,
};

#define _______ KC_TRNS
#define XXXXXXX KC_NO


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* QWERTY
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  ~   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LCTRL |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
     * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
     * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |   \  |
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *                   |LOWER | LGUI | Alt  | /Space  /       \Enter \  |BackSP| RGUI |RAISE |
     *                   |      |      |      |/       /         \      \ |      |      |      |
     *                   `-------------------''-------'           '------''--------------------'
     */

    [_QWERTY] = LAYOUT( \
                        KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV, \
                        KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
                        KC_LCTRL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                   KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
                        KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC, KC_RBRC, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS, \
                                              MO(_LOWER), KC_LGUI, KC_LALT, KC_SPC,  KC_ENT,  KC_BSPC, MO(_RAISE), KC_LCTRL \
                      ),
    /* LOWER
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |AD_WOL|ADVID1|ADVID2|ADVID3|ADVID4|      |                    |      |      |      |      |      |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |      |      |ENTSLP|ENTDFU|                    |      |      |      |      |      |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |      |      |USBDIS|USB_EN|-------.    ,-------|   ~  |   !  |   @  |   #  |   $  |   %  |
     * |------+------+------+------+------+------|   X   |    |    X  |------+------+------+------+------+------|
     * |BATTLV|DELID1|      |      |BLEDIS|BLE_EN|-------|    |-------|   ^  |   &  |   *  |   (  |   )  |   |  |
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *                   |LOWER | LGUI | Alt  | /Space  /       \Enter \  |BackSP| RGUI |RAISE |
     *                   |      |      |      |/       /         \      \ |      |      |      |
     *                   `-------------------''-------'           '------''--------------------'
     */
    [_LOWER] = LAYOUT( \
                       AD_WO_L, ADV_ID1, ADV_ID2, ADV_ID3, ADV_ID4, XXXXXXX,                  _______, _______, _______, _______, _______, _______,\
                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, ENT_SLP, ENT_DFU,                  _______, _______, _______, _______, _______, _______, \
                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, USB_DIS, USB_EN,                   KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, \
                       BATT_LV, XXXXXXX, XXXXXXX, XXXXXXX, BLE_DIS, BLE_EN, DELBNDS, DELBNDS, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE, \
                       _______, _______, _______, _______, _______,  _______, _______, _______\
                     ),
    /* RAISE
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |   `  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |      |      |      |      |-------.    ,-------|      | Left | Down |  Up  |Right |      |
     * |------+------+------+------+------+------|   X   |    |    X  |------+------+------+------+------+------|
     * |      |      |      |      |      |      |-------|    |-------|   +  |   -  |   =  |   [  |   ]  |   \  |
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *                   |LOWER | LGUI | Alt  | /Space  /       \Enter \  |BackSP| RGUI |RAISE |
     *                   |      |      |      |/       /         \      \ |      |      |      |
     *                   `-------------------''-------'           '------''--------------------'
     */

    [_RAISE] = LAYOUT( \
                       KC_F1,  KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
                       KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
                       _______, _______, _______, _______, _______, _______,                   XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, \
                       _______, _______, _______, _______, _______, _______, DELBNDS, DELBNDS, KC_PLUS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, \
                       _______, _______, _______, _______, _______, _______, _______, _______ \
                     )
};

size_t sizeof_keymaps() {
    return sizeof(keymaps);
}

void persistent_default_layer_set(uint16_t default_layer) {
    eeconfig_update_default_layer(default_layer);
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

#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation)
{
#if (IS_LEFT_HAND)
    return OLED_ROTATION_0;
#else
    return OLED_ROTATION_180;
#endif
}

#define get_highest_layer(state) biton32(state)

void oled_task_user(void)
{
    char str[64] = {0}, vc[16] = {0};
    int vcc = get_vcc();

#if (IS_LEFT_HAND)
    sprintf (str, "Master: %s%s%s",
             get_usb_enabled() ? "USB mode":"",
             get_ble_enabled() && ble_connected() ? "connected":"",
             get_ble_enabled() && !ble_connected() ? "disconnected":""
            );
#else
    sprintf(str, "Slave: %s", ble_connected() ? "connected" : "disconnected");
#endif
    oled_write_ln_P(PSTR(str), false);
    oled_write_ln_P("\n", false);

    memset(str, 0, sizeof(str));
#if (IS_LEFT_HAND)
    oled_write_P(PSTR("Layer: "), false);
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
        default:
            sprintf(str, "UND");
    }
#else
    sprintf(str, " ");
#endif
    oled_write_ln_P(PSTR(str), false);
    oled_write_ln_P("\n\n", false);

    memset(str, 0, sizeof str);
    sprintf(vc, "%4dmV", vcc);
    sprintf(str, "Bat: %s USB: %s", vcc==0 ? "off   " : vc, has_usb()? "on":"off");
    oled_write_ln_P(PSTR(str), false);
    oled_write_ln_P("\n", false);
}

#endif
