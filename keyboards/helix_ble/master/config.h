#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"

#if HELIX_ROWS == 4
  #define THIS_DEVICE_ROWS HELIX_ROWS
  #define MATRIX_ROW_PINS { PIN7, PIN8, PIN9, PIN10 }
#elif HELIX_ROWS == 5
  #define THIS_DEVICE_ROWS HELIX_ROWS
  #define MATRIX_ROW_PINS { PIN7, PIN8, PIN9, PIN10, PIN11 }
#endif

#define THIS_DEVICE_COLS 7
#define MATRIX_COL_PINS { PIN20, PIN19, PIN18, PIN17, PIN16, PIN15, PIN14 }

#define IS_LEFT_HAND  true

// Helix keyboard OLED support
//      see ./rules.mk: OLED_ENABLE=yes or no
#ifdef OLED_ENABLE
  #define SSD1306OLED
#endif

#endif /* MASTER_CONFIG_H_ */
