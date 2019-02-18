#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"

#define THIS_DEVICE_ROWS 10
#define THIS_DEVICE_COLS 7

#define MATRIX_ROW_PINS { PIN1, PIN5, PIN7, PIN10, PIN12, PIN2, PIN18, PIN15, PIN14, PIN13 }
#define MATRIX_COL_PINS { PIN6, PIN11, PIN8, PIN9, PIN20, PIN19, PIN17 }
#define IS_LEFT_HAND  true

#define RGB_DI_PIN PIN16 // The pin the LED strip is connected to

#define RGBLED_NUM 4     // Number of LEDs in your strip
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17

#define RGBLIGHT_ANIMATIONS

#endif /* MASTER_CONFIG_H_ */
