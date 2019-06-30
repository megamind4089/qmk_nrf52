#pragma once

//#include "../naked64.h"

//void promicro_bootloader_jmp(bool program);

#ifndef KEYBOARDS_NAKED64_BLE_REV1_REV1_H_
#define KEYBOARDS_NAKED64_BLE_REV1_REV1_H_

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
 * ,------------------------------------------       -----------------------------------------.
 * | L00 | L20 | L70 | L31 | L02 | L52 | L13 |       | L63 | L34 | L05 | L45 | L16 | L66 | L37 |
 * |------------------------------------------       ------------------------------------------+
 * | L10 | L30 | L01 | L41 | L12 | L62 | L23 |       | L73 | L44 | L15 | L55 | L26 | L76 | L47 |
 * |------------------------------------------       ------------------------------------------+
 *       | L40 | L11 | L51 | L22 | L72 | L33 |       | L04 | L54 | L25 | L65 | L36 | L07 | L57 |
 *       |------------------------------------       ------------------------------------------+
 *       | L50 | L21 | L61 | L32 | L03 | L43 |       | L14 | L64 | L35 | L75 | L46 | L17 | L67 |
 *       |------------------------------------       ------------------------------------------+
 *       | L60 |     | L71 | L42 |     | L53 |       | L24 | L74 |     | L06 | L56 | L27 | L77 |
 *       |------------------------------------       ------------------------------------------'
 */

#define LAYOUT( \
       L00, L20, L70, L31, L02, L52, L13,    L63, L34, L05, L45, L16, L66, L37, \
       L10, L30, L01, L41, L12, L62, L23,    L73, L44, L15, L55, L26, L76, L47, \
            L40, L11, L51, L22, L72, L33,    L04, L54, L25, L65, L36, L07, L57, \
            L50, L21, L61, L32, L03, L43,    L14, L64, L35, L75, L46, L17, L67, \
            L60,      L71, L42,      L53,    L24, L74,      L06, L56, L27, L77 \
  ) \
  { \
    {  L00,  L01,  L02,  L03,  L04,  L05,  L06,  L07  },  \
    {  L10,  L11,  L12,  L13,  L14,  L15,  L16,  L17  },  \
    {  L20,  L21,  L22,  L23,  L24,  L25,  L26,  L27  },  \
    {  L30,  L31,  L32,  L33,  L34,  L35,  L36,  L37  },  \
    {  L40,  L41,  L42,  L43,  L44,  L45,  L46,  L47  },  \
    {  L50,  L51,  L52,  L53,  L54,  L55,  L56,  L57  },  \
    {  L60,  L61,  L62,  L63,  L64,  L65,  L66,  L67  },  \
    {  L70,  L71,  L72,  L73,  L74,  L75,  L76,  L77  }  \
  }

// Used to create a keymap using only KC_ prefixed keys
#define LAYOUT_kc( \
    L00, L20, L70, L31, L02, L52, L13,    L63, L34, L05, L45, L16, L66, L37, \
    L10, L30, L01, L41, L12, L62, L23,    L73, L44, L15, L55, L26, L76, L47, \
         L40, L11, L51, L22, L72, L33,    L04, L54, L25, L65, L36, L07, L57, \
         L50, L21, L61, L32, L03, L43,    L14, L64, L35, L75, L46, L17, L67, \
         L60,      L71, L42,      L53,    L24, L74,      L06, L56, L27, L77 \
) \
  LAYOUT( \
    KC_##L00, KC_##L20, KC_##L70, KC_##L31, KC_##L02, KC_##L52, KC_##L13,    KC_##L63, KC_##L34, KC_##L05, KC_##L45, KC_##L16, KC_##L66, KC_##L37, \
    KC_##L10, KC_##L30, KC_##L01, KC_##L41, KC_##L12, KC_##L62, KC_##L23,    KC_##L73, KC_##L44, KC_##L15, KC_##L55, KC_##L26, KC_##L76, KC_##L47, \
              KC_##L40, KC_##L11, KC_##L51, KC_##L22, KC_##L72, KC_##L33,    KC_##L04, KC_##L54, KC_##L25, KC_##L65, KC_##L36, KC_##L07, KC_##L57, \
              KC_##L50, KC_##L21, KC_##L61, KC_##L32, KC_##L03, KC_##L43,    KC_##L14, KC_##L64, KC_##L35, KC_##L75, KC_##L46, KC_##L17, KC_##L67, \
              KC_##L60,           KC_##L71, KC_##L42,           KC_##L53,    KC_##L24, KC_##L74,           KC_##L06, KC_##L56, KC_##L27, KC_##L77  \
  )

#endif /* KEYBOARDS_naked64_BLE_REV1_REV1_H_ */
