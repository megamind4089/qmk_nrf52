#ifndef TMK_CORE_PROTOCOL_NRF_BLE_MAIN_H_
#define TMK_CORE_PROTOCOL_NRF_BLE_MAIN_H_

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "peer_manager.h"
#include "app_scheduler.h"
//#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "adc.h"

#include "host.h"

#ifdef NRF_SEPARATE_KEYBOARD_MASTER
#define CENTRAL_LINK_COUNT               1                                          /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#else
#define CENTRAL_LINK_COUNT               0                                          /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#endif
#define PERIPHERAL_LINK_COUNT            1                                          /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

void ble_send_keyboard(report_keyboard_t *report);
void ble_send_mouse(report_mouse_t *report);
void ble_send_system(uint16_t data);
void ble_send_consumer(uint16_t data);
void ble_send_abs_mouse(uint8_t x, uint8_t y);

void restart_advertising_wo_whitelist();
void delete_bonded_peers();
void send_abs_mouse(int8_t x, int8_t y);


void logger_init(void);
void main_task_start(uint8_t interval_ms);
void main_loop(void);
void advertising_start(void);
void battery_level_update(void);
void timers_init(void (*main_task)(void*));
void gap_params_init(void);
void services_init(void);
void conn_params_init(void);
void timers_start(void);
void sleep_mode_enter(void);
void ble_stack_init(void);
void scheduler_init(void);
void peer_manager_init();
void advertising_init(void);
void buttons_leds_init(bool * p_erase_bonds);
void power_manage(void);

#endif /* TMK_CORE_PROTOCOL_NRF_BLE_MAIN_H_ */
