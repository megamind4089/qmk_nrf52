/* Copyright 2019 Salicylic_Acid
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "app_ble_func.h"

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

enum layer_number {
  _QWERTY = 0,
  _FLOCK,
  _LOWER,
  _RAISE,
  _ADJUST,
};

enum custom_keycodes {
  FLOCK = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  KANJI,
  RGBRST,
  AD_WO_L,              /* Start advertising without whitelist  */
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

enum tapdances{
  TD_ESFL = 0,
  TD_ESQW,
};

extern keymap_config_t keymap_config;

// Layer Mode aliases
#define KC_MLAD  MO(_ADJUST)

#define KC_LOWER LOWER
#define KC_RAISE RAISE
#define KC_ADJUST ADJUST

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_KANJI KANJI

#define KC_LRST  RGBRST
#define KC_LTOG  RGB_TOG
#define KC_LHUI  RGB_HUI
#define KC_LHUD  RGB_HUD
#define KC_LSAI  RGB_SAI
#define KC_LSAD  RGB_SAD
#define KC_LVAI  RGB_VAI
#define KC_LVAD  RGB_VAD
#define KC_LMOD  RGB_MOD
#define KC_KNRM  AG_NORM
#define KC_KSWP  AG_SWAP
#define KC_DEBUG DEBUG

#define KC_RST   ENT_DFU
#define KC_BATT  BATT_LV
#define KC_BTON  BLE_EN
#define KC_BTOF  BLE_DIS
#define KC_USBON USB_EN
#define KC_USBOF USB_DIS
#define KC_BTNEW AD_WO_L
#define KC_BTID0 ADV_ID0
#define KC_BTID1 ADV_ID1
#define KC_BTID2 ADV_ID2
#define KC_BTID3 ADV_ID3
#define KC_BTID4 ADV_ID4

#define KC_CAD LCA(KC_DEL)
#define KC_APSCR LALT(KC_PSCR)

#define KC_SNUBS S(KC_NUBS)
#define KC_SNUHS S(KC_NUHS)

#define KC_ESFL TD(TD_ESFL)
#define KC_ESQW TD(TD_ESQW)

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_ESFL] = ACTION_TAP_DANCE_DUAL_ROLE(KC_ESC, _FLOCK),
  [TD_ESQW] = ACTION_TAP_DANCE_DUAL_ROLE(KC_ESC, _QWERTY),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_kc( \
  //,------------------------------------------------|             |------------------------------------------------.
       PGUP,  ESFL,     1,     2,     3,     4,     5,                   6,     7,     8,     9,     0,  MINS,  BSPC, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
       PGDN,   TAB,     Q,     W,     E,     R,     T,                   Y,     U,     I,     O,     P,  LCBR,  RCBR, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             LCTRL,     A,     S,     D,     F,     G,                   H,     J,     K,     L,  SCLN,  QUOT,   ENT, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
              LSFT,     Z,     X,     C,     V,     B,                   N,     M,  COMM,   DOT,  SLSH,    UP,  RSFT, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
            ADJUST,         LALT,          SPC, LOWER,               RAISE,          SPC,  LGUI,  LEFT,  DOWN, RIGHT  \
          //`-------------------------------------------------------------------------------------------------------'
  ),

  [_FLOCK] = LAYOUT_kc( \
  //,------------------------------------------------|             |------------------------------------------------.
      _____,  ESQW,    F1,    F2,    F3,    F4,    F5,                  F6,    F7,    F8,    F9,   F10,   F11,   F12, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
      _____,   TAB,     Q,     W,     E,     R,     T,                   Y,     U,     I,     O,     P,  LCBR,  RCBR, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             LCTRL,     A,     S,     D,     F,     G,                   H,     J,     K,     L,  SCLN,  QUOT,   ENT, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
              LSFT,     Z,     X,     C,     V,     B,                   N,     M,  COMM,   DOT,  SLSH,    UP,  RSFT, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
            ADJUST,         LALT,        _____, LOWER,               RAISE,        _____,  LGUI,  LEFT,  DOWN, RIGHT  \
          //`-------------------------------------------------------------------------------------------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,------------------------------------------------|             |------------------------------------------------.
      _____,   ESC,    F1,    F2,    F3,    F4,    F5,                  F6,    F7,    F8,    F9,   F10,   F11,   F12, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
      _____,  TILD,  EXLM,    AT,  HASH,   DLR,  PERC,                CIRC,  AMPR,  ASTR,  LPRN,  RPRN,   DEL, _____, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____,    F1,    F2,    F3,    F4,    F5,                  F6,  UNDS,  PLUS,  LCBR,  RCBR,  PIPE, _____, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____,    F7,    F8,  F9,     F10,   F11,                 F12, SNUHS, SNUBS, _____, _____,  VOLU, _____, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____,        _____,        _____, LOWER,                MLAD,        _____,  PDOT,  MNXT,  VOLD,  MPLY  \
          //`-------------------------------------------------------------------------------------------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,------------------------------------------------|             |------------------------------------------------.
      _____,   ESC,    F1,    F2,    F3,    F4,    F5,                  F6,    F7,    F8,    F9,   F10,   F11,   F12, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
      _____,   TAB,     1,     2,     3,     4,     5,                   6,     7,     8,     9,     0,  MINS,  BSPC, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____,    F1,    F2,    F3,    F4,    F5,                  F6,  MINS,   EQL,  LBRC,  RCBR,  BSLS, _____, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____,    F7,    F8,  F9,     F10,   F11,                 F12,  NUHS,  NUBS, _____, _____,  VOLU, _____, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____,        _____,        _____,  MLAD,               RAISE,        _____,  LGUI,  MNXT,  VOLD,  MPLY  \
          //`-------------------------------------------------------------------------------------------------------'
  ),


  [_ADJUST] = LAYOUT_kc( /* Base */
  //,------------------------------------------------|             |------------------------------------------------.
      _____,   RST,    F1,    F2,    F3,    F4,    F5,                  F6,    F7,    F8,    F9,   F10,   F11,   F12, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
      _____, _____, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,               XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, \
  //|------+------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,                LTOG,  LMOD, XXXXX,   CAD, APSCR,  PSCR, XXXXX, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,                LVAD,  LVAI,  LHUD,  LHUI,  LSAD,  LSAI, XXXXX, \
  //       |------+------+------+------+------+------|             |------+------+------+------+------+------+------|
             _____,        _____,        _____, _____,               _____,        _____, _____,  MNXT,  VOLD,  MPLY  \
          //`-------------------------------------------------------------------------------------------------------'
  )
};

// define variables for reactive RGB
bool TOG_STATUS = false;
int RGB_current_mode;

static inline void update_change_layer(bool pressed, uint8_t layer1, uint8_t layer2, uint8_t layer3) {

  pressed ? layer_on(layer1) : layer_off(layer1);
  IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2) ? layer_on(layer3) : layer_off(layer3);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

/*
//A description for expressing the layer position in LED mode.
uint32_t layer_state_set_user(uint32_t state) {
  state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
#ifdef RGBLIGHT_ENABLE
    switch (biton32(state)) {
    case _MOUSE:
      setrgb(150, 0, 0, (LED_TYPE *)&led[0]);
      setrgb(0, 0, 0, (LED_TYPE *)&led[1]);
      rgblight_set();
      break;
    case _BROWSER:
      setrgb(0, 0, 0, (LED_TYPE *)&led[0]);
      setrgb(150, 0, 0, (LED_TYPE *)&led[1]);
      rgblight_set();
      break;
    case _FLOCK:
      setrgb(100, 100, 0, (LED_TYPE *)&led[0]);
      setrgb(100, 100, 0, (LED_TYPE *)&led[1]);
      rgblight_set();
      break;
    case _LOWER:
      setrgb(0, 0, 150, (LED_TYPE *)&led[0]);
      setrgb(0, 0, 150, (LED_TYPE *)&led[1]);
      rgblight_set();
      break;
    case _RAISE:
      setrgb(150, 0, 0, (LED_TYPE *)&led[0]);
      setrgb(150, 0, 0, (LED_TYPE *)&led[1]);
      rgblight_set();
      break;
    case _ADJUST:
      setrgb(100, 0, 100, (LED_TYPE *)&led[0]);
      setrgb(100, 0, 100, (LED_TYPE *)&led[1]);
      rgblight_set();
      break;
    default: //  for any other layers, or the default layer
      setrgb(0, 0, 0, (LED_TYPE *)&led[0]);
      setrgb(0, 0, 0, (LED_TYPE *)&led[1]);
      rgblight_set();
      break;
    }
#endif
return state;
}
*/

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  char str[16];

  bool result = true;

  switch (keycode) {
  case LOWER:
    update_change_layer(record->event.pressed, _LOWER, _RAISE, _ADJUST);
    result = false;
    break;
  case RAISE:
    update_change_layer(record->event.pressed, _RAISE, _LOWER, _ADJUST);
    result = false;
    break;
  case KANJI:
    if (record->event.pressed) {
      if (keymap_config.swap_lalt_lgui == false) {
        register_code(KC_LANG2);
      } else {
        SEND_STRING(SS_LALT("`"));
      }
    } else {
      unregister_code(KC_LANG2);
    }
    result = false;
    break;
  #ifdef RGBLIGHT_ENABLE
    case RGB_MOD:
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      result = false;
      break;
    case RGBRST:
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      result = false;
      break;
#endif
  }

  if (record->event.pressed) {
    switch (keycode) {
    case DELBNDS:
      delete_bonds();
      result = false;
      break;
    case AD_WO_L:
      restart_advertising_wo_whitelist();
      result = false;
      break;
    case USB_EN:
      set_usb_enabled(true);
      result = false;
      break;
    case USB_DIS:
      set_usb_enabled(false);
      result = false;
      break;
    case BLE_EN:
      set_ble_enabled(true);
      result = false;
      break;
    case BLE_DIS:
      set_ble_enabled(false);
      result = false;
      break;
    case ADV_ID0:
      restart_advertising_id(0);
      result = false;
      break;
    case ADV_ID1:
      restart_advertising_id(1);
      result = false;
      break;
    case ADV_ID2:
      restart_advertising_id(2);
      result = false;
      break;
    case ADV_ID3:
      restart_advertising_id(3);
      result = false;
      break;
    case ADV_ID4:
      restart_advertising_id(4);
      result = false;
      break;
    case DEL_ID0:
      delete_bond_id(0);
      result = false;
      break;
    case DEL_ID1:
      delete_bond_id(1);
      result = false;
      break;
    case DEL_ID2:
      delete_bond_id(2);
      result = false;
      break;
    case DEL_ID3:
      delete_bond_id(3);
      result = false;
      break;
    case BATT_LV:
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
      result = false;
      break;
    case ENT_DFU:
      bootloader_jump();
      result = false;
      break;
    }
  }
  else if (!record->event.pressed) {
    switch (keycode) {
    case ENT_SLP:
      sleep_mode_enter();
      result = false;
      break;
    }
  }

  return result;
}
