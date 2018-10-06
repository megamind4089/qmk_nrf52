#ifndef _BLE_SLAVE_H_
#define _BLE_SLAVE_H_

#include <stdint.h>
#include <stdbool.h>

void ble_stack_init(void);
void peer_manager_init();
void scheduler_init();
void gap_params_init(void);
void services_init(void);
void advertising_init(void);
void conn_params_init(void);
void timers_init(void (*main_task)(void*));
void logger_init(void);
//void timers_start(void);
void advertising_start(void);
void main_task_start(uint8_t interval_ms);
void main_loop(void);

uint32_t ble_nus_send_bytes(uint8_t* buf, uint16_t len);

#endif //  _BLE_SLAVE_H_
