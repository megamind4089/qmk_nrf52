/*
 * io_expander.c
 *
 *  Created on: 2018/12/16
 *      Author: sekigon-gonnoc
 */

#ifdef USE_I2C_IOEXPANDER
#include <stdint.h>
#include "i2c.h"
#include "io_expander.h"
#include "matrix.h"

extern const IOEXPANDER_TYPEDEF ioexpander_types[];
extern const uint8_t ioexpander_addrs[];

matrix_row_t read_row_ioexpander (uint8_t row) {
  matrix_row_t  matrix_row_state = 0;
  uint8_t dat[2] = {0xFF, 0xFF};
  if (row > MATRIX_ROWS) {
    return 0;
  }
  switch (ioexpander_types[row]) {
  case TCA9555:
    i2c_init();
    i2c_readReg(ioexpander_addrs[row], 0, dat, 2, 0);
    matrix_row_state = ~((((uint16_t)dat[1]) << 8) | dat[0]);
    i2c_uninit();
    break;
  case PCA9654:
    i2c_init();
    i2c_receive(ioexpander_addrs[row], dat, 1);
    matrix_row_state = ~dat[0];
    i2c_uninit();
    break;
  default:
    break;
  }
  return matrix_row_state;
}
#endif
