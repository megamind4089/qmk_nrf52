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
    BACKLIT,
    EISU,
    KANA,
    RGBRST
};


extern keymap_config_t keymap_config;

enum layer_number {
    _QWERTY = 0,
    _COLEMAK,
    _DVORAK,
    _LOWER,
    _RAISE,
    _ADJUST
};

// Fillers to make layering more clear
#define RSFT_QT MT(MOD_RSFT,KC_QUOT)  // タップで「'」 ホールドで右Shift

#define LOW_ES  LT(_LOWER, KC_LANG2)  // タップで英数 ホールドでLower
#define RIS_KN  LT(_RAISE, KC_LANG1)  // タップでかな ホールドでRaise
#define LOWER  MO(_LOWER)           // ホールドでAdjustレイヤーをon
#define RAISE  MO(_RAISE)           // ホールドでAdjustレイヤーをon
#define ADJUST  MO(_ADJUST)           // ホールドでAdjustレイヤーをon
#define MY_VOLD LALT(LSFT(KC_VOLD))   // 細かいボリュームダウン
#define MY_VOLU LALT(LSFT(KC_VOLU))   // 細かいボリュームアップ

#define TGL_LOW TG(_LOWER)            // Lowerトグル
#define TGL_RIS TG(_RAISE)            // Raiseトグル
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Ctrl |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  | Enter|
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |'/Sft |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  | Adjust| Alt | GUI  | Lower|Space |Space | Raise| Left | Down |  Up  |Right|
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT( \
  KC_TAB,  KC_Q,    KC_W,    KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,     KC_I,    KC_O,    KC_P,    KC_BSPC, \
  KC_LCTRL,KC_A,    KC_S,    KC_D,     KC_F,    KC_G,    KC_H,    KC_J,     KC_K,    KC_L,    KC_SCLN, KC_ENT , \
  KC_LSFT, KC_Z,    KC_X,    KC_C,     KC_V,    KC_B,    KC_N,    KC_M,     KC_COMM, KC_DOT,  KC_SLSH, RSFT_QT, \
  KC_ESC,  ADJUST,  KC_LALT, KC_LGUI,  LOWER,  KC_SPC,  KC_SPC,   RAISE,    KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT  \
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   4  |   5  |   6  |   -  |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |   1  |   2  |   3  |   +  |   *  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |   0  |   ,  |   .  |   /  |   =  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT( \
  KC_ESC , KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,    KC_DEL,  \
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_4,    KC_5,    KC_6,   KC_PMNS, _______, \
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_1,    KC_2,    KC_3,   KC_PPLS, KC_PAST, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_COMM, KC_DOT, KC_PSLS, KC_PEQL  \
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |      |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      | Vol- | Vol+ | Mute | MBTN1|   \  |   `  |   -  |   =  |   [  |   ]  | PGup |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      | Prev | Next | Play | MBTN2|   |  |   ~  |   _  |   +  |   {  |   }  | PGdn |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |  MR  |  MD  |  MU  |  MR  |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT( \
  _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,  \
  _______, MY_VOLD, MY_VOLU, KC_MUTE, KC_MS_BTN1, KC_BSLS, KC_GRV,  KC_MINS, KC_EQL,  KC_LBRC ,KC_RBRC, KC_PGUP, \
  _______, KC_MRWD, KC_MFFD, KC_MPLY, KC_MS_BTN2, KC_PIPE, KC_TILD, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PGDOWN, \
  _______, _______, _______, _______, XXXXXXX, _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R \
),

/* Adjust
 * ,-----------------------------------------------------------------------------------.
 * |      |Reset |      |      |      |TglLow|TglRis|      |      |      |      |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |Power |      |      |Aud on|Audoff|      |      | DFLT |      |      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|      |      |BLtogg| BL - | BL + |BLstep|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | ADWOL|BLEdis|BLEen |      |      |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] =  LAYOUT( \
  XXXXXXX, RESET,   XXXXXXX, XXXXXXX, TGL_LOW, TGL_RIS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  KC_POWER,XXXXXXX, MU_MOD,  AU_ON,   AU_OFF,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  XXXXXXX, XXXXXXX, BL_TOGG, BL_DEC,  BL_INC,  BL_STEP, XXXXXXX, \
  _______, _______, AD_WO_L, BLE_DIS, BLE_EN, _______, _______, _______, _______, _______, _______, _______  \
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
  switch (keycode) {
//    case QWERTY:
//      if (record->event.pressed) {
//        #ifdef AUDIO_ENABLE
//          PLAY_SONG(tone_qwerty);
//        #endif
//        persistent_default_layer_set(1UL<<_QWERTY);
//      }
//      return false;
//      break;
//    case COLEMAK:
//      if (record->event.pressed) {
//        #ifdef AUDIO_ENABLE
//          PLAY_SONG(tone_colemak);
//        #endif
//        persistent_default_layer_set(1UL<<_COLEMAK);
//      }
//      return false;
//      break;
//    case DVORAK:
//      if (record->event.pressed) {
//        #ifdef AUDIO_ENABLE
//          PLAY_SONG(tone_dvorak);
//        #endif
//        persistent_default_layer_set(1UL<<_DVORAK);
//      }
//      return false;
//      break;
//    case LOWER:
//      if (record->event.pressed) {
//          //not sure how to have keyboard check mode and set it to a variable, so my work around
//          //uses another variable that would be set to true after the first time a reactive key is pressed.
//        if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
//        } else {
//          TOG_STATUS = !TOG_STATUS;
//          #ifdef RGBLIGHT_ENABLE
//            //rgblight_mode(16);
//          #endif
//        }
//        layer_on(_LOWER);
//        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
//      } else {
//        #ifdef RGBLIGHT_ENABLE
//          //rgblight_mode(RGB_current_mode);   // revert RGB to initial mode prior to RGB mode change
//        #endif
//        TOG_STATUS = false;
//        layer_off(_LOWER);
//        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
//      }
//      return false;
//      break;
//    case RAISE:
//      if (record->event.pressed) {
//        //not sure how to have keyboard check mode and set it to a variable, so my work around
//        //uses another variable that would be set to true after the first time a reactive key is pressed.
//        if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
//        } else {
//          TOG_STATUS = !TOG_STATUS;
//          #ifdef RGBLIGHT_ENABLE
//            //rgblight_mode(15);
//          #endif
//        }
//        layer_on(_RAISE);
//        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
//      } else {
//        #ifdef RGBLIGHT_ENABLE
//          //rgblight_mode(RGB_current_mode);  // revert RGB to initial mode prior to RGB mode change
//        #endif
//        layer_off(_RAISE);
//        TOG_STATUS = false;
//        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
//      }
//      return false;
//      break;
//    case ADJUST:
//        if (record->event.pressed) {
//          layer_on(_ADJUST);
//        } else {
//          layer_off(_ADJUST);
//        }
//        return false;
//        break;
      //led operations - RGB mode change now updates the RGB_current_mode to allow the right RGB mode to be set after reactive keys are released
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
      break;
    case EISU:
      if (record->event.pressed) {
        if(keymap_config.swap_lalt_lgui==false){
          register_code(KC_LANG2);
        }else{
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG2);
      }
      return false;
      break;
    case KANA:
      if (record->event.pressed) {
        if(keymap_config.swap_lalt_lgui==false){
          register_code(KC_LANG1);
        }else{
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG1);
      }
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
  }
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
