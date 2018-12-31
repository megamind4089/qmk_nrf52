#ifndef MODULO_BLE_H_
#define MODULO_BLE_H_

#include "quantum.h"

#ifdef ERGO42_MODULO_ALPHA
#define LAYOUT(   \
        L00, L01, L02, L03, L04, L05, L06, R00, R01, R02, R03, R04, R05, R06, \
        L10, L11, L12, L13, L14, L15, L16, R10, R11, R12, R13, R14, R15, R16, \
        L20, L21, L22, L23, L24, L25, L26, R20, R21, R22, R23, R24, R25, R26, \
        L30, L31, L32, L33, L34, L35, L36, R30, R31, R32, R33, R34, R35, R36 \
        ) \
        { \
                { L30, L20, L10, L00, L31, L21, L11, L01, L02, L12, L22, L32, L03, L13, L23, L33 }, \
                { L34, L24, L14, L04, L35, L25, L15, L05, L06, L16, L26, L36, KC_NO, KC_NO, KC_NO, KC_NO }, \
                { R30, R20, R10, R00, R31, R21, R11, R01, R02, R12, R22, R32, R03, R13, R23, R33 }, \
                { R34, R24, R14, R04, R35, R25, R15, R05, R06, R16, R26, R36, KC_NO, KC_NO, KC_NO, KC_NO } \
        }
#endif

#define LAYOUT(   \
        L00, L01, L02, L03, L04, L05, L06, R00, R01, R02, R03, R04, R05, R06, \
        L10, L11, L12, L13, L14, L15, L16, R10, R11, R12, R13, R14, R15, R16, \
        L20, L21, L22, L23, L24, L25, L26, R20, R21, R22, R23, R24, R25, R26, \
        L30, L31, L32, L33, L34, L35, L36, R30, R31, R32, R33, R34, R35, R36 \
        ) \
        { \
                { L00, L01, L02, L03, L04, L05, L06, KC_NO, L16, L15, L14, L13, L12, L11, L10, KC_NO }, \
                { L20, L21, L22, L23, L24, L25, L26, KC_NO, L36, L35, L34, L33, L32, L31, L30, KC_NO }, \
                { R00, R01, R02, R03, R04, R05, R06, KC_NO, R16, R15, R14, R13, R12, R11, R10, KC_NO }, \
                { R20, R21, R22, R23, R24, R25, R26, KC_NO, R36, R35, R34, R33, R32, R31, R30, KC_NO } \
}

#endif /* MODULO_BLE_H_ */
