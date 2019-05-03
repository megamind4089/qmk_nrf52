#pragma once

#include "../board/custom_board.h"

#define THIS_DEVICE_ROWS 1
#define THIS_DEVICE_COLS 25

#define MATRIX_ROW_PINS { 22 }
#define MATRIX_COL_PINS { 25, 28, 31, 15, 2, 6, 12, \
		                      26, 29, 19, 13, 3, 7, 14, \
		                      27, 30, 17, 0,  4, 8, 20, \
		                      1,  5,  24, 11 }
#define IS_LEFT_HAND  true
#define USE_AS_I2C_SLAVE
