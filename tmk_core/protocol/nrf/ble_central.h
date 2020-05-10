#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_assert.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advertising.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "bsp.h"
#include "fds.h"
#include "ble_conn_state.h"


void nus_c_init(void);
void db_discovery_init(void);
void on_ble_central_evt(const ble_evt_t * const p_ble_evt, uint16_t conn_handle);
