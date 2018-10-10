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
#include "ble_central.h"
#include "ble_master.h"
#include "ble_slave.h"
#include "app_ble_func.h"

#include "nrf_fstorage.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "peer_id.h"
#include "ble_nus_c.h"

//#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#if (NRF_SD_BLE_API_VERSION == 3)
#define NRF_BLE_MAX_MTU_SIZE        GATT_MTU_SIZE_DEFAULT                         /**< MTU size used in the softdevice enabling and to reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event. */
#endif


#define APP_BLE_CONN_CFG_TAG    1                                       /**< A tag that refers to the BLE stack configuration we set with @ref sd_ble_cfg_set. Default tag is @ref BLE_CONN_CFG_TAG_DEFAULT. */
#define APP_BLE_OBSERVER_PRIO   3                                       /**< Application's BLE observer priority. You shoulnd't need to modify this value. */

#define SCAN_INTERVAL               0x0100                                        /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                 0x0050                                        /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_DURATION               1000                                  /**< Timout when scanning in 10ms. 0x0000 disables timeout. */
#define SCAN_DURATION_WITELIST      3000                                /**< Duration of the scanning in units of 10 milliseconds. */


#ifndef BLE_NUS_MIN_INTERVAL
  #define BLE_NUS_MIN_INTERVAL 60
#endif
#ifndef BLE_NUS_MAX_INTERVAL
  #define BLE_NUS_MAX_INTERVAL 75
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

#define MIN_CONNECTION_INTERVAL     (uint16_t) MSEC_TO_UNITS(BLE_NUS_MIN_INTERVAL, UNIT_1_25_MS)   /**< Determines minimum connection interval in milliseconds. */
#define MAX_CONNECTION_INTERVAL     (uint16_t) MSEC_TO_UNITS(BLE_NUS_MAX_INTERVAL, UNIT_1_25_MS)    /**< Determines maximum connection interval in milliseconds. */
#define SLAVE_LATENCY               BLE_NUS_SLAVE_LATENCY
#define SUPERVISION_TIMEOUT         (uint16_t) MSEC_TO_UNITS(BLE_NUS_TIMEOUT, UNIT_10_MS)    /**< Determines supervision time-out in units of 10 milliseconds. */

static uint16_t   m_conn_handle          = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
bool m_whitelist_disabled;
/**@brief names which the central applications will scan for, and which will be advertised by the peripherals.
 *  if these are set to empty strings, the UUIDs defined below will be used
 */
static const char m_target_periph_name[] = "Nordic_UART";

/**
 * @brief Parameters used when scanning.
 */
static ble_gap_scan_params_t m_scan_params =
{
    .active   = 1,
    .interval = SCAN_INTERVAL,
    .window   = SCAN_WINDOW,
    .timeout          = SCAN_DURATION,
    .scan_phys        = BLE_GAP_PHY_1MBPS,
    .filter_policy    = BLE_GAP_SCAN_FP_ACCEPT_ALL,
};

/**@brief Connection parameters requested for connection. */
static const ble_gap_conn_params_t m_connection_param = {
    (uint16_t) MIN_CONNECTION_INTERVAL, (uint16_t) MAX_CONNECTION_INTERVAL, SLAVE_LATENCY,
    (uint16_t) SUPERVISION_TIMEOUT };

/**
 * @brief NUS uuid
 */
//static const ble_uuid_t m_nus_uuid = { .uuid = BLE_UUID_NUS_SERVICE, .type =
//    BLE_UUID_TYPE_VENDOR_BEGIN };

BLE_NUS_C_DEF(m_ble_nus_c);                                             /**< BLE NUS service client instance. */
BLE_DB_DISCOVERY_DEF(m_db_disc);
//static ble_nus_c_t m_ble_nus_c; /**< Instance of NUS service. Must be passed to all NUS_C API calls. */

static uint8_t m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_MIN]; /**< buffer where advertising reports will be stored by the SoftDevice. */

/**@brief Pointer to the buffer where advertising reports will be stored by the SoftDevice. */
static ble_data_t m_scan_buffer =
{
    m_scan_buffer_data,
    BLE_GAP_SCAN_BUFFER_MIN
};

#define UUID16_SIZE             2                               /**< Size of 16 bit UUID */
#define UUID32_SIZE             4                               /**< Size of 32 bit UUID */
#define UUID128_SIZE            16                              /**< Size of 128 bit UUID */

/**@brief Macro to unpack 16bit unsigned UUID from an octet stream. */
#define UUID16_EXTRACT(DST, SRC) \
    do                           \
    {                            \
        (*(DST))   = (SRC)[1];   \
        (*(DST)) <<= 8;          \
        (*(DST))  |= (SRC)[0];   \
    } while (0)

/**@brief Variable length data encapsulation in terms of length and pointer to data. */
typedef struct {
  uint8_t * p_data; /**< Pointer to data. */
  uint16_t data_len; /**< Length of data. */
} data_t;



/**@brief Retrieve a list of peer manager peer IDs.
 *
 * @param[inout] p_peers   The buffer where to store the list of peer IDs.
 * @param[inout] p_size    In: The size of the @p p_peers buffer.
 *                         Out: The number of peers copied in the buffer.
 */
//static void peer_list_get(pm_peer_id_t * p_peers, uint32_t * p_size)
//{
//    pm_peer_id_t peer_id;
//    uint32_t     peers_to_copy;
//
//    peers_to_copy = (*p_size < BLE_GAP_WHITELIST_ADDR_MAX_COUNT) ?
//                     *p_size : BLE_GAP_WHITELIST_ADDR_MAX_COUNT;
//
//    peer_id = pm_next_peer_id_get(PM_PEER_ID_INVALID);
//    *p_size = 0;
//
//    while ((peer_id != PM_PEER_ID_INVALID) && (peers_to_copy--))
//    {
//        p_peers[(*p_size)++] = peer_id;
//        peer_id = pm_next_peer_id_get(peer_id);
//    }
//}
//
//
//static void whitelist_load() {
//  ret_code_t ret;
//  pm_peer_id_t peers[8];
//  uint32_t peer_cnt;
//
//  memset(peers, PM_PEER_ID_INVALID, sizeof(peers));
//  peer_cnt = (sizeof(peers) / sizeof(pm_peer_id_t));
//
//  // Load all peers from flash and whitelist them.
//  peer_list_get(peers, &peer_cnt);
//
//  ret = pm_whitelist_set(peers, peer_cnt);
//  APP_ERROR_CHECK(ret);
//
//  // Setup the device identies list.
//  // Some SoftDevices do not support this feature.
//  ret = pm_device_identities_list_set(peers, peer_cnt);
//  if (ret != NRF_ERROR_NOT_SUPPORTED) {
//    APP_ERROR_CHECK(ret);
//  }
//}

/**@brief Function for initiating scanning.
 */
void scan_start(void) {
  ret_code_t err_code;

  if (m_conn_handle != BLE_CONN_HANDLE_INVALID) return;

  NRF_LOG_DEBUG("Chekck fstorage...");
  if (!nrf_fstorage_is_busy(NULL)) {
    (void) sd_ble_gap_scan_stop();

    // Whitelist buffers.
    ble_gap_addr_t whitelist_addrs[8];
    ble_gap_irk_t whitelist_irks[8];

    memset(whitelist_addrs, 0x00, sizeof(whitelist_addrs));
    memset(whitelist_irks, 0x00, sizeof(whitelist_irks));

    uint32_t addr_cnt = (sizeof(whitelist_addrs) / sizeof(ble_gap_addr_t));
    uint32_t irk_cnt = (sizeof(whitelist_irks) / sizeof(ble_gap_irk_t));

    // Reload the whitelist and whitelist all peers.
//    whitelist_load();

    // Get the whitelist previously set using pm_whitelist_set().
    err_code = pm_whitelist_get(whitelist_addrs, &addr_cnt, whitelist_irks,
        &irk_cnt);

    m_scan_params.active = 0;
    m_scan_params.interval = SCAN_INTERVAL;
    m_scan_params.window = SCAN_WINDOW;

    if( !peer_id_is_allocated(0) ) {
      m_whitelist_disabled = true;
    } else {
      m_whitelist_disabled = false;
    }

    if (((addr_cnt == 0) && (irk_cnt == 0)) || (m_whitelist_disabled)) {
      // Don't use whitelist.
      m_scan_params.timeout = SCAN_DURATION;
      m_scan_params.scan_phys = BLE_GAP_PHY_1MBPS;
      m_scan_params.filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
    } else {
      // Use whitelist.
      m_scan_params.scan_phys = BLE_GAP_PHY_1MBPS;
      m_scan_params.filter_policy = BLE_GAP_SCAN_FP_WHITELIST;
      m_scan_params.timeout = SCAN_DURATION_WITELIST;
    }

    err_code = sd_ble_gap_scan_start(&m_scan_params, &m_scan_buffer);
    // It is okay to ignore this error since we are stopping the scan anyway.
    if (err_code != NRF_SUCCESS) {
      NRF_LOG_ERROR("scan start error: %d", err_code);
//      APP_ERROR_CHECK(err_code);
    } else {
      NRF_LOG_INFO("Scanning slave keyboard...");
    }
  } else {
    NRF_LOG_INFO("Fstorage is busy");
  }
}

__WEAK void ble_nus_packetrcv_handler(ble_switch_state_t* buf, uint8_t len) {
  NRF_LOG_DEBUG("nus packet received");
}

__WEAK void ble_nus_on_disconnect() {

}

/**@brief Callback handling NUS Client events.
 *
 * @details This function is called to notify the application of NUS client events.
 *
 * @param[in]   p_ble_nus_c   NUS Client Handle. This identifies the NUS client
 * @param[in]   p_ble_nus_evt Pointer to the NUS Client event.
 */

/**@snippet [Handling events from the ble_nus_c module] */
static void ble_nus_c_evt_handler(ble_nus_c_t * p_ble_nus_c,
    const ble_nus_c_evt_t * p_ble_nus_evt) {
  uint32_t err_code;
  switch (p_ble_nus_evt->evt_type) {
  case BLE_NUS_C_EVT_DISCOVERY_COMPLETE:
    m_conn_handle = p_ble_nus_evt->conn_handle;
    err_code = ble_nus_c_handles_assign(p_ble_nus_c, p_ble_nus_evt->conn_handle,
        &p_ble_nus_evt->handles);
    APP_ERROR_CHECK(err_code);

    // Initiate bonding.
    err_code = pm_conn_secure(p_ble_nus_evt->conn_handle, false);
    if (err_code != NRF_ERROR_INVALID_STATE)
    {
        APP_ERROR_CHECK(err_code);
    }

    err_code = ble_nus_c_tx_notif_enable(p_ble_nus_c);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEBUG("The device has the Nordic UART Service\r\n");
    break;

  case BLE_NUS_C_EVT_NUS_TX_EVT:
    NRF_LOG_DEBUG("NUS:Received\r\n");
    ble_nus_packetrcv_handler((ble_switch_state_t*) p_ble_nus_evt->p_data,
        p_ble_nus_evt->data_len/sizeof(ble_switch_state_t));
    // for (uint32_t i = 0; i < p_ble_nus_evt->data_len; i++)
    // {
    //     while (app_uart_put( p_ble_nus_evt->p_data[i]) != NRF_SUCCESS);
    // }
    break;

  case BLE_NUS_C_EVT_DISCONNECTED:
    m_conn_handle = BLE_CONN_HANDLE_INVALID;
    NRF_LOG_DEBUG("NUS:Disconnected\r\n");
    NRF_LOG_INFO("Slave keyboard is disconnected");
    ble_nus_on_disconnect();
    scan_start();
    break;
  }
}
/**@brief Function for handling the advertising report BLE event.
 *
 * @param[in] p_adv_report  Advertising report from the SoftDevice.
 */
static void on_adv_report(ble_gap_evt_adv_report_t const * p_adv_report) {
  ret_code_t err_code;

//  if (ble_advdata_uuid_find(p_adv_report->data.p_data, p_adv_report->data.len,
//      &m_nus_uuid)) {
  if (ble_advdata_name_find(p_adv_report->data.p_data, p_adv_report->data.len,
      m_target_periph_name)) {
    err_code = sd_ble_gap_connect(&p_adv_report->peer_addr, &m_scan_params,
        &m_connection_param, APP_BLE_CONN_CFG_TAG);

    if (err_code == NRF_SUCCESS) {
      // scan is automatically stopped by the connect
//      err_code = bsp_indication_set(BSP_INDICATE_IDLE);
//      APP_ERROR_CHECK(err_code);
      NRF_LOG_DEBUG("Connecting to target %02x%02x%02x%02x%02x%02x",
          p_adv_report->peer_addr.addr[0], p_adv_report->peer_addr.addr[1],
          p_adv_report->peer_addr.addr[2], p_adv_report->peer_addr.addr[3],
          p_adv_report->peer_addr.addr[4], p_adv_report->peer_addr.addr[5]);
    }
  } else {
    err_code = sd_ble_gap_scan_start(NULL, &m_scan_buffer);
    APP_ERROR_CHECK(err_code);
  }
}

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
//static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
static void on_ble_central_conn_evt(const ble_evt_t * const p_ble_evt) {
  ret_code_t err_code;
  ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

  switch (p_ble_evt->header.evt_id) {
  case BLE_GAP_EVT_ADV_REPORT:
    on_adv_report(&p_gap_evt->params.adv_report);
    break; // BLE_GAP_EVT_ADV_REPORT

  case BLE_GAP_EVT_CONNECTED:
    NRF_LOG_INFO("Connected to the slave keyboard");
    err_code = ble_nus_c_handles_assign(&m_ble_nus_c,
        p_ble_evt->evt.gap_evt.conn_handle, NULL);
    APP_ERROR_CHECK(err_code);

//    err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
//    APP_ERROR_CHECK(err_code);

    // start discovery of services. The NUS Client waits for a discovery result
    err_code = ble_db_discovery_start(&m_db_disc,
        p_ble_evt->evt.gap_evt.conn_handle);
//    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GAP_EVT_TIMEOUT:
    if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_SCAN) {
      NRF_LOG_INFO("Scan timed out.");
      scan_start();
    } else if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN) {
      NRF_LOG_INFO("Connection Request timed out.");
    }
    break;

//  case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
//    // Pairing not supported
//    err_code = sd_ble_gap_sec_params_reply(p_ble_evt->evt.gap_evt.conn_handle,
//        BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
//    APP_ERROR_CHECK(err_code);
//    break;

  case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
    // Accepting parameters requested by peer.
    err_code = sd_ble_gap_conn_param_update(p_gap_evt->conn_handle,
        &p_gap_evt->params.conn_param_update_request.conn_params);
    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GAP_EVT_PHY_UPDATE_REQUEST: {
    NRF_LOG_DEBUG("PHY update request.");
    ble_gap_phys_t const phys = { .rx_phys = BLE_GAP_PHY_AUTO, .tx_phys =
        BLE_GAP_PHY_AUTO, };
    err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
    APP_ERROR_CHECK(err_code);
  }
    break;

  case BLE_GATTC_EVT_TIMEOUT:
    // Disconnect on GATT Client timeout event.
    NRF_LOG_DEBUG("GATT Client Timeout.");
    err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
        BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GATTS_EVT_TIMEOUT:
    // Disconnect on GATT Server timeout event.
    NRF_LOG_DEBUG("GATT Server Timeout.");
    err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
        BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
    break;

  default:
    break;
  }
}

void on_ble_central_evt(const ble_evt_t * const p_ble_evt, uint16_t conn_handle) {
  /** on_ble_central_evt will update the connection handles, so we want to execute it
   * after dispatching to the central applications upon disconnection. */
  if (p_ble_evt->header.evt_id != BLE_GAP_EVT_DISCONNECTED) {
    on_ble_central_conn_evt(p_ble_evt);
  }

//  if (conn_handle < CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT) {
//    ble_db_discovery_on_ble_evt(&m_ble_db_discovery[conn_handle], p_ble_evt);
//  }
//  ble_nus_c_on_ble_evt(&m_ble_nus_c, p_ble_evt);

  // If the peer disconnected, we update the connection handles last.
  if (p_ble_evt->header.evt_id == BLE_GAP_EVT_DISCONNECTED) {
    on_ble_central_conn_evt(p_ble_evt);
  }
}

/**@brief Heart rate collector initialization.
 */
void nus_c_init(void) {
  uint32_t err_code;
  ble_nus_c_init_t nus_c_init_obj;

  nus_c_init_obj.evt_handler = ble_nus_c_evt_handler;

  err_code = ble_nus_c_init(&m_ble_nus_c, &nus_c_init_obj);
  APP_ERROR_CHECK(err_code);
//  UNUSED_VARIABLE(err_code);
}

/**@brief Function for handling database discovery events.
 *
 * @details This function is callback function to handle events from the database discovery module.
 *          Depending on the UUIDs that are discovered, this function should forward the events
 *          to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt) {
  ble_nus_c_on_db_disc_evt(&m_ble_nus_c, p_evt);
}

/**
 * @brief Database discovery initialization.
 */
void db_discovery_init(void) {
  ret_code_t err_code = ble_db_discovery_init(db_disc_handler);
  APP_ERROR_CHECK(err_code);
}
