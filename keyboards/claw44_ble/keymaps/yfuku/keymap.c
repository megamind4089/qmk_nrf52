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
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _BLE,
};

// common
#define KC_ KC_TRNS
#define KC_XXXX KC_NO
#define KC_RST RESET
#define KC_VD KC__VOLDOWN
#define KC_VU KC__VOLUP

// layer
#define KC_L_SPC LT(_LOWER, KC_SPC)
#define KC_R_ENT LT(_RAISE, KC_ENT)

// shift_t
#define KC_S_TAB LSFT_T(KC_TAB)
#define KC_S_ESC LSFT_T(KC_ESC)
#define KC_S_JA LSFT_T(KC_LANG1)
#define KC_S_EN LSFT_T(KC_LANG2)

// cmd_t
#define KC_M_F LCMD_T(KC_F)
#define KC_M_D LCMD_T(KC_D)
#define KC_M_J RCMD_T(KC_J)
#define KC_M_K RCMD_T(KC_K)

// ctl_t
#define KC_C_G LCTL_T(KC_G)
#define KC_C_S LCTL_T(KC_S)
#define KC_C_L RCTL_T(KC_L)
#define KC_C_H RCTL_T(KC_H)
#define KC_C_BS LCTL_T(KC_BSPC)

// alt_t
#define KC_A_D ALT_T(KC_D)
#define KC_A_K ALT_T(KC_K)
#define KC_A_Z ALT_T(KC_Z)
#define KC_A_SL ALT_T(KC_SLSH)
#define KC_A_DEL ALT_T(KC_DEL)

// cmd+shift_t
#define KC_MS_Q SCMD_T(KC_Q)
#define KC_MS_A SCMD_T(KC_A)
#define KC_MS_S SCMD_T(KC_S)
#define KC_MS_SC SCMD_T(KC_SCLN)
#define KC_MS_ESC SCMD_T(KC_ESC)

// ble
#define KC_BLE MO(_BLE)
#define KC_AD_WO_L AD_WO_L 

//
#define KC_MR RCMD(KC_R)
#define KC_MF RCMD(KC_F)
#define KC_MW RCMD(KC_W)
#define KC_MX RCMD(KC_X)
#define KC_MC RCMD(KC_C)
#define KC_MV RCMD(KC_V)
#define KC_MTAB RCMD(KC_TAB)
#define KC_MSF RCMD(RSFT(KC_F))
#define KC_MSR RCMD(RSFT(KC_R))
#define KC_MST RCMD(RSFT(KC_T))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT_kc( \
  //,----+----+----+----+----+----.     ,----+----+----+----+----+----.
         , Q  , W  , E  , R  , T  ,       Y  , U  , I  , O  , P  ,MINS,
  //|----+----+----+----+----+----|     |----+----+----+----+----+----|
      TAB, A  , S  , D  ,M_F ,C_G ,      C_H ,M_J , K  , L  ,SCLN,ESC ,
  //|----+----+----+----+----+----+     |----+----+----+----+----+----|
      BLE, Z  , X  , C  , V  , B  ,       N  , M  ,COMM,DOT ,SLSH,    ,
  //`----+----+----+----+----+----/     \----+----+----+----+----+----'
             A_DEL,S_EN,L_SPC,C_BS,      C_BS,R_ENT,S_JA,A_DEL
  //          `----+----+----+----'     `----+----+----+----'
  ),

  //   \ ! & |      = + * ^
  //   # ` " ' ~  ← ↓ ↑ → $
  //       { [ (  ) ] } @

  [_RAISE] = LAYOUT_kc( \
  //,----+----+----+----+----+----.     ,----+----+----+----+----+----.
         ,BSLS,EXLM,AMPR,PIPE,XXXX,      XXXX,EQL ,PLUS,ASTR,CIRC,    ,
  //|----+----+----+----+----+----|     |----+----+----+----+----+----|
         ,HASH,GRV ,DQT ,QUOT,TILD,      LEFT,DOWN, UP ,RGHT,DLR ,    ,
  //|----+----+----+----+----+----|     |----+----+----+----+----+----|
         ,    ,    ,LCBR,LBRC,LPRN,      RPRN,RBRC,RCBR,AT  ,PERC,    ,
  //`----+----+----+----+----+----/     \----+----+----+----+----+----'
                   ,    ,BSPC,    ,          ,    ,    ,RST
  //          `----+----+----+----'     `----+----+----+----'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,----+----+----+----+----+----.     ,----+----+----+----+----+----.
         ,    ,    ,MSF ,MSR ,MST ,          ,EQL ,PLUS,ASTR,    ,MINS,
  //|----+----+----+----+----+----|     |----+----+----+----+----+----|
         , 1  , 2  , 3  , 4  , 5  ,       6  , 7  , 8  , 9  , 0  ,    ,
  //|----+----+----+----+----+----|     |----+----+----+----+----+----|
         ,    ,    ,    ,    ,    ,          ,PERC,COMM,DOT ,SLSH,    ,
  //`----+----+----+--+-+----+----/     \----+----+----+----+----+----'
               RST ,    ,    ,    ,          ,DEL ,    ,
  //          `----+----+----+----'     `----+----+----+----'
  ),

  [_BLE] = LAYOUT( \
  //,--------+--------+--------+--------+--------+--------.   ,--------+--------+--------+--------+--------+--------.
     AD_WO_L ,ADV_ID1 ,ADV_ID2 ,ADV_ID3 ,ADV_ID4 ,XXXXXXX ,     _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
     ENT_SLP ,ENT_DFU ,KC_B    ,XXXXXXX ,USB_DIS ,USB_EN  ,     _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
      _______,BATT_LV ,DEL_ID1 ,XXXXXXX ,BLE_DIS ,BLE_EN  ,     _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
                        _______, _______, _______, _______,     _______, _______, _______, _______
  //                  `--------+--------+--------+--------'   `--------+--------+--------+--------'
  )
};

size_t sizeof_keymaps() {
  return sizeof(keymaps);
}

// define variables for reactive RGB
bool TOG_STATUS = false;
int RGB_current_mode;
#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
  layer_state_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    #ifdef RGBLIGHT_ENABLE
      //rgblight_mode(RGB_current_mode);
    #endif
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}
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
