#ifndef KEYBOARDS_MODULO_REV2_H_
#define KEYBOARDS_MODULO_REV2_H_

#include "quantum.h"

#define LAYOUT( \
    L00, L01, L02, L03, L04, L05, R00, R01, R02, R03, R04, R05,    \
    L10, L11, L12, L13, L14, L15, R10, R11, R12, R13, R14, R15,    \
    L20, L21, L22, L23, L24, L25, R20, R21, R22, R23, R24, R25,    \
    L30, L31, L32, L33, L34, L35, R30, R31, R32, R33, R34, R35,    \
    L40, L41, L42, L43, L44, L45, R40, R41, R42, R43, R44, R45    \
) \
{ \
{L00, L01, L02, L03, L04, L05}, \
{L10, L11, L12, L13, L14, L15}, \
{L20, L21, L22, L23, L24, L25}, \
{L30, L31, L32, L33, L34, L35}, \
{L40, L41, L42, L43, L44, L45}, \
{KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
{R00, R01, R02, R03, R04, R05}, \
{R10, R11, R12, R13, R14, R15}, \
{R20, R21, R22, R23, R24, R25}, \
{R30, R31, R32, R33, R34, R35}, \
{R40, R41, R42, R43, R44, R45}, \
{KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO } \
}

#define LAYOUT_kc( \
    L00, L01, L02, L03, L04, L05, R00, R01, R02, R03, R04, R05,    \
    L10, L11, L12, L13, L14, L15, R10, R11, R12, R13, R14, R15,    \
    L20, L21, L22, L23, L24, L25, R20, R21, R22, R23, R24, R25,    \
    L30, L31, L32, L33, L34, L35, R30, R31, R32, R33, R34, R35,    \
    L40, L41, L42, L43, L44, L45, R40, R41, R42, R43, R44, R45    \
) \
{ \
{KC ## L00, KC ## L01, KC ## L02, KC ## L03, KC ## L04, KC ## L05}, \
{KC ## L10, KC ## L11, KC ## L12, KC ## L13, KC ## L14, KC ## L15}, \
{KC ## L20, KC ## L21, KC ## L22, KC ## L23, KC ## L24, KC ## L25}, \
{KC ## L30, KC ## L31, KC ## L32, KC ## L33, KC ## L34, KC ## L35}, \
{KC ## L40, KC ## L41, KC ## L42, KC ## L43, KC ## L44, KC ## L45}, \
{KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
{KC ## R00, KC ## R01, KC ## R02, KC ## R03, KC ## R04, KC ## R05}, \
{KC ## R10, KC ## R11, KC ## R12, KC ## R13, KC ## R14, KC ## R15}, \
{KC ## R20, KC ## R21, KC ## R22, KC ## R23, KC ## R24, KC ## R25}, \
{KC ## R30, KC ## R31, KC ## R32, KC ## R33, KC ## R34, KC ## R35}, \
{KC ## R40, KC ## R41, KC ## R42, KC ## R43, KC ## R44, KC ## R45}, \
{KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO } \
}

#endif /* KEYBOARDS_MODULO_REV2_H_ */
