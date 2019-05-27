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
    // BLE keys
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
    ADV_ID5,              /* Start advertising to PeerID 5        */
    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    DEL_ID5,              /* Delete bonding of PeerID 5           */
    ENT_DFU,              /* Start bootloader                     */
    ENT_SLP,              /* Deep sleep mode                      */

    // default layer control
    QWERTY,
    COLEMAK,
    DVORAK,
    EUCALYN,

    // layer control
    LOWER,
    RAISE,
    ADJUST,
    CONFIG,

    // other keys
    EISU,
    KANA,

    // combination keycode and layer
    RAISE_SPC,
    LOWER_SPC,
    RAISE_ENT,
    LOWER_ENT,
    CONFIG_EISU,
    CONFIG_KANA,
};

extern keymap_config_t keymap_config;

enum layer_number {
    _QWERTY = 0,
    _COLEMAK,
    _DVORAK,
    _EUCALYN,
    _LOWER,
    _RAISE,
    _ADJUST,
    _CONFIG,
    _LAYERSIZE,
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

// define KC_xxx code
#define KC_RAISE  RAISE
#define KC_LOWER  LOWER
#define KC_ADJ    ADJUST
#define KC_RASP   RAISE_SPC
#define KC_LOSP   LOWER_SPC
#define KC_RAEN   RAISE_ENT
#define KC_LOEN   LOWER_ENT
#define KC_RCEN   RCTL_T(KC_ENT)
#define KC_MAC    AG_NORM
#define KC_WIN    AG_SWAP
#define KC_SLP	  ENT_SLP

#define KC_xEISU  EISU
#define KC_cKANA  CONFIG_KANA

#define KC_____   _______
#define KC_XXXX   XXXXXXX

// define keymaps
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Qwerty
   * ,-------------------------------------------------------------------------------------------------.
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |   -  |   =  |   Y  |   U  |   I  |   O  |   P  |  \   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Ctrl |   A  |   S  |   D  |   F  |   G  |   [  |   ]  |   H  |   J  |   K  |   L  |   ;  | Ctrl |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |   `  |   '  |   N  |   M  |   ,  |   .  |   /  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | ESC  |      |  Alt |  GUI | kana | Space|             | Enter|  BS  | GUI  | Alt  |      | BS   |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_QWERTY] = LAYOUT_kc(
    TAB,     Q,    W,    E,    R,    T, MINS,  EQL,    Y,    U,    I,    O,    P, BSLS,
    LCTL,    A,    S,    D,    F,    G, LBRC, RBRC,    H,    J,    K,    L, SCLN, RCEN,
    LSFT,    Z,    X,    C,    V,    B,  GRV, QUOT,    N,    M, COMM,  DOT, SLSH, RSFT,
    ESC,        LALT, LGUI,cKANA, LOSP,             RAEN, BSPC, RGUI, RALT,       BSPC
    ),

  /* Colemak
   * ,-------------------------------------------------------------------------------------------------.
   * | Tab  |   Q  |   W  |   F  |   P  |   G  |   -  |   =  |   J  |   L  |   U  |   Y  |   ;  | \    |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Ctrl |   A  |   R  |   S  |   T  |   D  |   [  |   ]  |   H  |   N  |   E  |   I  |   O  | Ctrl |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |   `  |   '  |   K  |   M  |   ,  |   .  |   /  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | ESC  |      |  Alt |  GUI | kana | Space|             | Enter|  BS  | GUI  | Alt  |      | BS   |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_COLEMAK] = LAYOUT_kc(
    TAB,     Q,    W,    F,    P,    G, MINS,  EQL,    J,    L,    U,    Y, SCLN, BSLS,
    LCTL,    A,    R,    S,    T,    D, LBRC, RBRC,    H,    N,    E,    I,    O, RCEN,
    LSFT,    Z,    X,    C,    V,    B,  GRV, QUOT,    K,    M, COMM,  DOT, SLSH, RSFT,
    ESC,        LALT, LGUI,cKANA, LOSP,             RAEN, BSPC, RGUI, RALT,       BSPC
    ),

  /* Dvorak
   * ,-------------------------------------------------------------------------------------------------.
   * | Tab  |   '  |   ,  |   .  |   P  |   Y  |   -  |   =  |   F  |   G  |   C  |   R  |   L  |  \   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Ctrl |   A  |   O  |   E  |   U  |   I  |   [  |   =  |   D  |   H  |   T  |   N  |   S  | Ctrl |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   ;  |   Q  |   J  |   K  |   X  |   `  |   /  |   B  |   M  |   W  |   V  |   Z  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | ESC  |      |  Alt |  GUI | kana | Space|             | Enter|  BS  | GUI  | Alt  |      | BS   |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_DVORAK] = LAYOUT_kc(
    TAB,  QUOT, COMM,  DOT,    P,    Y, MINS,  EQL,    F,    G,    C,    R,    L, BSLS,
    LCTL,    A,    O,    E,    U,    I, LBRC, RBRC,    D,    H,    T,    N,    S, RCEN,
    LSFT, SCLN,    Q,    J,    K,    X,  GRV, SLSH,    B,    M,    W,    V,    Z, RSFT,
    ESC,        LALT, LGUI,cKANA, LOSP,             RAEN, BSPC, RGUI, RALT,       BSPC
    ),

  /* Eucalyn (http://eucalyn.hatenadiary.jp/entry/about-eucalyn-layout)
   * ,-------------------------------------------------------------------------------------------------.
   * | Tab  |   Q  |   W  |   ,  |   .  |   ;  |   -  |   =  |   M  |   R  |   D  |   Y  |   P  |  \   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Ctrl |   A  |   O  |   E  |   I  |   U  |   [  |   ]  |   G  |   T  |   K  |   S  |   N  | Ctrl |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   F  |   `  |   '  |   B  |   H  |   J  |   L  |   /  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | ESC  |      |  Alt |  GUI | kana | Space|             | Enter|  BS  | GUI  | Alt  |      | BS   |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_EUCALYN] = LAYOUT_kc(
    TAB,     Q,    W, COMM,  DOT, SCLN, MINS,  EQL,    M,    R,    D,    Y,    P, BSLS,
    LCTL,    A,    O,    E,    I,    U, LBRC, RBRC,    G,    T,    K,    S,    N, RCEN,
    LSFT,    Z,    X,    C,    V,    F,  GRV, QUOT,    B,    H,    J,    L, SLSH, RSFT,
    ESC,        LALT, LGUI,cKANA, LOSP,             RAEN, BSPC, RGUI, RALT,       BSPC
    ),

  /* Raise/Lower
   * ,-------------------------------------------------------------------------------------------------.
   * | CAPS | ! 1  | @ 2  | # 3  | $ 4  | % 5  |      |      | ^ 6  | & 7  | * 8  | ( 9  | ) 0  |  INS |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      | Home |      | Up   |Delete| PgUp |      |      | PgUp |Delete| Up   |      | Home |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      | End  | Left | Down | Right| PgDn |      |      | PgDn | Left | Down | Right| End  |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      | PrtSc| Pause| eisu |      |             |      |  DEL | Pause| PrtSc|      |  DEL |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_RAISE] = LAYOUT_kc(
    CAPS,    1,    2,    3,    4,    5,  F11,  F12,    6,    7,    8,    9,    0,  INS,
    ____, HOME, XXXX, UP,   XXXX, PGUP, XXXX, XXXX, PGUP, XXXX,   UP, XXXX, HOME, ____,
    ____, END,  LEFT, DOWN, RGHT, PGDN, XXXX, XXXX, PGDN, LEFT, DOWN, RGHT,  END, ____,
    ____,       PSCR, PAUS,xEISU, ____,             ____,  DEL, PAUS, PSCR,        DEL 
    ),
  [_LOWER] = LAYOUT_kc(
    CAPS, EXLM,   AT, HASH,  DLR, PERC,  F11,  F12, CIRC, AMPR, ASTR, LPRN, RPRN,  INS,
    ____, HOME, XXXX, UP,   XXXX, PGUP, XXXX, XXXX, PGUP, XXXX,   UP, XXXX, HOME, ____,
    ____, END,  LEFT, DOWN, RGHT, PGDN, XXXX, XXXX, PGDN, LEFT, DOWN, RGHT,  END, ____,
    ____,       PSCR, PAUS,xEISU, ____,             ____,  DEL, PAUS, PSCR,        DEL 
    ),

  /* Adjust
   * ,-------------------------------------------------------------------------------------------------.
   * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F11 |  F12 |  F6  |  F7  |  F8  |  F9  |  F10 |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_ADJUST] =  LAYOUT_kc(
    XXXX,   F1,   F2,   F3,   F4,   F5,  F11,  F12,   F6,   F7,   F8,   F9,  F10, XXXX,
    ____, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, ____,
    ____, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, ____,
    ____,       ____, ____, ____, ____,             ____, XXXX, ____, ____,       ____ 
    ),

  /* Config
   * ,-------------------------------------------------------------------------------------------------.
   * | DFU  | ADV1 | ADV2 | ADV3 | ADV4 | ADV5 | MAC  | MAC  |      |      |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      | DEL1 | DEL2 | DEL3 | DEL4 | DEL5 | WIN  | WIN  |QWERTY|COLEMK|DVORAK|EUCLYN|      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |BLEDIS|BLE_EN|USBDIS|USB_EN| ADV  | BATT |      |AU_ON |AU_OFF|      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_CONFIG] =  LAYOUT(
    ENT_DFU, ADV_ID1, ADV_ID2, ADV_ID3, ADV_ID4, ADV_ID5, AG_NORM, AG_NORM, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, DEL_ID1, DEL_ID2, DEL_ID3, DEL_ID4, DEL_ID5, AG_SWAP, AG_SWAP, QWERTY,  COLEMAK, DVORAK,  EUCALYN, XXXXXXX, _______,
    _______, BLE_DIS, BLE_EN,  USB_DIS, USB_EN,  AD_WO_L, BATT_LV, XXXXXXX, AU_ON,   AU_OFF,  XXXXXXX, XXXXXXX, XXXXXXX, _______,
    _______,          _______, _______, _______, _______,                   _______, XXXXXXX, _______, _______,          _______
    ),
};

static uint16_t layer_pressed_time[_LAYERSIZE];

#ifdef TAPPING_TERM_PER_KEY
#define TAP_TIME(kc)	(get_tapping_term(kc))
#else
#define TAP_TIME(kc)	(TAPPING_TERM)
#endif

static void set_layer_and_key(int layer, uint16_t kc, keyrecord_t *record) {
  if (record->event.pressed) {
    layer_pressed_time[layer] = record->event.time;
    layer_on(layer);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
  } else {
    layer_off(layer);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
    if (layer_pressed_time[layer] && 
	(TIMER_DIFF_16(record->event.time, layer_pressed_time[layer]) < TAP_TIME(kc))) {
      switch (kc) {
      case KANA:
	if (!keymap_config.swap_lalt_lgui){
	  register_code(KC_LANG1);
	  unregister_code(KC_LANG1);
	} else {
	  SEND_STRING(SS_LALT("`"));
	}
	break;
      case EISU:
	if (!keymap_config.swap_lalt_lgui){
	  register_code(KC_LANG2);
	  unregister_code(KC_LANG2);
	} else {
	  SEND_STRING(SS_LALT("`"));
	}
	break;
      default:
	register_code(kc);
	unregister_code(kc);
	break;
      }
    }
    layer_pressed_time[layer] = 0;
  }
}

static void set_layer(int layer, keyrecord_t *record) {
  if (record->event.pressed) {
    layer_pressed_time[layer] = record->event.time;
    layer_on(layer);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
  } else {
    layer_off(layer);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
    layer_pressed_time[layer] = 0;
  }
}

static void unset_layer(keyrecord_t *record) {
  if (record->event.pressed) {
    memset(layer_pressed_time, 0, sizeof(layer_pressed_time));
  }
}

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
  layer_state_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // layer function
  case LOWER:
    set_layer(_LOWER, record);
    return false;
  case RAISE:
    set_layer(_RAISE, record);
    return false;
  case ADJUST:
    set_layer(_ADJUST, record);
    return false;
  case CONFIG:
    set_layer(_CONFIG, record);
    return false;

    // combine keycode and layer key
  case RAISE_SPC:
    set_layer_and_key(_RAISE, KC_SPC, record);
    return false;
  case LOWER_SPC:
    set_layer_and_key(_LOWER, KC_SPC, record);
    return false;
  case RAISE_ENT:
    set_layer_and_key(_RAISE, KC_ENT, record);
    return false;
  case LOWER_ENT:
    set_layer_and_key(_LOWER, KC_ENT, record);
    return false;
  case CONFIG_EISU:
    set_layer_and_key(_CONFIG, EISU, record);
    return false;
  case CONFIG_KANA:
    set_layer_and_key(_CONFIG, KANA, record);
    return false;

    // special function key
  case QWERTY:
    if (record->event.pressed) {
#ifdef AUDIO_ENABLE
      PLAY_SONG(tone_qwerty);
#endif
      persistent_default_layer_set(1UL<<_QWERTY);
    }
    break;
  case COLEMAK:
    if (record->event.pressed) {
#ifdef AUDIO_ENABLE
      PLAY_SONG(tone_colemak);
#endif
      persistent_default_layer_set(1UL<<_COLEMAK);
    }
    break;
  case DVORAK:
    if (record->event.pressed) {
#ifdef AUDIO_ENABLE
      PLAY_SONG(tone_dvorak);
#endif
      persistent_default_layer_set(1UL<<_DVORAK);
    }
    break;
  case EUCALYN:
    if (record->event.pressed) {
#ifdef AUDIO_ENABLE
      PLAY_SONG(tone_dvorak);
#endif
      persistent_default_layer_set(1UL<<_EUCALYN);
    }
    break;
  case EISU:
    if (record->event.pressed) {
      if (!keymap_config.swap_lalt_lgui){
	register_code(KC_LANG2);
      } else {
	SEND_STRING(SS_LALT("`"));
      }
    } else {
      unregister_code(KC_LANG2);
    }
    break;
  case KANA:
    if (record->event.pressed) {
      if (!keymap_config.swap_lalt_lgui){
	register_code(KC_LANG1);
      } else {
	SEND_STRING(SS_LALT("`"));
      }
    } else {
      unregister_code(KC_LANG1);
    }
    break;
  case DELBNDS:
    if (record->event.pressed)
      delete_bonds();
    break;
  case AD_WO_L:
    if (record->event.pressed)
      restart_advertising_wo_whitelist();
    break;
  case USB_EN:
  case USB_DIS:
    if (record->event.pressed)
      set_usb_enabled(keycode == USB_EN);
    break;
  case BLE_EN:
  case BLE_DIS:
    if (record->event.pressed)
      set_ble_enabled(keycode == BLE_EN);
    break;
  case ADV_ID0:
  case ADV_ID1:
  case ADV_ID2:
  case ADV_ID3:
  case ADV_ID4:
  case ADV_ID5:
    if (record->event.pressed)
      restart_advertising_id(keycode-ADV_ID0);
    break;
  case DEL_ID0:
  case DEL_ID1:
  case DEL_ID2:
  case DEL_ID3:
  case DEL_ID4:
  case DEL_ID5:
    if (record->event.pressed)
      delete_bond_id(keycode-DEL_ID0);
    break;
  case BATT_LV:
    if (record->event.pressed) {
      char str[16];

      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
    }
    break;
  case ENT_DFU:
    if (record->event.pressed)
      bootloader_jump();
    break;
  case ENT_SLP:
    if (!record->event.pressed)
      sleep_mode_enter();
    break;

    // and... default
  default:
    unset_layer(record);
    return true;
  }
  unset_layer(record);
  return false;
}

#ifdef TAPPING_TERM_PER_KEY
uint16_t get_tapping_term(uint16_t keycode) {
  switch (keycode) {
  default:
    return TAPPING_TERM;
  }
}
#endif
