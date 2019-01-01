/*
 * pro_v1.c
 *
 *  Created on: 2018/11/02
 *      Author: Sekigon
 */



#include "matrix.h"
#include "quantum.h"
#include "nrf.h"
#include "app_ble_func.h"

volatile int hoge;
void matrix_init_user() {
  set_usb_enabled(true);
//  for (int i=0; i<2; i++) {
//    hoge = 0; // dummy to avoid booloader bugs
//  }
  debug_enable = true;
}
