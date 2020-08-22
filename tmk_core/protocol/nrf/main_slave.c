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

#include "ble_slave.h"
#include "usbd.h"
#include "nrf_soc.h"
#include "nrf_power.h"
#include "matrix.h"
#include "keyboard.h"
#include "printf.h"
#include "sendchar.h"

#ifdef RGBLIGHT_ENABLE
#include "rgblight.h"
#endif
#ifdef OLED_DRIVER_ENABLE
#    include "oled_driver.h"
#endif

const uint8_t MAINTASK_INTERVAL=17;

void sendchar_pf(void *p, char c){
  UNUSED_VARIABLE(p);
  UNUSED_VARIABLE(c);
};
void timer_tick(uint8_t interval);
static void slave_main_tasks(void* context) {
  timer_tick(MAINTASK_INTERVAL);
  matrix_scan();

#ifdef OLED_DRIVER_ENABLE
    oled_task();
#endif

#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_ANIMATIONS)
  rgblight_task();
#endif
}

/**@brief Application main function.
 */
int main(void) {
  // Initialize.
  logger_init();
  timers_init(slave_main_tasks);

  usbd_init();

  ble_stack_init();
  sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
  services_init();
  scheduler_init();
  peer_manager_init();

  gap_params_init();
  advertising_init();
  conn_params_init();

//  matrix_init();
  init_printf(NULL, sendchar_pf);
  keyboard_init();
  advertising_start();
  main_task_start(MAINTASK_INTERVAL);
  usbd_enable();
  // Enter main loop.
  for (;;) {
    usbd_process();
    main_loop();
  }
}


/**
 * @}
 */

#include "app_error.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#if NRF_SDK_MAJOR_VER==15
#include "nrf_strerror.h"
#define ERR_TO_STRING(err_code) nrf_strerror_get(err_code)
#else
#define ERR_TO_STRING(err_code) (uint32_t)ERR_TO_STR(err_code)
#endif

__WEAK void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
  error_info_t *err = (error_info_t*)info;
    NRF_LOG_ERROR("Fatal %d:%s %s:%d\r\n", err->err_code, ERR_TO_STRING(err->err_code), (uint32_t)err->p_file_name, err->line_num);
    NRF_LOG_FINAL_FLUSH();
    // On assert, the system can only recover with a reset.
#ifndef DEBUG
    NVIC_SystemReset();
#else
    while(1) { continue; }
    app_error_save_and_stop(id, pc, info);
#endif // DEBUG
}
