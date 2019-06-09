#ifndef KEYBOARDS_BOOTLEG31_H_
#define KEYBOARDS_BOOTLEG31_H_

#include "quantum.h"

#define LAYOUT( \
       K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, \
  K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, \
  K20, K21, K22, K23, K24, K25, K26, K27, K28            \
) \
{ \
  { K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A,   K0B  },  \
  { K10, K11, K12, K13, K14, K15, K16, K17, K18, K19,   K1A,  }, \
  { K20, K21, K22, K23, K24, K25, K26, K27, K28, KC_NO, KC_NO, },\
}

#define LAYOUT_kc( \
       K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, \
  K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A,      \
  K20, K21, K22, K23, K24, K25, K26, K27, K28                 \
) \
LAYOUT( \
           KC_##01, KC_##02, KC_##03, KC_##04, KC_##05, KC_##06, KC_##07, KC_##08, KC_##09, KC_##0A, KC_##0B, \
  KC_##10, KC_##11, KC_##12, KC_##13, KC_##14, KC_##15, KC_##16, KC_##17, KC_##18, KC_##19, KC_##1A,          \
  KC_##20, KC_##21, KC_##22, KC_##23, KC_##24, KC_##25, KC_##26, KC_##27, KC_##28,                            \
)

#endif /* KEYBOARDS_BOOTLEG31_H_ */
