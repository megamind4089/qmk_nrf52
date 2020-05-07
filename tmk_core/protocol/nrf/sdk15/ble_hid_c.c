

#include "ble_hid_c.h"
#include "ble.h"
#include <string.h>

#define NRF_LOG_MODULE_NAME nrf_ble_hids_c
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

#define HIDS_LOG                                                               \
  NRF_LOG_INFO /**< A debug logger macro that can be used in this file to      \
                  perform logging over UART. */
#define MODULE_INITIALIZED                                                     \
  (p_hids_c->initialized) /**< Macro indicating whether the module has been    \
                             initialized properly. */

static const ble_uuid_t m_gatts_uuid = {
    BLE_UUID_GATT, BLE_UUID_TYPE_BLE}; /**< Service Changed indication UUID. */
static const ble_uuid_t m_dis_uuid = {BLE_UUID_DEVICE_INFORMATION_SERVICE,
                                      BLE_UUID_TYPE_BLE};
static const ble_uuid_t m_bas_uuid = {BLE_UUID_BATTERY_SERVICE,
                                      BLE_UUID_TYPE_BLE};
static const ble_uuid_t m_hids_uuid = {BLE_UUID_HUMAN_INTERFACE_DEVICE_SERVICE,
                                       BLE_UUID_TYPE_BLE};

#define BLE_UUID_HIDS_CHARACTERISTIC_BOOT_KEYBOARD 0x2A32
#define BLE_UUID_HIDS_CHARACTERISTIC_BOOT_MOUSE 0x2A33
#define BLE_UUID_HIDS_CHARACTERISTIC_REPORT 0x2A4D

#define TX_BUFFER_MASK                                                         \
  0x0F /**< TX Buffer mask, must be a mask of contiguous zeroes, followed by   \
          contiguous sequence of ones: 000...111. */
#define TX_BUFFER_SIZE                                                         \
  (TX_BUFFER_MASK +                                                            \
   1) /**< Size of the send buffer, which is 1 higher than the mask. */
#define WRITE_MESSAGE_LENGTH                                                   \
  BLE_CCCD_VALUE_LEN /**< Length of the write message for CCCD. */

typedef enum {
  READ_REQ, /**< Type identifying that this tx_message is a read request. */
  WRITE_REQ /**< Type identifying that this tx_message is a write request. */
} tx_request_t;

/**@brief Structure for writing a message to the peer, i.e. CCCD.
 */
typedef struct {
  uint8_t gattc_value[WRITE_MESSAGE_LENGTH]; /**< The message to write. */
  ble_gattc_write_params_t
      gattc_params; /**< The GATTC parameters for this message. */
} write_params_t;

/**@brief Structure for holding the data that will be transmitted to the
 * connected central.
 */
typedef struct {
  uint16_t conn_handle; /**< Connection handle to be used when transmitting this
                           message. */
  tx_request_t type;    /**< Type of message. (read or write). */
  union {
    uint16_t read_handle;     /**< Read request handle. */
    write_params_t write_req; /**< Write request message. */
  } req;
} tx_message_t;

static tx_message_t
    m_tx_buffer[TX_BUFFER_SIZE]; /**< Transmit buffer for the messages that will
                                    be transmitted to the central. */
static uint32_t m_tx_insert_index =
    0; /**< Current index in the transmit buffer where the next message should
          be inserted. */
static uint32_t m_tx_index =
    0; /**< Current index in the transmit buffer containing the next message to
          be transmitted. */

/**@brief Function for passing any pending request from the buffer to the stack.
 */
static void tx_buffer_process(void) {
  if (m_tx_index != m_tx_insert_index) {
    uint32_t err_code;

    if (m_tx_buffer[m_tx_index].type == READ_REQ) {
      err_code = sd_ble_gattc_read(m_tx_buffer[m_tx_index].conn_handle,
                                   m_tx_buffer[m_tx_index].req.read_handle, 0);
    } else {
      err_code = sd_ble_gattc_write(
          m_tx_buffer[m_tx_index].conn_handle,
          &m_tx_buffer[m_tx_index].req.write_req.gattc_params);
    }
    if (err_code == NRF_SUCCESS) {
      NRF_LOG_DEBUG("SD Read/Write API returns Success..");
      m_tx_index++;
      m_tx_index &= TX_BUFFER_MASK;
    } else {
      NRF_LOG_DEBUG(
          "SD Read/Write API returns error. This message sending will be "
          "attempted again..");
    }
  }
}

/**@brief Function for handling the indication and notifications from the HID
   Service Server.

   @param[in] p_hids_c       Pointer to Service Changed client structure.
   @param[in] p_ble_hidc_evt Pointer to a hidc event.
*/
static void on_hvx(nrf_ble_hids_c_t const *const p_hids_c,
                   ble_gattc_evt_t const *const p_ble_gattc_evt) {
  uint16_t idx = 0;

  if (p_hids_c->conn_handle != p_ble_gattc_evt->conn_handle) {
    return;
  }

  for (idx = 0; idx < p_hids_c->hids_chars.report_count; idx++) {
    if (p_ble_gattc_evt->params.hvx.handle ==
        p_hids_c->hids_chars.report[idx].characteristic.handle_value) {
      break;
    }
  }

  if (p_hids_c->data_handler != NULL &&
      idx != p_hids_c->hids_chars.report_count) {
    NRF_LOG_DEBUG("handle %d received:%d bytes", p_hids_c->conn_handle,
                  p_ble_gattc_evt->params.hvx.len);
    p_hids_c->data_handler(p_ble_gattc_evt->params.hvx.data,
                           p_ble_gattc_evt->params.hvx.len,
                           p_ble_gattc_evt->conn_handle, idx);
  }

  // NRF_LOG_INFO("receive:%d bytes", p_ble_gattc_evt->params.hvx.len);
  // for (int idx = 0; idx < p_ble_gattc_evt->params.hvx.len; idx++) {
  //   NRF_LOG_INFO("%d", p_ble_gattc_evt->params.hvx.data[idx]);
  // }
}

ret_code_t nrf_ble_hids_c_init(nrf_ble_hids_c_t *p_hids_c,
                               nrf_ble_hids_c_init_t *p_hids_c_init) {
  ret_code_t err_code = NRF_SUCCESS;
  VERIFY_PARAM_NOT_NULL(p_hids_c);
  VERIFY_PARAM_NOT_NULL(p_hids_c_init);
  VERIFY_PARAM_NOT_NULL(p_hids_c_init->evt_handler);
  VERIFY_PARAM_NOT_NULL(p_hids_c_init->data_handler);

  memset(p_hids_c, 0, sizeof(nrf_ble_hids_c_t));

  p_hids_c->conn_handle = BLE_CONN_HANDLE_INVALID;
  p_hids_c->evt_handler = p_hids_c_init->evt_handler;
  p_hids_c->data_handler = p_hids_c_init->data_handler;

  err_code = ble_db_discovery_evt_register(&m_gatts_uuid);
  VERIFY_SUCCESS(err_code);

  err_code = ble_db_discovery_evt_register(&m_bas_uuid);
  VERIFY_SUCCESS(err_code);

  err_code = ble_db_discovery_evt_register(&m_dis_uuid);
  VERIFY_SUCCESS(err_code);

  err_code = ble_db_discovery_evt_register(&m_hids_uuid);
  VERIFY_SUCCESS(err_code);

  p_hids_c->initialized = true;
  return err_code;
}

static bool hids_hid_handles_are_valid(nrf_ble_hids_c_t const *const p_hids_c) {
  return (p_hids_c->conn_handle != BLE_CONN_HANDLE_INVALID);
}

void nrf_ble_hids_c_on_db_disc_evt(nrf_ble_hids_c_t const *const p_hids_c,
                                   ble_db_discovery_evt_t *const p_evt) {
  VERIFY_MODULE_INITIALIZED_VOID();

  nrf_ble_hids_c_evt_t evt = {0};
  ble_gatt_db_char_t *p_chars;

  p_chars = p_evt->params.discovered_db.charateristics;
  evt.evt_type = NRF_BLE_HIDS_C_EVT_DISCOVERY_FAILED;

  HIDS_LOG("evt_type:%d, uuid:0x%04x, type:%d, char:%d", p_evt->evt_type,
           p_evt->params.discovered_db.srv_uuid.uuid,
           p_evt->params.discovered_db.srv_uuid.type,
           p_evt->params.discovered_db.char_count);

  evt.evt_type = NRF_BLE_HIDS_C_EVT_DISCONN_COMPLETE;

  if ((p_evt->evt_type == BLE_DB_DISCOVERY_COMPLETE) &&
      (p_evt->params.discovered_db.srv_uuid.uuid ==
       BLE_UUID_HUMAN_INTERFACE_DEVICE_SERVICE) &&
      (p_evt->params.discovered_db.srv_uuid.type == BLE_UUID_TYPE_BLE)) {
    // Find the handles of the hids Characteristics.
    for (uint32_t i = 0; i < p_evt->params.discovered_db.char_count; i++) {
      HIDS_LOG("uuid:0x%04x", p_chars[i].characteristic.uuid.uuid);
      if ((p_chars[i].characteristic.uuid.uuid ==
           BLE_UUID_HIDS_CHARACTERISTIC_REPORT) &&
          (p_chars[i].characteristic.char_props.notify != 0) &&
          evt.params.report_count <
              sizeof(evt.params.report) / sizeof(evt.params.report[0])) {

        memcpy(&evt.params.report[evt.params.report_count++], &p_chars[i],
               sizeof(ble_gatt_db_char_t));

        evt.evt_type = NRF_BLE_HIDS_C_EVT_DISCOVERY_COMPLETE;
        HIDS_LOG("HIDs report characteristic found.\n\r");
      } else if (p_chars[i].characteristic.uuid.uuid ==
                 BLE_UUID_HIDS_CHARACTERISTIC_BOOT_KEYBOARD) {
        memcpy(&evt.params.boot_keyboard, &p_chars[i],
               sizeof(ble_gatt_db_char_t));

        evt.evt_type = NRF_BLE_HIDS_C_EVT_DISCOVERY_COMPLETE;
        HIDS_LOG("HIDs boot keyboard characteristic found.\n\r");
      } else if (p_chars[i].characteristic.uuid.uuid ==
                 BLE_UUID_HIDS_CHARACTERISTIC_BOOT_MOUSE) {
        memcpy(&evt.params.boot_mouse, &p_chars[i], sizeof(ble_gatt_db_char_t));

        evt.evt_type = NRF_BLE_HIDS_C_EVT_DISCOVERY_COMPLETE;
        HIDS_LOG("HIDs boot mouse characteristic found.\n\r");
      }
    }
  }

  if (p_hids_c->evt_handler != NULL) {
    nrf_ble_hids_c_evt_handler_t evt_handler = p_hids_c->evt_handler;
    evt.conn_handle = p_evt->conn_handle;
    evt_handler(&evt);
  }
}

static void on_disconnect(nrf_ble_hids_c_t *p_hids_c,
                          ble_evt_t const *p_ble_evt) {
  if (p_hids_c->conn_handle == p_ble_evt->evt.gap_evt.conn_handle) {
    p_hids_c->conn_handle = BLE_CONN_HANDLE_INVALID;

    if (hids_hid_handles_are_valid(p_hids_c)) {
      nrf_ble_hids_c_evt_t evt;

      evt.evt_type = NRF_BLE_HIDS_C_EVT_DISCONN_COMPLETE;

      p_hids_c->evt_handler(&evt);

      for (int idx = 0; idx < p_hids_c->hids_chars.report_count; idx++) {
        p_hids_c->hids_chars.report[idx].characteristic.handle_value =
            BLE_CONN_HANDLE_INVALID;
        p_hids_c->hids_chars.report[idx].cccd_handle = BLE_CONN_HANDLE_INVALID;
        p_hids_c->hids_chars.report[idx].ext_prop_handle =
            BLE_CONN_HANDLE_INVALID;
        p_hids_c->hids_chars.report[idx].report_ref_handle =
            BLE_CONN_HANDLE_INVALID;
        p_hids_c->hids_chars.report[idx].user_desc_handle =
            BLE_CONN_HANDLE_INVALID;
      }
    }
  }
}

static uint32_t cccd_configure(uint16_t conn_handle, uint16_t handle_cccd,
                               bool notification_enable) {
  NRF_LOG_DEBUG("Configuring CCCD. CCCD Handle = %d, Connection Handle = %d",
                handle_cccd, conn_handle);

  tx_message_t *p_msg;
  uint16_t cccd_val = notification_enable ? BLE_GATT_HVX_NOTIFICATION : 0;

  p_msg = &m_tx_buffer[m_tx_insert_index++];
  m_tx_insert_index &= TX_BUFFER_MASK;

  p_msg->req.write_req.gattc_params.handle = handle_cccd;
  p_msg->req.write_req.gattc_params.len = WRITE_MESSAGE_LENGTH;
  p_msg->req.write_req.gattc_params.p_value = p_msg->req.write_req.gattc_value;
  p_msg->req.write_req.gattc_params.offset = 0;
  p_msg->req.write_req.gattc_params.write_op = BLE_GATT_OP_WRITE_REQ;
  p_msg->req.write_req.gattc_value[0] = LSB_16(cccd_val);
  p_msg->req.write_req.gattc_value[1] = MSB_16(cccd_val);
  p_msg->conn_handle = conn_handle;
  p_msg->type = WRITE_REQ;

  tx_buffer_process();
  return NRF_SUCCESS;
}

void nrf_ble_hids_c_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context) {
  nrf_ble_hids_c_t *p_hids_c;
  p_hids_c = p_context;

  VERIFY_MODULE_INITIALIZED_VOID();
  VERIFY_PARAM_NOT_NULL_VOID(p_hids_c);
  VERIFY_PARAM_NOT_NULL_VOID(p_ble_evt);

  switch (p_ble_evt->header.evt_id) {
  case BLE_GAP_EVT_DISCONNECTED:
    on_disconnect(p_hids_c, p_ble_evt);
    break;

  case BLE_GATTC_EVT_HVX:
    NRF_LOG_DEBUG("hidc_evt_hvx");
    on_hvx(p_hids_c, &(p_ble_evt->evt.gattc_evt));
    break;

  case BLE_GATTC_EVT_WRITE_RSP:
    tx_buffer_process();
    break;

  case BLE_GATTC_EVT_READ_RSP:
    tx_buffer_process();
    break;

  default:
    break;
  }
}

ret_code_t nrf_ble_hids_c_handles_assign(nrf_ble_hids_c_t *const p_hids_c,
                                         uint16_t const conn_handle,
                                         hids_chars_t const *p_hids_chars) {
  VERIFY_PARAM_NOT_NULL(p_hids_c);
  VERIFY_MODULE_INITIALIZED();

  p_hids_c->conn_handle = conn_handle;
  if (p_hids_chars != NULL) {
    memcpy(&p_hids_c->hids_chars, p_hids_chars, sizeof(*p_hids_chars));
  }
  return NRF_SUCCESS;
}

ret_code_t nrf_ble_hids_c_enable_notification(nrf_ble_hids_c_t *const p_hids_c,
                                              bool const enable) {
  VERIFY_MODULE_INITIALIZED();

  if (p_hids_c->conn_handle == BLE_CONN_HANDLE_INVALID) {
    NRF_LOG_INFO("handle invalid");
    return NRF_ERROR_INVALID_STATE;
  }

  ret_code_t err_code = NRF_SUCCESS;

  for (int idx = 0; idx < p_hids_c->hids_chars.report_count; idx++) {
    err_code =
        cccd_configure(p_hids_c->conn_handle,
                       p_hids_c->hids_chars.report[idx].cccd_handle, enable);

    if (err_code == NRF_SUCCESS) {
      NRF_LOG_INFO("Enable notifications");
      p_hids_c->notify_enabled[idx] = true;
    } else {
      NRF_LOG_INFO("Failed to enable notifications:%d", err_code);
    }
  }

  return err_code;
}

ret_code_t ble_hids_c_read_report(nrf_ble_hids_c_t const *const p_hids_c,
                                  uint8_t report_idx) {
  VERIFY_MODULE_INITIALIZED();

  if (p_hids_c->conn_handle == BLE_CONN_HANDLE_INVALID) {
    NRF_LOG_INFO("handle invalid");
    return NRF_ERROR_INVALID_STATE;
  }

  if (report_idx >= p_hids_c->hids_chars.report_count) {
    NRF_LOG_INFO("invalid idx");
    return NRF_ERROR_INVALID_PARAM;
  }

  tx_message_t *msg;

  msg = &m_tx_buffer[m_tx_insert_index++];
  m_tx_insert_index &= TX_BUFFER_MASK;

  msg->req.read_handle =
      p_hids_c->hids_chars.report[report_idx].characteristic.handle_value;
  msg->conn_handle = p_hids_c->conn_handle;
  msg->type = READ_REQ;

  tx_buffer_process();
  return NRF_SUCCESS;
}
