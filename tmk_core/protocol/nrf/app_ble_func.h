#ifndef TMK_CORE_PROTOCOL_NRF_APP_BLE_FUNC_H_
#define TMK_CORE_PROTOCOL_NRF_APP_BLE_FUNC_H_

#include "nrf.h"
#include <stdint.h>
#include <stdbool.h>

#define PIN_DUMMY 0xFF

typedef union {
  struct {
    uint8_t timing :8;
    uint8_t state :1;
    uint8_t id :7;
  };
  uint8_t dat[2];
} ble_switch_state_t;

_Static_assert(sizeof(ble_switch_state_t) == 2,
    "too large bitfield");

typedef struct {
  uint8_t row_num;
  uint8_t col_num;
  uint8_t row_pin_start;
  uint8_t col_pin_start;
  uint8_t checksum;
} lp_mat_config_t;

__WEAK void ble_nus_packetrcv_handler(ble_switch_state_t* buf, uint8_t len);
__WEAK void ble_nus_on_disconnect();

uint32_t ble_nus_send_bytes(uint8_t* buf, uint16_t len);

void ble_disconnect();
bool ble_connected();
void restart_advertising_wo_whitelist(void);
void restart_advertising_id(uint8_t id);
void delete_bonds(void);
void delete_bond_id(uint8_t id);

void start_dfu();
void sleep_mode_enter();

uint16_t get_vcc();

#ifdef NRF_SEPARATE_KEYBOARD_SLAVE
#define get_ble_enabled() false
#define set_ble_enabled(x)
#define get_usb_enabled() false
#define set_usb_enabled(x)
#define select_ble()
#define select_usb()
#else
bool get_ble_enabled();
void set_ble_enabled(bool enabled);
bool get_usb_enabled();
void set_usb_enabled(bool enabled);
void select_ble();
void select_usb();
#endif

#endif /* TMK_CORE_PROTOCOL_NRF_APP_BLE_FUNC_H_ */
