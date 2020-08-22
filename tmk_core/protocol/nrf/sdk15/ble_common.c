
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_gpio.h"
#include "nrfx_power.h"
#include "peer_manager.h"

#include "app_ble_func.h"
#include "usbd.h"

#ifndef THIS_DEVICE_ROWS
#include "pin_assign.h"
#endif

uint8_t keyboard_idle __attribute__((aligned(2))) = 0;
uint8_t keyboard_protocol __attribute__((aligned(2))) = 1;
uint16_t keyboard_led_stats __attribute__((aligned(2))) = 0;

/**@brief Function for placing the application in low power state while waiting for events.
 */
static inline void power_manage(void) {
  nrf_pwr_mgmt_run();
}

void main_loop(void) {
  if (NRF_LOG_PROCESS() == false) {
    power_manage();
  }
}

/**@brief Function for the Event Scheduler initialization.
 */
void scheduler_init(void) {
}

/**@brief Function for initializing power management.
 */
static inline void power_management_init(void) {
  ret_code_t err_code;
  err_code = nrf_pwr_mgmt_init();
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the nrf log module.
 */
void logger_init(void) {
  ret_code_t err_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(err_code);

  NRF_LOG_DEFAULT_BACKENDS_INIT();

#ifdef NRF52840_XXAA
  extern const nrf_log_backend_api_t nrf_log_backend_cdc_acm_api;
  static nrf_log_backend_t backend = {.p_api = &nrf_log_backend_cdc_acm_api};
  nrf_log_backend_add(&backend, NRF_LOG_SEVERITY_INFO);
  nrf_log_backend_enable(&backend);
#endif

  power_management_init();
}

/**@brief Fetch the list of peer manager peer IDs.
 *
 * @param[inout] p_peers   The buffer where to store the list of peer IDs.
 * @param[inout] p_size    In: The size of the @p p_peers buffer.
 *                         Out: The number of peers copied in the buffer.
 */
void peer_list_get(pm_peer_id_t * p_peers, uint32_t * p_size) {
  pm_peer_id_t peer_id;
  uint32_t peers_to_copy;

  peers_to_copy =
      (*p_size < BLE_GAP_WHITELIST_ADDR_MAX_COUNT) ?
          *p_size : BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

  peer_id = pm_next_peer_id_get(PM_PEER_ID_INVALID);
  *p_size = 0;

  while ((peer_id != PM_PEER_ID_INVALID) && (peers_to_copy--)) {
    p_peers[(*p_size)++] = peer_id;
    peer_id = pm_next_peer_id_get(peer_id);
  }
}

/**@brief Clear bond information from persistent storage.
 */
void delete_bonds(void) {
  ret_code_t err_code;

  NRF_LOG_INFO("Erase all bonds!");

  err_code = pm_peers_delete();
  APP_ERROR_CHECK(err_code);
}

void delete_bond_id(uint8_t id) {
  ret_code_t err_code;

  ble_disconnect();

  err_code = pm_peer_delete(id);
  APP_ERROR_CHECK(err_code);

  NRF_LOG_INFO("Erase bonds ID: %d", id);
}

void nrfmicro_power_enable(bool enable);

/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
void sleep_mode_enter(void) {
  extern const uint32_t row_pins[THIS_DEVICE_ROWS];
  extern const uint32_t col_pins[THIS_DEVICE_COLS];
  int i;

  if (nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_CONNECTED ||
      nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_READY) {
    return;
  }

  nrfmicro_power_enable(false);

  for (i=0; i<THIS_DEVICE_ROWS; i++) {
    nrf_gpio_pin_clear(row_pins[i]);
  }
  for (i=0; i<THIS_DEVICE_COLS; i++) {
    nrf_gpio_cfg_sense_input(col_pins[i], NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
  }

  sd_power_system_off();
}
