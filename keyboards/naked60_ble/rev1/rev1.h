#pragma once

//#include "../naked60.h"

//void promicro_bootloader_jmp(bool program);

#ifndef KEYBOARDS_NAKED60_BLE_REV1_REV1_H_
#define KEYBOARDS_NAKED60_BLE_REV1_REV1_H_

#include "quantum.h"

/*#ifdef RGBLIGHT_ENABLE
//rgb led driver
#include "ws2812.h"
#endif

#ifdef USE_I2C
#include <stddef.h>
#ifdef __AVR__
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif
#endif*/

//void promicro_bootloader_jmp(bool program);
/*
 * ,------------------------------------           ------------------------------------.
 * | L00 | L01 | L02 | L03 | L04 | L05 |           | L06 | L07 | L08 | L09 | L0A | L0B |
 * |------------------------------------           ------------------------------------+
 * | L10 | L11 | L12 | L13 | L14 | L15 |           | L16 | L17 | L18 | L19 | L1A | L1B |
 * |------------------------------------           ------------------------------------+
 * | L20 | L21 | L22 | L23 | L24 | L25 |           | L26 | L17 | L28 | L29 | L2A | L2B |
 * |------------------------------------           ------------------------------------+
 * | L30 | L31 | L32 | L33 | L34 | L35 |           | L36 | L37 | L38 | L39 | L3A | L3B |
 *       |-----------------------------------------------------------------------'
 *       | L40 | L41 | L42 | L43 | L44 | L45 | L46 | L47 | L48 | L49 | L4A | L4B |
 *       |-----------------------------------------------------------------------'
 */

#define LAYOUT( \
    L00, L01, L02, L03, L04, L05, L06, L07, L08, L09, L0A, L0B, \
    L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, L1A, L1B, \
    L20, L21, L22, L23, L24, L25, L26, L27, L28, L29, L2A, L2B, \
    L30, L31, L32, L33, L34, L35, L36, L37, L38, L39, L3A, L3B, \
    L40, L41, L42, L43, L44, L45, L46, L47, L48, L49, L4A, L4B  \
  ) \
  { \
    {  L00,  L01,  L02,  L03,  L04,  L05,  L06,  L07,  L08,  L09,  L0A,  L0B  },  \
    {  L10,  L11,  L12,  L13,  L14,  L15,  L16,  L17,  L18,  L19,  L1A,  L1B  },  \
    {  L20,  L21,  L22,  L23,  L24,  L25,  L26,  L27,  L28,  L29,  L2A,  L2B  },  \
    {  L30,  L31,  L32,  L33,  L34,  L35,  L36,  L37,  L38,  L39,  L3A,  L3B  },  \
    {  L40,  L41,  L42,  L43,  L44,  L45,  L46,  L47,  L48,  L49,  L4A,  L4B  },  \
  }

// Used to create a keymap using only KC_ prefixed keys
#define LAYOUT_kc( \
    L00, L01, L02, L03, L04, L05, L06, L07, L08, L09, L0A, L0B, \
    L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, L1A, L1B, \
    L20, L21, L22, L23, L24, L25, L26, L27, L28, L29, L2A, L2B, \
    L30, L31, L32, L33, L34, L35, L36, L37, L38, L39, L3A, L3B, \
    L40, L41, L42, L43, L44, L45, L46, L47, L48, L49, L4A, L4B  \
) \
  LAYOUT( \
    KC_##L00, KC_##L01, KC_##L02, KC_##L03, KC_##L04, KC_##L05,                     KC_##L06, KC_##L07, KC_##L08, KC_##L09, KC_##L0A, KC_##L0B, \
    KC_##L10, KC_##L11, KC_##L12, KC_##L13, KC_##L14, KC_##L15,                     KC_##L16, KC_##L17, KC_##L18, KC_##L19, KC_##L1A, KC_##L1B, \
    KC_##L20, KC_##L21, KC_##L22, KC_##L23, KC_##L24, KC_##L25,                     KC_##L26, KC_##L27, KC_##L28, KC_##L29, KC_##L2A, KC_##L2B, \
    KC_##L30, KC_##L31, KC_##L32, KC_##L33, KC_##L34, KC_##L35,                     KC_##L36, KC_##L37, KC_##L38, KC_##L39, KC_##L3A, KC_##L3B, \
              KC_##L40, KC_##L41, KC_##L42, KC_##L43, KC_##L44, KC_##L45, KC_##L46, KC_##L47, KC_##L48, KC_##L49, KC_##L4A, KC_##L4B  \
  )

#endif /* KEYBOARDS_naked60_BLE_REV1_REV1_H_ */
