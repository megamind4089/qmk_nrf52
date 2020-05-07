
#include "app_ble_func.h"
#include "ble_central.h"
#include "ble_master.h"
#include "ble_slave.h"

#include "ble_hid_c.h"
#include "nrf_fstorage.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "peer_id.h"

//#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#if (NRF_SD_BLE_API_VERSION == 3)
#define NRF_BLE_MAX_MTU_SIZE                                                   \
  GATT_MTU_SIZE_DEFAULT /**< MTU size used in the softdevice enabling and to   \
                           reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST       \
                           event. */
#endif

#define APP_BLE_CONN_CFG_TAG                                                   \
  1 /**< A tag that refers to the BLE stack configuration we set with @ref     \
       sd_ble_cfg_set. Default tag is @ref BLE_CONN_CFG_TAG_DEFAULT. */
#define APP_BLE_OBSERVER_PRIO                                                  \
  3 /**< Application's BLE observer priority. You shoulnd't need to modify     \
       this value. */

#define SCAN_INTERVAL                                                          \
  0x0100 /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                                                            \
  0x0050 /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_DURATION                                                          \
  0 /**< Timout when scanning in 10ms. 0x0000 disables timeout. */
#define SCAN_DURATION_WITELIST                                                 \
  0 /**< Duration of the scanning in units of 10 milliseconds. */

#ifndef BLE_HIDS_C_MIN_INTERVAL
#define BLE_HIDS_C_MIN_INTERVAL 10
#endif
#ifndef BLE_HIDS_C_MAX_INTERVAL
#define BLE_HIDS_C_MAX_INTERVAL 75
#endif
#if BLE_HIDS_C_MIN_INTERVAL > BLE_HIDS_C_MAX_INTERVAL
#error "MIN_INTERVAL should be larger than MAX_INTERVAL"
#endif
#ifndef BLE_HIDS_C_SLAVE_LATENCY
#define BLE_HIDS_C_SLAVE_LATENCY 8
#endif
#ifndef BLE_HIDS_C_TIMEOUT
#define BLE_HIDS_C_TIMEOUT 1500
#endif

#define MIN_CONNECTION_INTERVAL                                                \
  (uint16_t) MSEC_TO_UNITS(BLE_HIDS_C_MIN_INTERVAL,                            \
                           UNIT_1_25_MS) /**< Determines minimum connection    \
                                            interval in milliseconds. */
#define MAX_CONNECTION_INTERVAL                                                \
  (uint16_t) MSEC_TO_UNITS(BLE_HIDS_C_MAX_INTERVAL,                            \
                           UNIT_1_25_MS) /**< Determines maximum connection    \
                                            interval in milliseconds. */
#define SLAVE_LATENCY BLE_HIDS_C_SLAVE_LATENCY
#define SUPERVISION_TIMEOUT                                                    \
  (uint16_t) MSEC_TO_UNITS(BLE_HIDS_C_TIMEOUT,                                 \
                           UNIT_10_MS) /**< Determines supervision time-out in \
                                          units of 10 milliseconds. */

bool m_whitelist_disabled;

/**
 * @brief Parameters used when scanning.
 */
static ble_gap_scan_params_t m_scan_params = {
    .active = 1,
    .interval = SCAN_INTERVAL,
    .window = SCAN_WINDOW,
    .timeout = SCAN_DURATION,
    .scan_phys = BLE_GAP_PHY_1MBPS,
    .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
};

/**@brief Connection parameters requested for connection. */
static const ble_gap_conn_params_t m_connection_param = {
    (uint16_t)MIN_CONNECTION_INTERVAL, (uint16_t)MAX_CONNECTION_INTERVAL,
    SLAVE_LATENCY, (uint16_t)SUPERVISION_TIMEOUT};

NRF_BLE_HIDS_C_ARRAY_DEF(m_hids_gatt, 5);
BLE_DB_DISCOVERY_DEF(m_db_disc);

static uint8_t
    m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_MIN]; /**< buffer where advertising
                                                    reports will be stored by
                                                    the SoftDevice. */

/**@brief Pointer to the buffer where advertising reports will be stored by the
 * SoftDevice. */
static ble_data_t m_scan_buffer = {m_scan_buffer_data, BLE_GAP_SCAN_BUFFER_MIN};

#define UUID16_SIZE 2   /**< Size of 16 bit UUID */
#define UUID32_SIZE 4   /**< Size of 32 bit UUID */
#define UUID128_SIZE 16 /**< Size of 128 bit UUID */

/**@brief Macro to unpack 16bit unsigned UUID from an octet stream. */
#define UUID16_EXTRACT(DST, SRC)                                               \
  do {                                                                         \
    (*(DST)) = (SRC)[1];                                                       \
    (*(DST)) <<= 8;                                                            \
    (*(DST)) |= (SRC)[0];                                                      \
  } while (0)

/**@brief Variable length data encapsulation in terms of length and pointer to
 * data. */
typedef struct {
  uint8_t *p_data;   /**< Pointer to data. */
  uint16_t data_len; /**< Length of data. */
} data_t;

void scan_start_wo_whitelist(void) {
  ret_code_t err_code;

  if (ble_conn_state_central_conn_count() >= NRF_SDH_BLE_CENTRAL_LINK_COUNT) {
    NRF_LOG_ERROR("Connections exceed limit.");
    return;
  }

  NRF_LOG_DEBUG("Chekck fstorage...");
  if (!nrf_fstorage_is_busy(NULL)) {
    (void)sd_ble_gap_scan_stop();

    m_scan_params.active = 0;
    m_scan_params.interval = SCAN_INTERVAL;
    m_scan_params.window = SCAN_WINDOW;
    // Don't use whitelist.
    m_scan_params.timeout = SCAN_DURATION;
    m_scan_params.scan_phys = BLE_GAP_PHY_1MBPS;
    m_scan_params.filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL;

    err_code = sd_ble_gap_scan_start(&m_scan_params, &m_scan_buffer);
    // It is okay to ignore this error since we are stopping the scan anyway.
    if (err_code != NRF_SUCCESS) {
      NRF_LOG_ERROR("scan start error: %d", err_code);
      //      APP_ERROR_CHECK(err_code);
    } else {
      NRF_LOG_INFO("Scanning slave keyboard (wl:0)...");
    }
  } else {
    NRF_LOG_INFO("Fstorage is busy");
  }
}

/**@brief Function for initiating scanning.
 */
void scan_start(void) {
  ret_code_t err_code;

  if (ble_conn_state_central_conn_count() >= NRF_SDH_BLE_CENTRAL_LINK_COUNT) {
    NRF_LOG_ERROR("Connections exceed limit.");
    return;
  }

  NRF_LOG_DEBUG("Chekck fstorage...");
  if (!nrf_fstorage_is_busy(NULL)) {
    (void)sd_ble_gap_scan_stop();

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
    err_code =
        pm_whitelist_get(whitelist_addrs, &addr_cnt, whitelist_irks, &irk_cnt);

    m_scan_params.active = 0;
    m_scan_params.interval = SCAN_INTERVAL;
    m_scan_params.window = SCAN_WINDOW;

    if (!peer_id_is_allocated(0)) {
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
      NRF_LOG_INFO("Scanning slave keyboard (wl:%d)...",
                   m_whitelist_disabled ? 0 : 1);
    }
  } else {
    NRF_LOG_INFO("Fstorage is busy");
  }
}

/**@brief Function for handling the advertising report BLE event.
 *
 * @param[in] p_adv_report  Advertising report from the SoftDevice.
 */
static void on_adv_report(ble_gap_evt_adv_report_t const *p_adv_report) {
  ret_code_t err_code;

  // if (ble_advdata_name_find(p_adv_report->data.p_data,
  // p_adv_report->data.len,
  //     m_target_periph_name)) {

  uint16_t data_offset = 0;
  uint16_t parsed_name_len =
      ble_advdata_search(p_adv_report->data.p_data, p_adv_report->data.len,
                         &data_offset, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME);

  uint8_t const *p_parsed_name = &p_adv_report->data.p_data[data_offset];
  char adv_name[64] = {0};
  memcpy(adv_name, p_parsed_name, parsed_name_len);

  if (adv_name[0] != '\0') {
    NRF_LOG_INFO("adv:%s", adv_name);
  }

  uint16_t target_appearance = BLE_APPEARANCE_GENERIC_HID;

  if (ble_advdata_appearance_find(p_adv_report->data.p_data,
                                  p_adv_report->data.len, &target_appearance)) {
    err_code = sd_ble_gap_connect(&p_adv_report->peer_addr, &m_scan_params,
                                  &m_connection_param, APP_BLE_CONN_CFG_TAG);

    if (err_code == NRF_SUCCESS) {
      NRF_LOG_INFO(
          "Connecting to target %02x%02x%02x%02x%02x%02x",
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
// static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
static void on_ble_central_conn_evt(const ble_evt_t *const p_ble_evt) {
  ret_code_t err_code;
  ble_gap_evt_t const *p_gap_evt = &p_ble_evt->evt.gap_evt;

  switch (p_ble_evt->header.evt_id) {
  case BLE_GAP_EVT_ADV_REPORT:
    on_adv_report(&p_gap_evt->params.adv_report);
    break; // BLE_GAP_EVT_ADV_REPORT

  case BLE_GAP_EVT_CONNECTED:
    if (p_ble_evt->evt.gap_evt.conn_handle < NRF_SDH_BLE_CENTRAL_LINK_COUNT) {
      NRF_LOG_INFO("Connected to the slave keyboard");
    } else {
      return;
    }

    err_code = nrf_ble_hids_c_handles_assign(
        &m_hids_gatt[p_ble_evt->evt.gap_evt.conn_handle],
        p_ble_evt->evt.gap_evt.conn_handle, NULL);
    APP_ERROR_CHECK(err_code);

    err_code =
        ble_db_discovery_start(&m_db_disc, p_ble_evt->evt.gap_evt.conn_handle);
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

  case BLE_GAP_EVT_DISCONNECTED:
    NRF_LOG_INFO("Slave keyboard disconencted");
    scan_start();
    break;

  case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
    // Accepting parameters requested by peer.
    err_code = sd_ble_gap_conn_param_update(
        p_gap_evt->conn_handle,
        &p_gap_evt->params.conn_param_update_request.conn_params);
    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GAP_EVT_PHY_UPDATE_REQUEST: {
    NRF_LOG_INFO("PHY update request.");
    ble_gap_phys_t const phys = {
        .rx_phys = BLE_GAP_PHY_AUTO,
        .tx_phys = BLE_GAP_PHY_AUTO,
    };
    err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
    APP_ERROR_CHECK(err_code);
  } break;

  case BLE_GATTC_EVT_TIMEOUT:
    // Disconnect on GATT Client timeout event.
    NRF_LOG_DEBUG("GATT Client Timeout.");
    err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                     BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GATTS_EVT_TIMEOUT:
    // Disconnect on GATT Server timeout event.
    NRF_LOG_INFO("GATT Server Timeout.");
    err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                     BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
    break;

  default:
    break;
  }
}

void on_ble_central_evt(const ble_evt_t *const p_ble_evt,
                        uint16_t conn_handle) {
  /** on_ble_central_evt will update the connection handles, so we want to
   * execute it after dispatching to the central applications upon
   * disconnection. */
  if (p_ble_evt->header.evt_id != BLE_GAP_EVT_DISCONNECTED) {
    on_ble_central_conn_evt(p_ble_evt);
  }

  // insert dispatching to the central application

  // If the peer disconnected, we update the connection handles last.
  if (p_ble_evt->header.evt_id == BLE_GAP_EVT_DISCONNECTED) {
    on_ble_central_conn_evt(p_ble_evt);
  }
}

/**@brief Function for handling database discovery events.
 *
 * @details This function is callback function to handle events from the
 * database discovery module. Depending on the UUIDs that are discovered, this
 * function should forward the events to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t *p_evt) {
  nrf_ble_hids_c_on_db_disc_evt(&m_hids_gatt[p_evt->conn_handle], p_evt);
}

/**
 * @brief Database discovery initialization.
 */
void db_discovery_init(void) {
  ret_code_t err_code = ble_db_discovery_init(db_disc_handler);
  APP_ERROR_CHECK(err_code);
}

void hids_c_evt_handler(nrf_ble_hids_c_evt_t *p_evt) {
  switch (p_evt->evt_type) {
  case NRF_BLE_HIDS_C_EVT_DISCOVERY_COMPLETE:
    if (p_evt->conn_handle < NRF_SDH_BLE_CENTRAL_LINK_COUNT) {
      ret_code_t err_code = nrf_ble_hids_c_handles_assign(
          &m_hids_gatt[p_evt->conn_handle], p_evt->conn_handle, &p_evt->params);
      APP_ERROR_CHECK(err_code);

      NRF_LOG_INFO("hids_discovery_complete:handle:%d", p_evt->conn_handle);

      // Initiate bonding.
      err_code = pm_conn_secure(p_evt->conn_handle, false);
      if (err_code != NRF_ERROR_INVALID_STATE) {
        APP_ERROR_CHECK(err_code);
      } else {
        NRF_LOG_INFO("pm_conn_secure:INVALID_STATE");
      }

      for (int idx = 0;
           idx < m_hids_gatt[p_evt->conn_handle].hids_chars.report_count;
           idx++) {
        err_code =
            ble_hids_c_read_report(&m_hids_gatt[p_evt->conn_handle], idx);
      }

      err_code = nrf_ble_hids_c_enable_notification(
          &m_hids_gatt[p_evt->conn_handle], true);
      if (err_code != NRF_SUCCESS) {
        NRF_LOG_ERROR("Failed to enable hids notifications");
      }

      if (ble_conn_state_central_conn_count() <
          NRF_SDH_BLE_CENTRAL_LINK_COUNT) {
        scan_start();
      }
    }

    break;

  case NRF_BLE_HIDS_C_EVT_DISCONN_COMPLETE:
    break;

  default:
    break;
  }
  NRF_LOG_INFO("hids_c_evt_handler");
}

void hids_error_handler(uint32_t nrf_error) {
  NRF_LOG_INFO("hids_c_error_handler:%d", nrf_error);
}

__attribute__((weak)) void hids_c_on_disconnect() {}

__attribute__((weak)) void hids_c_data_handler(uint8_t const *dat, uint16_t len,
                                               uint16_t conn_handle,
                                               uint16_t report_idx) {
  NRF_LOG_INFO("report idx:%d, len:%d", report_idx, len);
}

void ble_hids_c_init(void) {
  nrf_ble_hids_c_init_t init_config = {hids_c_evt_handler, hids_error_handler,
                                       hids_c_data_handler};

  for (int idx = 0; idx < ARRAY_SIZE(m_hids_gatt); idx++) {
    nrf_ble_hids_c_init(&m_hids_gatt[idx], &init_config);
  }
}
