/*
 * pin_assign.h
 *
 *  Created on: 2018/11/23
 *      Author: Sekigon
 */

#ifndef PIN_ASSIGN_H_
#define PIN_ASSIGN_H_

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

#define THIS_DEVICE_ROWS 5
//#define MATRIX_ROW_PINS { PIN7, PIN8, PIN9, PIN10, PIN11 }
#define MATRIX_ROW_PINS { D4, C6, D7, E6, B4 }

#define THIS_DEVICE_COLS 7
//#define MATRIX_COL_PINS { PIN20, PIN19, PIN18, PIN17, PIN16, PIN15, PIN14 }
#define MATRIX_COL_PINS { F4, F5, F6, F7, B1, B3, B2 }

#endif /* PIN_ASSIGN_H_ */
