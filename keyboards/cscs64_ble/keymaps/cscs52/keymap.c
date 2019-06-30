/*
Copyright 2018 Sekigon
Copyright 2019 hatano.h

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
    SEL_BLE,		  /* Select BLE HID Sending		  */
    SEL_USB,		  /* Select USB HID Sending		  */
    TOG_HID,		  /* Toggle BLE/USB HID Sending		  */
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

    // other keys
    EISU,
    KANA,

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
#define KC_MAC    AG_SWAP
#define KC_WIN    AG_NORM
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
   * | ESC  |      |  GUI |  Alt | kana | Space|             | Enter|  BS  | Alt  | GUI  |      | BS   |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_QWERTY] = LAYOUT_kc(
    TAB,     Q,    W,    E,    R,    T, MINS,  EQL,    Y,    U,    I,    O,    P, BSLS,
    LCTL,    A,    S,    D,    F,    G, LBRC, RBRC,    H,    J,    K,    L, SCLN, RCEN,
    LSFT,    Z,    X,    C,    V,    B,  GRV, QUOT,    N,    M, COMM,  DOT, SLSH, RSFT,
    ESC,        LGUI, LALT,cKANA, LOSP,             RAEN, BSPC, RALT, RGUI,       BSPC
    ),

  /* Colemak
   * ,-------------------------------------------------------------------------------------------------.
   * | Tab  |   Q  |   W  |   F  |   P  |   G  |   -  |   =  |   J  |   L  |   U  |   Y  |   ;  | \    |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Ctrl |   A  |   R  |   S  |   T  |   D  |   [  |   ]  |   H  |   N  |   E  |   I  |   O  | Ctrl |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |   `  |   '  |   K  |   M  |   ,  |   .  |   /  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | ESC  |      |  GUI |  Alt | kana | Space|             | Enter|  BS  | Alt  | GUI  |      | BS   |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_COLEMAK] = LAYOUT_kc(
    TAB,     Q,    W,    F,    P,    G, MINS,  EQL,    J,    L,    U,    Y, SCLN, BSLS,
    LCTL,    A,    R,    S,    T,    D, LBRC, RBRC,    H,    N,    E,    I,    O, RCEN,
    LSFT,    Z,    X,    C,    V,    B,  GRV, QUOT,    K,    M, COMM,  DOT, SLSH, RSFT,
    ESC,        LGUI, LALT,cKANA, LOSP,             RAEN, BSPC, RALT, RGUI,       BSPC
    ),

  /* Dvorak
   * ,-------------------------------------------------------------------------------------------------.
   * | Tab  |   '  |   ,  |   .  |   P  |   Y  |   -  |   =  |   F  |   G  |   C  |   R  |   L  |  \   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Ctrl |   A  |   O  |   E  |   U  |   I  |   [  |   =  |   D  |   H  |   T  |   N  |   S  | Ctrl |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   ;  |   Q  |   J  |   K  |   X  |   `  |   /  |   B  |   M  |   W  |   V  |   Z  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | ESC  |      |  GUI |  Alt | kana | Space|             | Enter|  BS  | Alt  | GUI  |      | BS   |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_DVORAK] = LAYOUT_kc(
    TAB,  QUOT, COMM,  DOT,    P,    Y, MINS,  EQL,    F,    G,    C,    R,    L, BSLS,
    LCTL,    A,    O,    E,    U,    I, LBRC, RBRC,    D,    H,    T,    N,    S, RCEN,
    LSFT, SCLN,    Q,    J,    K,    X,  GRV, SLSH,    B,    M,    W,    V,    Z, RSFT,
    ESC,        LGUI, LALT,cKANA, LOSP,             RAEN, BSPC, RALT, RGUI,       BSPC
    ),

  /* Eucalyn (http://eucalyn.hatenadiary.jp/entry/about-eucalyn-layout)
   * ,-------------------------------------------------------------------------------------------------.
   * | Tab  |   Q  |   W  |   ,  |   .  |   ;  |   -  |   =  |   M  |   R  |   D  |   Y  |   P  |  \   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Ctrl |   A  |   O  |   E  |   I  |   U  |   [  |   ]  |   G  |   T  |   K  |   S  |   N  | Ctrl |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   F  |   `  |   '  |   B  |   H  |   J  |   L  |   /  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | ESC  |      |  GUI |  Alt | kana | Space|             | Enter|  BS  | Alt  | GUI  |      | BS   |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_EUCALYN] = LAYOUT_kc(
    TAB,     Q,    W, COMM,  DOT, SCLN, MINS,  EQL,    M,    R,    D,    Y,    P, BSLS,
    LCTL,    A,    O,    E,    I,    U, LBRC, RBRC,    G,    T,    K,    S,    N, RCEN,
    LSFT,    Z,    X,    C,    V,    F,  GRV, QUOT,    B,    H,    J,    L, SLSH, RSFT,
    ESC,        LGUI, LALT,cKANA, LOSP,             RAEN, BSPC, RALT, RGUI,       BSPC
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
   * | DFU  | ADV1 | ADV2 | ADV3 | ADV4 | ADV5 | MAC  | MAC  | DEL1 | DEL2 | DEL3 | DEL4 | DEL5 |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      | WIN  | WIN  |QWERTY|COLEMK|DVORAK|EUCLYN|      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |SELBLE|SELUSB|      |      | ADV  | BATT |      |AU_ON |AU_OFF|      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_CONFIG] =  LAYOUT(
    ENT_DFU, ADV_ID1, ADV_ID2, ADV_ID3, ADV_ID4, ADV_ID5, AG_SWAP, AG_SWAP, DEL_ID1, DEL_ID2, DEL_ID3, DEL_ID4, DEL_ID5, XXXXXXX,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, AG_NORM, AG_NORM, QWERTY,  COLEMAK, DVORAK,  EUCALYN, XXXXXXX, _______,
    _______, SEL_BLE, SEL_USB, XXXXXXX, XXXXXXX, AD_WO_L, BATT_LV, XXXXXXX, AU_ON,   AU_OFF,  XXXXXXX, XXXXXXX, XXXXXXX, _______,
    _______,          _______, _______, _______, _______,                   _______, XXXXXXX, _______, _______,          _______
    ),
};

struct layer_press {
  bool pressed;
  uint16_t time;
};
static struct layer_press layer_pressed[_LAYERSIZE];

static void set_layer(int layer, keyrecord_t *record);
static void unset_layer(keyrecord_t *record);
static void set_layer_and_key(int layer, uint16_t kc, keyrecord_t *record);
static bool process_record_user_special(uint16_t keycode, bool pressed);

#ifndef TAPPING_TERM_PER_KEY
#define get_tapping_term(kc)	(TAPPING_TERM)
#endif

static void set_layer(int layer, keyrecord_t *record) {
  if (record->event.pressed) {
    layer_pressed[layer].time = record->event.time;
    layer_pressed[layer].pressed = true;
    layer_on(layer);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
  } else {
    layer_off(layer);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
    layer_pressed[layer].pressed = false;
  }
}

static void unset_layer(keyrecord_t *record) {
  if (record->event.pressed) {
    memset(layer_pressed, 0, sizeof(layer_pressed));
  }
}

static void set_layer_and_key(int layer, uint16_t kc, keyrecord_t *record) {
  if (record->event.pressed) {
    layer_pressed[layer].time = record->event.time;
    layer_pressed[layer].pressed = true;
    layer_on(layer);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
  } else {
    layer_off(layer);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
    if (layer_pressed[layer].pressed && 
	(TIMER_DIFF_16(record->event.time, layer_pressed[layer].time) < get_tapping_term(kc))) {
      if (process_record_user_special(kc, true))
	register_code(kc);
      if (process_record_user_special(kc, false))
	unregister_code(kc);
    }
    layer_pressed[layer].pressed = false;
  }
}

static bool process_record_user_special(uint16_t keycode, bool pressed) {
  switch (keycode) {
  case QWERTY:
    if (pressed) {
#ifdef AUDIO_ENABLE
      PLAY_SONG(tone_qwerty);
#endif
      set_single_persistent_default_layer(_QWERTY);
    }
    break;
  case COLEMAK:
    if (pressed) {
#ifdef AUDIO_ENABLE
      PLAY_SONG(tone_colemak);
#endif
      set_single_persistent_default_layer(_COLEMAK);
    }
    break;
  case DVORAK:
    if (pressed) {
#ifdef AUDIO_ENABLE
      PLAY_SONG(tone_dvorak);
#endif
      set_single_persistent_default_layer(_DVORAK);
    }
    break;
  case EUCALYN:
    if (pressed) {
#ifdef AUDIO_ENABLE
      PLAY_SONG(tone_dvorak);
#endif
      set_single_persistent_default_layer(_EUCALYN);
    }
    break;
  case EISU:
    if (pressed) {
      if (keymap_config.swap_lalt_lgui){
	register_code(KC_LANG2);
      } else {
	SEND_STRING(SS_LALT("`"));
      }
    } else {
      unregister_code(KC_LANG2);
    }
    break;
  case KANA:
    if (pressed) {
      if (keymap_config.swap_lalt_lgui){
	register_code(KC_LANG1);
      } else {
	SEND_STRING(SS_LALT("`"));
      }
    } else {
      unregister_code(KC_LANG1);
    }
    break;
  case DELBNDS:
    if (pressed)
      delete_bonds();
    break;
  case AD_WO_L:
    if (pressed)
      restart_advertising_wo_whitelist();
    break;
  case SEL_BLE:
    if (pressed) {
      set_ble_enabled(true);
      set_usb_enabled(false);
    }
    break;
  case SEL_USB:
    if (pressed) {
      set_ble_enabled(false);
      set_usb_enabled(true);
    }
    break;
  case TOG_HID:
#ifndef NRF_SEPARATE_KEYBOARD_SLAVE
    // workaround:
    // get_ble_enabled() macro(in app_ble_func.h) is incorrect.
    if (pressed) {
      bool ble = get_ble_enabled();
      
      set_ble_enabled(!ble);
      set_usb_enabled(ble);
    }
#endif
    break;
  case ADV_ID0:
  case ADV_ID1:
  case ADV_ID2:
  case ADV_ID3:
  case ADV_ID4:
  case ADV_ID5:
    if (pressed)
      restart_advertising_id(keycode-ADV_ID0);
    break;
  case DEL_ID0:
  case DEL_ID1:
  case DEL_ID2:
  case DEL_ID3:
  case DEL_ID4:
  case DEL_ID5:
    if (pressed)
      delete_bond_id(keycode-DEL_ID0);
    break;
  case BATT_LV:
    if (pressed) {
      char str[16];

      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
    }
    break;
  case ENT_DFU:
    if (pressed)
      bootloader_jump();
    break;
  case ENT_SLP:
    if (!pressed)
      sleep_mode_enter();
    break;

  default:
    // other unspecial keys
    return true;
  }
  return false;
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // layer function
  case LOWER:
    set_layer(_LOWER, record);
    break;
  case RAISE:
    set_layer(_RAISE, record);
    break;
  case ADJUST:
    set_layer(_ADJUST, record);
    break;
  case CONFIG:
    set_layer(_CONFIG, record);
    break;

    // combine keycode and layer key
  case RAISE_SPC:
    set_layer_and_key(_RAISE, KC_SPC, record);
    break;
  case LOWER_SPC:
    set_layer_and_key(_LOWER, KC_SPC, record);
    break;
  case RAISE_ENT:
    set_layer_and_key(_RAISE, KC_ENT, record);
    break;
  case LOWER_ENT:
    set_layer_and_key(_LOWER, KC_ENT, record);
    break;
  case CONFIG_EISU:
    set_layer_and_key(_CONFIG, EISU, record);
    break;
  case CONFIG_KANA:
    set_layer_and_key(_CONFIG, KANA, record);
    break;

    // special function key
  default:
    unset_layer(record);
    return process_record_user_special(keycode, record->event.pressed);
  }
  return false;
}
