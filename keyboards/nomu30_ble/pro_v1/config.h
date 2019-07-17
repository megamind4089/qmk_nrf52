#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"

#define THIS_DEVICE_ROWS 3
#define THIS_DEVICE_COLS 12

#undef C6
#undef D0
#undef D1
#undef D2
#undef D3
#undef D4
#undef D7
#undef E6
#undef F4
#undef F5
#undef F6
#undef F7

// B* names are already used in nRF SDK. 
#define _B1 PIN16
#define _B2 PIN14
#define _B3 PIN15
#define _B4 PIN11
#define _B5 PIN12
#define _B6 PIN13

#define C6 PIN8
#define D0 PIN6
#define D1 PIN5
#define D2 PIN2
#define D3 PIN1
#define D4 PIN7
#define D7 PIN9
#define E6 PIN10
#define F4 PIN20
#define F5 PIN19
#define F6 PIN18
#define F7 PIN17

#define MATRIX_ROW_PINS { D1, D0, D4 }
#define MATRIX_COL_PINS { C6, D7, E6, _B4, F4, F5, F6, F7, _B1, _B3, _B2, _B6 }

#define IS_LEFT_HAND  true
#define ENABLE_STARTUP_ADV_NOLIST

#endif /* MASTER_CONFIG_H_ */
