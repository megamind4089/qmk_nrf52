#pragma once

#include "custom_board.h"

#undef B0
#undef B1
#undef B2
#undef B3
#undef B4
#undef B5
#undef B6
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

#define D3 PIN1
#define D2 PIN2
#define D1 PIN5
#define D0 SIDE_PIN_7
#define D4 PIN7
#define C6 PIN8
#define D7 PIN9
#define E6 PIN10
#define B4 PIN11
#define B5 PIN12
#define B6 PIN13
#define B2 PIN14
#define B3 PIN15
#define B1 PIN16
#define F7 PIN17
#define F6 PIN18
#define F5 PIN19
#define F4 PIN20


#define T1 TOP_PIN_1
#define T2 TOP_PIN_5
#define T3 TOP_PIN_6
#define S1 SIDE_PIN_4
#define S2 SIDE_PIN_5
#define S3 SIDE_PIN_6
#define S4 SIDE_PIN_7

#define THIS_DEVICE_ROWS 4
#define THIS_DEVICE_COLS 13

#define MATRIX_ROW_PINS { B2, B6, B4, B5 }
#define MATRIX_COL_PINS { D2, D1, D0, C6, D7, E6, T2, B3, B1, F7, F6, F5, F4 }

