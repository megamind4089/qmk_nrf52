/*
Copyright 2018 Sekigon

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"
#include "quantum.h"

#include "progmem.h"

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "app_ble_func.h"

#include <stdbool.h>
const uint32_t row_pins[THIS_DEVICE_ROWS] = MATRIX_ROW_PINS;
const uint32_t col_pins[THIS_DEVICE_COLS] = MATRIX_COL_PINS;
const bool isLeftHand = IS_LEFT_HAND;

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

#define QUEUE_LEN 32
struct {
  ble_switch_state_t received_keys[QUEUE_LEN];
  uint8_t ridx, widx, cnt;
} rcv_keys_queue;


static void init_rows(void);
static void init_cols(void);
void unselect_rows(void);
void select_row(uint8_t row);
matrix_row_t read_cols(void);

__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

#define LED_ON()    do { } while (0)
#define LED_OFF()   do { } while (0)
#define LED_TGL()   do { } while (0)

void matrix_init(void)
{
    // initialize row and col
    init_rows();
    unselect_rows();
    init_cols();
    NRF_LOG_INFO("matrix init\r\n")

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    matrix_init_user();
}

static inline void set_received_key(ble_switch_state_t key) {
#if defined(NRF_SEPARATE_KEYBOARD_MASTER) || defined(NRF_SEPARATE_KEYBOARD_SLAVE)
  const uint8_t matrix_offset = isLeftHand ? THIS_DEVICE_ROWS : 0;
  if (key.state) {
    matrix[key.row + matrix_offset] |= (1 << key.col);
  } else {
    matrix[key.row + matrix_offset] &= ~(1 << key.col);
  }
#endif
}


char str[16];
uint8_t matrix_scan(void)
{

  uint8_t matrix_offset = isLeftHand ? 0 : MATRIX_ROWS-THIS_DEVICE_ROWS;
  volatile int matrix_changed = 0;
  static uint8_t timing;
  ble_switch_state_t ble_switch_send[THIS_DEVICE_ROWS*THIS_DEVICE_COLS];

  for (uint8_t i = 0; i < THIS_DEVICE_ROWS; i++) {
    select_row(i);
    wait_us(0);
    matrix_row_t cols = read_cols();
    if (matrix_debouncing[i + matrix_offset] != cols) {
      matrix_debouncing[i + matrix_offset] = cols;
      debouncing = DEBOUNCE;
    }
    unselect_rows();
  }

  if (debouncing) {
    if (--debouncing) {
//            wait_ms(1);
    } else {
      for (uint8_t i = 0; i < THIS_DEVICE_ROWS; i++) {
        if (matrix[i + matrix_offset] != matrix_debouncing[i + matrix_offset]) {
          for (uint8_t j = 0; j < THIS_DEVICE_COLS; j++) {
            if ((matrix[i + matrix_offset]
                ^ matrix_debouncing[i + matrix_offset]) & (1 << j)) {
              ble_switch_send[matrix_changed].timing = timing;
              ble_switch_send[matrix_changed].state = (matrix_debouncing[i
                  + matrix_offset] >> j) & 1;
              ble_switch_send[matrix_changed].row = i;
              ble_switch_send[matrix_changed].col = j;
              matrix_changed++;
            }
          }
        }
        matrix[i + matrix_offset] = matrix_debouncing[i + matrix_offset];
      }
    }
  }

/* Power consumption test*/
//  static int cnt1, cnt2;
//  if(cnt1 == 0){
////    matrix[0] |= 0x1;
//    matrix_changed = 1;
//    ble_switch_send[0].timing = 0;
//    ble_switch_send[0].row = 3;
//    ble_switch_send[0].col = 0;
//    ble_switch_send[0].state = 1;
//  } else if(cnt1==3){
////    matrix[0] &= ~0x1;
//    matrix_changed = 1;
//    ble_switch_send[0].timing = 0;
//    ble_switch_send[0].row = 3;
//    ble_switch_send[0].col = 0;
//    ble_switch_send[0].state = 0;
//  }
//  cnt1++;
//  cnt1%=6;
//
//  if(cnt2 == 0){
//      sprintf(str, "%4dmV\n", get_vcc());
////      send_string(str);
//  }
//  cnt2++;
//  cnt2%=10000;

#ifdef NRF_SEPARATE_KEYBOARD_SLAVE
  if (matrix_changed) {
    NRF_LOG_INFO("NUS send\r\n");
    ble_nus_send_bytes((uint8_t*) ble_switch_send, matrix_changed*sizeof(ble_switch_state_t));
  }
#else
  UNUSED_VARIABLE(ble_switch_send);
#endif

  timing++;

  // Process received keys
  ble_switch_state_t rcv_key;
  uint8_t process = 0;
  if (rcv_keys_queue.cnt) {
    process = rcv_keys_queue.received_keys[rcv_keys_queue.ridx].timing;
    while(rcv_keys_queue.cnt) {
      rcv_key = rcv_keys_queue.received_keys[rcv_keys_queue.ridx];
      if (process == rcv_key.timing) {
        set_received_key(rcv_key);
        rcv_keys_queue.ridx++;
        rcv_keys_queue.ridx %= QUEUE_LEN;
        rcv_keys_queue.cnt--;
      } else {
        break;
      }
    }
  }

  matrix_scan_user();

  return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
}

static void init_rows() {
  for(int i=0; i<THIS_DEVICE_ROWS; i++) {
    nrf_gpio_cfg(row_pins[i],
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE);
  }
}
/* Column pin configuration
 */
static void  init_cols(void)
{
  for(int i=0; i<THIS_DEVICE_COLS; i++) {
    nrf_gpio_cfg_input(col_pins[i], NRF_GPIO_PIN_PULLUP);
  }
}

/* Returns status of switches(1:on, 0:off) */
matrix_row_t read_cols(void)
{
  matrix_row_t row = 0;
  for (int i=0; i<THIS_DEVICE_COLS; i++) {
    row |= ((nrf_gpio_pin_read(col_pins[i]) ? 0 : 1) << i);
  }
  return row;
}

/* Row pin configuration
 */
void unselect_rows(void)
{
  for(int i=0; i<THIS_DEVICE_ROWS; i++) {
    nrf_gpio_pin_set(row_pins[i]);
  }
}

void select_row(uint8_t row)
{
    nrf_gpio_pin_clear(row_pins[row]);
}

void ble_nus_on_disconnect() {
#if defined(NRF_SEPARATE_KEYBOARD_MASTER) || defined(NRF_SEPARATE_KEYBOARD_SLAVE)
  uint8_t matrix_offset = isLeftHand ? THIS_DEVICE_ROWS : 0;
  for (uint8_t i = 0; i < THIS_DEVICE_ROWS; i++) {
    matrix[i + matrix_offset] = 0;
  }
  rcv_keys_queue.cnt = 0;
#endif
}

void ble_nus_packetrcv_handler(ble_switch_state_t* buf, uint8_t len) {
  for (int i=0; i<len; i++) {
    rcv_keys_queue.received_keys[rcv_keys_queue.widx++] = buf[i];
    rcv_keys_queue.widx %= QUEUE_LEN;
    rcv_keys_queue.cnt++;
  }
}
