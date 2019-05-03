#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"

#define THIS_DEVICE_ROWS 1
#define THIS_DEVICE_COLS 19

#define MATRIX_ROW_PINS { GPIO(1,1) } // Dummy
#define MATRIX_COL_PINS { PIN1, PIN2, PIN5, PIN6, PIN7, \
		                      PIN8, PIN9, PIN10, PIN11, PIN12, \
		                      PIN13, PIN14, PIN15, PIN16, PIN17, \
		                      PIN18, PIN19, PIN20, BOOTPIN }
#define IS_LEFT_HAND  true
#define ENABLE_STARTUP_ADV_NOLIST

#endif /* MASTER_CONFIG_H_ */
