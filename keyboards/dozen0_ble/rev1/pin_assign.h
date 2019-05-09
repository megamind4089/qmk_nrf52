/*
 * pin_assign.h
 */

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
#define D0 PIN6
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

#define THIS_DEVICE_ROWS MATRIX_ROWS
#define THIS_DEVICE_COLS MATRIX_COLS

#define MATRIX_ROW_PINS { F4 }
#define MATRIX_COL_PINS { B6, B2, B3, B1, F7, F6, B5, B4, E6, D7, C6, D4 }
