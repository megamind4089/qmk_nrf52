/*
 * master.c
 *
 *  Created on: 2019/03/18
 *      Author: sekigon
 */

#include "matrix.h"
#include "quantum.h"
#include "nrf.h"
#include "app_ble_func.h"

void matrix_init_user() {
  set_usb_enabled(true);
}
