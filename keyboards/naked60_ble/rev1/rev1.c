#include "matrix.h"
#include "quantum.h"
#include "nrf.h"
#include "app_ble_func.h"

void matrix_init_user() {
  set_usb_enabled(true);
  debug_enable = true;
}
