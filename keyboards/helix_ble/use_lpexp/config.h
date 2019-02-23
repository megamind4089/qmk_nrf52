#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"
#define IS_LEFT_HAND  true

#define SLAVE_I2C_ADDRESS           (0x32>>1)
#define USE_I2C
#define USE_LP_MAT_EXPANDER
#define CONFIG_I2C_FREQ NRF_TWI_FREQ_250K
#define ENABLE_STARTUP_ADV_NOLIST

#define LP_MAT_CONFIG {.row_num = 5, .col_num = 7, .row_pin_start = 1, .col_pin_start = 8}
#endif /* MASTER_CONFIG_H_ */
