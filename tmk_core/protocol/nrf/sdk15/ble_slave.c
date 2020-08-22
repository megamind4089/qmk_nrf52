/**
 * Copyright (c) 2012 - 2018, Nordic Semiconductor ASA
 * Copyright (c) 2018, Sekigon
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_gpio.h"
#include "app_timer.h"
#include "ble_nus.h"
#include "app_util_platform.h"
#include "nrf_pwr_mgmt.h"
#include "peer_manager.h"
#include "fds.h"
#include "nrf_fstorage.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "matrix.h"
#include "ble_slave.h"
#include "ble_common.h"

#define APP_FEATURE_NOT_SUPPORTED       BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2        /**< Reply when unsupported features are requested. */

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

#define DEVICE_NAME                     "Nordic_UART"                               /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

#define APP_ADV_DURATION                18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

#ifndef BLE_NUS_MIN_INTERVAL
  #define BLE_NUS_MIN_INTERVAL 20
#endif
#ifndef BLE_NUS_MAX_INTERVAL
  #define BLE_NUS_MAX_INTERVAL 60
#endif
#if BLE_NUS_MIN_INTERVAL > BLE_NUS_MAX_INTERVAL
#error "MIN_INTERVAL should be larger than MAX_INTERVAL"
#endif
#ifndef BLE_NUS_SLAVE_LATENCY
  #define BLE_NUS_SLAVE_LATENCY 8
#endif
#ifndef BLE_NUS_TIMEOUT
  #define BLE_NUS_TIMEOUT 1500
#endif
#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(BLE_NUS_MIN_INTERVAL, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(BLE_NUS_MAX_INTERVAL, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   BLE_NUS_SLAVE_LATENCY                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(BLE_NUS_TIMEOUT, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */
                                       /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_BOND                   1                                           /**< Perform bonding. */
#define SEC_PARAM_MITM                   0                                           /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                   0                                           /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS               0                                           /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES        BLE_GAP_IO_CAPS_NONE                        /**< No I/O capabilities. */
#define SEC_PARAM_OOB                    0                                           /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE           7                                           /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE           16                                          /**< Maximum encryption key size. */

#define APP_ADV_FAST_INTERVAL            0x0028                                     /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 25 ms.). */
#define APP_ADV_SLOW_INTERVAL            0x0C80                                     /**< Slow advertising interval (in units of 0.625 ms. This value corrsponds to 2 seconds). */
#define APP_ADV_FAST_TIMEOUT             3000                                       /**< The advertising duration of fast advertising in units of 10 milliseconds. */
#define APP_ADV_SLOW_TIMEOUT             18000                                      /**< The advertising duration of slow advertising in units of 10 milliseconds. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */

static pm_peer_id_t m_peer_id; /**< Device reference handle to the current bonded central. */

static pm_peer_id_t m_whitelist_peers[BLE_GAP_WHITELIST_ADDR_MAX_COUNT]; /**< List of peers currently in the whitelist. */
uint32_t m_whitelist_peer_cnt; /**< Number of peers currently in the whitelist. */
static bool m_is_wl_changed; /**< Indicates if the whitelist has been changed since last time it has been updated in the Peer Manager. */

static uint16_t   m_conn_handle          = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
static uint16_t   m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */

static ble_uuid_t m_adv_uuids[] = { { BLE_UUID_NUS_SERVICE,
    NUS_SERVICE_UUID_TYPE } }; /**< Universally unique service identifier. */

BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);                                                 /**< Advertising module instance. */
APP_TIMER_DEF( main_task_timer_id);

/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name) {
  app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

///**@brief Function for handling advertising errors.
// *
// * @param[in] nrf_error  Error code containing information about what went wrong.
// */
//static void ble_advertising_error_handler(uint32_t nrf_error) {
//  APP_ERROR_HANDLER(nrf_error);
//}
//

/**@brief Function for handling events from the GATT library. */
static void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        m_ble_nus_max_data_len = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
        NRF_LOG_DEBUG("Data len is set to 0x%X(%d)", m_ble_nus_max_data_len, m_ble_nus_max_data_len);
    }
    NRF_LOG_DEBUG("ATT MTU exchange completed. central 0x%x peripheral 0x%x",
                  p_gatt->att_mtu_desired_central,
                  p_gatt->att_mtu_desired_periph);
}

/**@brief Function for initializing the GATT library. */
static void gatt_init(void) {
  ret_code_t err_code;

  err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
  APP_ERROR_CHECK(err_code);

  err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt,
      NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
 *          the device. It also sets the permissions and appearance.
 */
void gap_params_init(void) {
  uint32_t err_code;
  ble_gap_conn_params_t gap_conn_params;
  ble_gap_conn_sec_mode_t sec_mode;

  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

  err_code = sd_ble_gap_device_name_set(&sec_mode,
      (const uint8_t *) DEVICE_NAME, strlen(DEVICE_NAME));
  APP_ERROR_CHECK(err_code);

  memset(&gap_conn_params, 0, sizeof(gap_conn_params));

  gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
  gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
  gap_conn_params.slave_latency = SLAVE_LATENCY;
  gap_conn_params.conn_sup_timeout = CONN_SUP_TIMEOUT;

  err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
  APP_ERROR_CHECK(err_code);

  gatt_init();
}

/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const * p_evt) {
  ret_code_t err_code;

//  NRF_LOG_INFO("pm_evt_handler %d", p_evt->evt_id);
  switch (p_evt->evt_id) {
  case PM_EVT_BONDED_PEER_CONNECTED: {
    NRF_LOG_DEBUG("Connected to a previously bonded device.");
  }
    break;

  case PM_EVT_CONN_SEC_SUCCEEDED: {
    NRF_LOG_INFO("Connected to master keyboard:%d", p_evt->peer_id);
    NRF_LOG_DEBUG(
        "Connection secured: role: %d, conn_handle: 0x%x, procedure: %d.",
        ble_conn_state_role(p_evt->conn_handle), p_evt->conn_handle,
        p_evt->params.conn_sec_succeeded.procedure);

    m_peer_id = p_evt->peer_id;
  }
    break;

  case PM_EVT_CONN_SEC_FAILED: {
    /* Often, when securing fails, it shouldn't be restarted, for security reasons.
     * Other times, it can be restarted directly.
     * Sometimes it can be restarted, but only after changing some Security Parameters.
     * Sometimes, it cannot be restarted until the link is disconnected and reconnected.
     * Sometimes it is impossible, to secure the link, or the peer device does not support it.
     * How to handle this error is highly application dependent. */
  }
    break;

  case PM_EVT_CONN_SEC_CONFIG_REQ: {
    // Reject pairing request from an already bonded peer.
    pm_conn_sec_config_t conn_sec_config = { .allow_repairing = false };
    pm_conn_sec_config_reply(p_evt->conn_handle, &conn_sec_config);
  }
    break;

  case PM_EVT_STORAGE_FULL: {
    // Run garbage collection on the flash.
    err_code = fds_gc();
    if (err_code == FDS_ERR_NO_SPACE_IN_QUEUES) {
      // Retry.
    } else {
      APP_ERROR_CHECK(err_code);
    }
  }
    break;

  case PM_EVT_PEERS_DELETE_SUCCEEDED: {
    advertising_start();
  }
    break;

  case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED: {
    if (p_evt->params.peer_data_update_succeeded.flash_changed
        && (p_evt->params.peer_data_update_succeeded.data_id
            == PM_PEER_DATA_ID_BONDING)) {
      NRF_LOG_INFO("New Bond, add the peer to the whitelist if possible");
//      advertising_start();
      NRF_LOG_INFO("\tm_whitelist_peer_cnt %d, MAX_PEERS_WLIST %d",
          m_whitelist_peer_cnt + 1, BLE_GAP_WHITELIST_ADDR_MAX_COUNT);
      // Note: You should check on what kind of white list policy your application should use.

      if (m_whitelist_peer_cnt < BLE_GAP_WHITELIST_ADDR_MAX_COUNT) {
        // Bonded to a new peer, add it to the whitelist.
        m_whitelist_peers[m_whitelist_peer_cnt++] = m_peer_id;

        // The whitelist has been modified, update it in the Peer Manager.
        err_code = pm_device_identities_list_set(m_whitelist_peers,
            m_whitelist_peer_cnt);
//        if (err_code != NRF_ERROR_NOT_SUPPORTED) {
//          APP_ERROR_CHECK(err_code);
//        }

        err_code = pm_whitelist_set(m_whitelist_peers, m_whitelist_peer_cnt);
        APP_ERROR_CHECK(err_code);
      }
    }
  }
    break;

  case PM_EVT_PEER_DATA_UPDATE_FAILED: {
    // Assert.
    APP_ERROR_CHECK(p_evt->params.peer_data_update_failed.error);
  }
    break;

  case PM_EVT_PEER_DELETE_FAILED: {
    // Assert.
    APP_ERROR_CHECK(p_evt->params.peer_delete_failed.error);
  }
    break;

  case PM_EVT_PEERS_DELETE_FAILED: {
    // Assert.
    APP_ERROR_CHECK(p_evt->params.peers_delete_failed_evt.error);
  }
    break;

  case PM_EVT_ERROR_UNEXPECTED: {
    // Assert.
    APP_ERROR_CHECK(p_evt->params.error_unexpected.error);
  }
    break;

  case PM_EVT_CONN_SEC_START:
  case PM_EVT_PEER_DELETE_SUCCEEDED:
  case PM_EVT_LOCAL_DB_CACHE_APPLIED:
  case PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED:
    // This can happen when the local DB has changed.
  case PM_EVT_SERVICE_CHANGED_IND_SENT:
  case PM_EVT_SERVICE_CHANGED_IND_CONFIRMED:
  default:
    break;
  }
}

/**@brief Function for the Peer Manager initialization.
 *
 * @param[in] erase_bonds  Indicates whether bonding information should be cleared from
 *                         persistent storage during initialization of the Peer Manager.
 */
void peer_manager_init() {
  ble_gap_sec_params_t sec_param;
  ret_code_t err_code;

  err_code = pm_init();
  APP_ERROR_CHECK(err_code);

//  if (erase_bonds) {
//    err_code = pm_peers_delete();
//    APP_ERROR_CHECK(err_code);
//  }

  memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

  // Security parameters to be used for all security procedures.
  sec_param.bond = SEC_PARAM_BOND;
  sec_param.mitm = SEC_PARAM_MITM;
  sec_param.lesc = SEC_PARAM_LESC;
  sec_param.keypress = SEC_PARAM_KEYPRESS;
  sec_param.io_caps = SEC_PARAM_IO_CAPABILITIES;
  sec_param.oob = SEC_PARAM_OOB;
  sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
  sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
  sec_param.kdist_own.enc = 1;
  sec_param.kdist_own.id = 1;
  sec_param.kdist_peer.enc = 1;
  sec_param.kdist_peer.id = 1;

  err_code = pm_sec_params_set(&sec_param);
  APP_ERROR_CHECK(err_code);

  err_code = pm_register(pm_evt_handler);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in] p_nus    Nordic UART Service structure.
 * @param[in] p_data   Data to be send to UART module.
 * @param[in] length   Length of the data.
 */
/**@snippet [Handling the data received over BLE] */
static void nus_data_handler(ble_nus_evt_t * p_evt) {
}
/**@snippet [Handling the data received over BLE] */


/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing services that will be used by the application.
 */
void services_init(void) {
  uint32_t err_code;
  ble_nus_init_t nus_init;
  nrf_ble_qwr_init_t qwr_init = {0};

  // Initialize Queued Write Module.
  qwr_init.error_handler = nrf_qwr_error_handler;

  err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
  APP_ERROR_CHECK(err_code);

  memset(&nus_init, 0, sizeof(nus_init));

  nus_init.data_handler = nus_data_handler;

  err_code = ble_nus_init(&m_nus, &nus_init);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
void on_conn_params_evt(ble_conn_params_evt_t * p_evt) {
  uint32_t err_code;

  if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED) {
    err_code = sd_ble_gap_disconnect(m_conn_handle,
        BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
    APP_ERROR_CHECK(err_code);
  }
}

/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error) {
  APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
void conn_params_init(void) {
  uint32_t err_code;
  ble_conn_params_init_t cp_init;

  memset(&cp_init, 0, sizeof(cp_init));

  cp_init.p_conn_params = NULL;
  cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
  cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
  cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
  cp_init.start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID;
  cp_init.disconnect_on_fail = false;
  cp_init.evt_handler = on_conn_params_evt;
  cp_init.error_handler = conn_params_error_handler;

  err_code = ble_conn_params_init(&cp_init);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt) {
  uint32_t err_code;
  switch (ble_adv_evt) {
  case BLE_ADV_EVT_DIRECTED:
    NRF_LOG_INFO("BLE_ADV_EVT_DIRECTED\r\n");
    break; //BLE_ADV_EVT_DIRECTED

  case BLE_ADV_EVT_FAST:
    NRF_LOG_INFO("BLE_ADV_EVT_FAST\r\n");
    break; //BLE_ADV_EVT_FAST

  case BLE_ADV_EVT_SLOW:
    NRF_LOG_INFO("BLE_ADV_EVT_SLOW\r\n");
    break; //BLE_ADV_EVT_SLOW

  case BLE_ADV_EVT_FAST_WHITELIST:
    NRF_LOG_INFO("BLE_ADV_EVT_FAST_WHITELIST\r\n");
    break; //BLE_ADV_EVT_FAST_WHITELIST

  case BLE_ADV_EVT_SLOW_WHITELIST:
    NRF_LOG_INFO("BLE_ADV_EVT_SLOW_WHITELIST\r\n");
    break; //BLE_ADV_EVT_SLOW_WHITELIST

  case BLE_ADV_EVT_IDLE:
    sleep_mode_enter();
    break; //BLE_ADV_EVT_IDLE

  case BLE_ADV_EVT_WHITELIST_REQUEST: {
    ble_gap_addr_t whitelist_addrs[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
    ble_gap_irk_t whitelist_irks[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
    uint32_t addr_cnt = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;
    uint32_t irk_cnt = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

    err_code = pm_whitelist_get(whitelist_addrs, &addr_cnt, whitelist_irks,
        &irk_cnt);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEBUG(
        "pm_whitelist_get returns %d addr in whitelist and %d irk whitelist\r\n",
        addr_cnt, irk_cnt);

    // Apply the whitelist.
    err_code = ble_advertising_whitelist_reply(&m_advertising,
        whitelist_addrs, addr_cnt,
        whitelist_irks, irk_cnt);
    APP_ERROR_CHECK(err_code);
  }
    break; //BLE_ADV_EVT_WHITELIST_REQUEST

  case BLE_ADV_EVT_PEER_ADDR_REQUEST: {
    pm_peer_data_bonding_t peer_bonding_data;

    // Only Give peer address if we have a handle to the bonded peer.
    if (m_peer_id != PM_PEER_ID_INVALID) {
      err_code = pm_peer_data_bonding_load(m_peer_id, &peer_bonding_data);
      if (err_code != NRF_ERROR_NOT_FOUND) {
        APP_ERROR_CHECK(err_code);

        ble_gap_addr_t * p_peer_addr =
            &(peer_bonding_data.peer_ble_id.id_addr_info);
        err_code = ble_advertising_peer_addr_reply(&m_advertising, p_peer_addr);
        APP_ERROR_CHECK(err_code);
      }
    }
  }
    break; //BLE_ADV_EVT_PEER_ADDR_REQUEST

  default:
    break;
  }
}

/**@brief Function for the application's SoftDevice event handler.
 *
 * @param[in] p_ble_evt SoftDevice event.
 */
static void on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context) {
  uint32_t err_code;

  switch (p_ble_evt->header.evt_id) {
  case BLE_GAP_EVT_CONNECTED:
//            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
//            APP_ERROR_CHECK(err_code);
    m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
    err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
    APP_ERROR_CHECK(err_code);
    break; // BLE_GAP_EVT_CONNECTED

  case BLE_GAP_EVT_DISCONNECTED:
    NRF_LOG_INFO("Disconnected from master keyboard");
//
//            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
//            APP_ERROR_CHECK(err_code);
    m_conn_handle = BLE_CONN_HANDLE_INVALID;

    if (m_is_wl_changed) {
      // The whitelist has been modified, update it in the Peer Manager.
      err_code = pm_whitelist_set(m_whitelist_peers, m_whitelist_peer_cnt);
      APP_ERROR_CHECK(err_code);

      err_code = pm_device_identities_list_set(m_whitelist_peers,
          m_whitelist_peer_cnt);
      if (err_code != NRF_ERROR_NOT_SUPPORTED) {
        APP_ERROR_CHECK(err_code);
      }
    }
    break; // BLE_GAP_EVT_DISCONNECTED

  case BLE_GAP_EVT_PHY_UPDATE_REQUEST: {
    NRF_LOG_DEBUG("PHY update request.");
    ble_gap_phys_t const phys = { .rx_phys = BLE_GAP_PHY_AUTO, .tx_phys =
        BLE_GAP_PHY_AUTO, };
    err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
    APP_ERROR_CHECK(err_code);
  }
    break;

//        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
//            // Pairing not supported
//            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
//            APP_ERROR_CHECK(err_code);
//            break; // BLE_GAP_EVT_SEC_PARAMS_REQUEST
//
//        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
//            // No system attributes have been stored.
//            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
//            APP_ERROR_CHECK(err_code);
//            break; // BLE_GATTS_EVT_SYS_ATTR_MISSING

  case BLE_GATTC_EVT_TIMEOUT:
    // Disconnect on GATT Client timeout event.
    err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
        BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
    break; // BLE_GATTC_EVT_TIMEOUT

  case BLE_GATTS_EVT_TIMEOUT:
    // Disconnect on GATT Server timeout event.
    err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
        BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
    break; // BLE_GATTS_EVT_TIMEOUT

  case BLE_EVT_USER_MEM_REQUEST:
    err_code = sd_ble_user_mem_reply(p_ble_evt->evt.gattc_evt.conn_handle,
        NULL);
    APP_ERROR_CHECK(err_code);
    break; // BLE_EVT_USER_MEM_REQUEST

  case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST: {
    ble_gatts_evt_rw_authorize_request_t req;
    ble_gatts_rw_authorize_reply_params_t auth_reply;

    req = p_ble_evt->evt.gatts_evt.params.authorize_request;

    if (req.type != BLE_GATTS_AUTHORIZE_TYPE_INVALID) {
      if ((req.request.write.op == BLE_GATTS_OP_PREP_WRITE_REQ)
          || (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW)
          || (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL)) {
        if (req.type == BLE_GATTS_AUTHORIZE_TYPE_WRITE) {
          auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
        } else {
          auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
        }
        auth_reply.params.write.gatt_status = APP_FEATURE_NOT_SUPPORTED;
        err_code = sd_ble_gatts_rw_authorize_reply(
            p_ble_evt->evt.gatts_evt.conn_handle, &auth_reply);
        APP_ERROR_CHECK(err_code);
      }
    }
  }
    break; // BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST

  default:
    // No implementation needed.
    break;
  }
}

///**@brief Function for dispatching a SoftDevice event to all modules with a SoftDevice
// *        event handler.
// *
// * @details This function is called from the SoftDevice event interrupt handler after a
// *          SoftDevice event has been received.
// *
// * @param[in] p_ble_evt  SoftDevice event.
// */
//static void ble_evt_dispatch(ble_evt_t * p_ble_evt) {
//  ble_conn_state_on_ble_evt(p_ble_evt);
//  pm_on_ble_evt(p_ble_evt);
//  on_ble_evt(p_ble_evt);
//  ble_advertising_on_ble_evt(p_ble_evt);
//  ble_conn_params_on_ble_evt(p_ble_evt);
//  ble_nus_on_ble_evt(&m_nus, p_ble_evt);
////    bsp_btn_ble_on_ble_evt(p_ble_evt);
//
//}

// Radio event setting for master and slave synchronization
void radio_event_callback(bool active);
void SWI1_IRQHandler(void) {
  radio_event_callback(false);
}
uint32_t ble_radio_notification_init(uint32_t irq_priority, uint8_t distance) {
  uint32_t err_code;

  // Initialize Radio Notification software interrupt
  err_code = sd_nvic_ClearPendingIRQ(SWI1_IRQn);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  err_code = sd_nvic_SetPriority(SWI1_IRQn, irq_priority);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  err_code = sd_nvic_EnableIRQ(SWI1_IRQn);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // Configure the event
  return sd_radio_notification_cfg_set(NRF_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE,
      distance);
}


/**@brief Function for the SoftDevice initialization.
 *
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void) {
  ret_code_t err_code;

  err_code = nrf_sdh_enable_request();
  APP_ERROR_CHECK(err_code);

  // Configure the BLE stack using the default settings.
  // Fetch the start address of the application RAM.
  uint32_t ram_start = 0;
  err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
  APP_ERROR_CHECK(err_code);

  // Enable BLE stack.
  err_code = nrf_sdh_ble_enable(&ram_start);
  APP_ERROR_CHECK(err_code);

  ble_radio_notification_init(3,
        NRF_RADIO_NOTIFICATION_DISTANCE_800US);
  // Register a handler for BLE events.
  NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, on_ble_evt, NULL);

}

/**@brief Function for initializing the Advertising functionality.
 */
void advertising_init(void) {
  uint32_t err_code;
  ble_advertising_init_t init;
  uint8_t adv_flags;

  // Build and set advertising data
  memset(&init, 0, sizeof(init));

  adv_flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
  init.advdata.name_type = BLE_ADVDATA_FULL_NAME;
  init.advdata.include_appearance = true;
  init.advdata.flags = adv_flags;

  init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids)
      / sizeof(m_adv_uuids[0]);
  init.srdata.uuids_complete.p_uuids = m_adv_uuids;

  init.config.ble_adv_whitelist_enabled = true;
  init.config.ble_adv_directed_high_duty_enabled = true;
  init.config.ble_adv_directed_enabled = false;
  init.config.ble_adv_directed_interval = 0;
  init.config.ble_adv_directed_timeout = 0;
  init.config.ble_adv_fast_enabled = true;
  init.config.ble_adv_fast_interval = APP_ADV_FAST_INTERVAL;
  init.config.ble_adv_fast_timeout = APP_ADV_FAST_TIMEOUT;
  init.config.ble_adv_slow_enabled = true;
  init.config.ble_adv_slow_interval = APP_ADV_SLOW_INTERVAL;
  init.config.ble_adv_slow_timeout = APP_ADV_SLOW_TIMEOUT;

  init.evt_handler = on_adv_evt;

  err_code = ble_advertising_init(&m_advertising, &init);
  APP_ERROR_CHECK(err_code);

  ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

uint32_t ble_nus_send_bytes(uint8_t* buf, uint16_t len) {
  uint32_t err_code = ble_nus_data_send(&m_nus, buf, &len, m_conn_handle);
  return err_code;
}

void timers_init(void (*main_task)(void*)) {
  ret_code_t err_code = app_timer_init();
  APP_ERROR_CHECK(err_code);
  APP_ERROR_CHECK(
      app_timer_create(&main_task_timer_id, APP_TIMER_MODE_REPEATED,
          main_task));
}

void ble_disconnect() {
  sd_ble_gap_adv_stop(m_advertising.adv_handle);
  if (m_conn_handle != BLE_CONN_HANDLE_INVALID) {
    ret_code_t err_code = sd_ble_gap_disconnect(m_conn_handle,
    BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    if (err_code != NRF_ERROR_INVALID_STATE) {
      APP_ERROR_CHECK(err_code);
    }
  }
}

bool ble_connected() {
    return m_conn_handle != BLE_CONN_HANDLE_INVALID;
}

void advertising_start(void) {
  ret_code_t ret;

  memset(m_whitelist_peers, PM_PEER_ID_INVALID, sizeof(m_whitelist_peers));
  m_whitelist_peer_cnt = (sizeof(m_whitelist_peers) / sizeof(pm_peer_id_t));

  peer_list_get(m_whitelist_peers, &m_whitelist_peer_cnt);

  ret = pm_whitelist_set(m_whitelist_peers, m_whitelist_peer_cnt);
  APP_ERROR_CHECK(ret);

  // Setup the device identies list.
  // Some SoftDevices do not support this feature.
  ret = pm_device_identities_list_set(m_whitelist_peers, m_whitelist_peer_cnt);
  if (ret != NRF_ERROR_NOT_SUPPORTED) {
    APP_ERROR_CHECK(ret);
  }

  m_is_wl_changed = false;

  ret = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
  APP_ERROR_CHECK(ret);
  NRF_LOG_INFO("Advertising start...");
}

void main_task_start(uint8_t interval_ms) {
  uint32_t err_code = app_timer_start(main_task_timer_id,
      APP_TIMER_TICKS(interval_ms), NULL);
  APP_ERROR_CHECK(err_code);
}
