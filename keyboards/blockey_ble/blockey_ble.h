#ifndef BLOCKEY_BLE_H_
#define BLOCKEY_BLE_H_

#include "quantum.h"

// This a shortcut to help you visually see your layout.
// The following is an example using the Planck MIT layout
// The first section contains all of the arguments
// The second converts the arguments into a two-dimensional array
#define LAYOUT( \
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, k13, k14, \
    k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, k25, k26, k27, k28, \
    k29, k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k40, k41, \
    k42, k43, k44, k45, k46, k47, k48, k49, k50, k51, k52, k53, k54, \
    k55, k56, k57, k58, k59, k60, k61, k62 \
) \
{ \
    { k01, k02, k03, k04, k05, k06, k07 }, \
    { k15, k16, k17, k18, k19, k20, k21 }, \
    { k29, k30, k31, k32, k33, k34, k35 }, \
    { k42, k43, k44, k45, k46, k47, k48 }, \
    { k55, k56, k57, k58, KC_NO, KC_NO, k59 }, \
    { k08, k09, k10, k11, k12, k13, k14 }, \
    { k22, k23, k24, k25, k26, k27, k28 }, \
    { k36, k37, k38, k39, k40, k41, KC_NO }, \
    { k49, k50, k51, k52, k53, k54, KC_NO }, \
    { KC_NO, KC_NO, KC_NO, k60, k61, k62, KC_NO}, \
}

#endif /* BLOCKEY_BLE_H_ */
