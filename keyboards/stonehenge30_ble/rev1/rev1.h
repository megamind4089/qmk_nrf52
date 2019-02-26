/*
 * rev1.h
 *
 *  Created on: 2019/02/25
 *      Author: Sekigon
 */

#ifndef KEYBOARDS_stonehenge30_BLE_REV1_REV1_H_
#define KEYBOARDS_stonehenge30_BLE_REV1_REV1_H_

#include "quantum.h"

#define K_N KC_NO

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
  L00, L01, L02, L03, L04, L05, L06, L07, L08, L09, L0A, \
  L10, L11, L12, L13, L14,      L16, L17, L18, L19, L1A, \
  L20, L21, L22, L23, L24, L25, L26, L27, L28 \
  ) \
  { \
    { L00, L01, L02, L03, L04, L05 }, \
    { L10, L11, L12, L13, L14, L25 }, \
    { L20, L21, L22, L23, L24, K_N }, \
    { L06, L07, L08, L09, L0A, K_N }, \
    { L16, L17, L18, L19, L1A, K_N }, \
    { L26, L27, L28, K_N, K_N, K_N }, \
  }

// Used to create a keymap using only KC_ prefixed keys
#define LAYOUT_kc( \
  L00, L01, L02, L03, L04, L05, L06, L07, L08, L09, L0A, \
  L10, L11, L12, L13, L14,      L16, L17, L18, L19, L1A, \
  L20, L21, L22, L23, L24, L25, L26, L27, L28 \
  ) \
  LAYOUT( \
  KC_##L00, KC_##L01, KC_##L02, KC_##L03, KC_##L04, KC_##L05, KC_##L06, KC_##L07, KC_##L08, KC_##L09, KC_##L0A, \
  KC_##L10, KC_##L11, KC_##L12, KC_##L13, KC_##L14,           KC_##L16, KC_##L17, KC_##L18, KC_##L19, KC_##L1A, \
  KC_##L20, KC_##L21, KC_##L22, KC_##L23, KC_##L24, KC_##L25, KC_##L26, KC_##L27, KC_##L28 \
  )

#endif /* KEYBOARDS_stonehenge30_BLE_REV1_REV1_H_ */
