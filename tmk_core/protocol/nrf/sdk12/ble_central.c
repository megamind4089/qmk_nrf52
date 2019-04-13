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

#include "ble_nus_c.h"
#include "ble_gap.h"
#include "softdevice_handler_appsh.h"
#include "fstorage.h"

//#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#if (NRF_SD_BLE_API_VERSION == 3)
#define NRF_BLE_MAX_MTU_SIZE        GATT_MTU_SIZE_DEFAULT                         /**< MTU size used in the softdevice enabling and to reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event. */
#endif

#define SCAN_INTERVAL               0x0100                                        /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                 0x0050                                        /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_TIMEOUT                0

#ifndef BLE_NUS_MIN_INTERVAL
  #define BLE_NUS_MIN_INTERVAL 30
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

#define MIN_CONNECTION_INTERVAL     (uint16_t) MSEC_TO_UNITS(BLE_NUS_MIN_INTERVAL, UNIT_1_25_MS)   /**< Determines minimum connection interval in milliseconds. */
#define MAX_CONNECTION_INTERVAL     (uint16_t) MSEC_TO_UNITS(BLE_NUS_MAX_INTERVAL, UNIT_1_25_MS)    /**< Determines maximum connection interval in milliseconds. */
#define SLAVE_LATENCY               BLE_NUS_SLAVE_LATENCY
#define SUPERVISION_TIMEOUT         (uint16_t) MSEC_TO_UNITS(BLE_NUS_TIMEOUT, UNIT_10_MS)    /**< Determines supervision time-out in units of 10 milliseconds. */

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
    .timeout  = SCAN_TIMEOUT,
    #if (NRF_SD_BLE_API_VERSION == 2)
        .selective   = 0,
        .p_whitelist = NULL,
    #endif
    #if (NRF_SD_BLE_API_VERSION == 3)
        .use_whitelist = 0,
    #endif
};

// Whitelist buffers.
//static bool m_whitelist_disabled;
static ble_gap_addr_t whitelist_addrs[8];
static ble_gap_irk_t whitelist_irks[8];
static ble_gap_addr_t *p_whitelist_addrs = whitelist_addrs;
static ble_gap_irk_t *p_whitelist_irks = whitelist_irks;
#if (NRF_SD_BLE_API_VERSION == 2)
static ble_gap_whitelist_t whitelist = {
    .pp_addrs = &p_whitelist_addrs,
    .addr_count = 0,
    .pp_irks = &p_whitelist_irks,
    .irk_count = 0,
};
#else
typedef struct {
  ble_gap_addr_t** pp_addrs;
  uint8_t addr_count;
  ble_gap_irk_t** pp_irks;
  uint8_t irk_count;
} whitelist_t;
static whitelist_t whitelist = {
    .pp_addrs = &p_whitelist_addrs,
    .addr_count = 0,
    .pp_irks = &p_whitelist_irks,
    .irk_count = 0,
};
#endif

/**@brief Connection parameters requested for connection. */
static const ble_gap_conn_params_t m_connection_param =
{
    (uint16_t)MIN_CONNECTION_INTERVAL,
    (uint16_t)MAX_CONNECTION_INTERVAL,
    0,
    (uint16_t)SUPERVISION_TIMEOUT
};

/**
 * @brief NUS uuid
 */
//static const ble_uuid_t m_nus_uuid =
//  {
//    .uuid = BLE_UUID_NUS_SERVICE,
//    .type = BLE_UUID_TYPE_VENDOR_BEGIN
//  };

static ble_nus_c_t              m_ble_nus_c;                    /**< Instance of NUS service. Must be passed to all NUS_C API calls. */
static uint16_t           m_conn_handle_nus_c            = BLE_CONN_HANDLE_INVALID;       /**< Connection handle for the NUS central application */

static ble_db_discovery_t m_ble_db_discovery[CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT]; /**< list of DB structures used by the database discovery module. */

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
typedef struct
{
    uint8_t     * p_data;    /**< Pointer to data. */
    uint16_t      data_len;  /**< Length of data. */
} data_t;

/**
 * @brief Parses advertisement data, providing length and location of the field in case
 *        matching data is found.
 *
 * @param[in]  Type of data to be looked for in advertisement data.
 * @param[in]  Advertisement report length and pointer to report.
 * @param[out] If data type requested is found in the data report, type data length and
 *             pointer to data will be populated here.
 *
 * @retval NRF_SUCCESS if the data type is found in the report.
 * @retval NRF_ERROR_NOT_FOUND if the data type could not be found.
 */
static uint32_t adv_report_parse(uint8_t type, data_t * p_advdata, data_t * p_typedata)
{
    uint32_t  index = 0;
    uint8_t * p_data;

    p_data = p_advdata->p_data;

    while (index < p_advdata->data_len)
    {
        uint8_t field_length = p_data[index];
        uint8_t field_type   = p_data[index + 1];

        if (field_type == type)
        {
            p_typedata->p_data   = &p_data[index + 2];
            p_typedata->data_len = field_length - 1;
            return NRF_SUCCESS;
        }
        index += field_length + 1;
    }
    return NRF_ERROR_NOT_FOUND;
}


/**@brief Function for initiating scanning.
 */
void scan_start(void)
{
    ret_code_t err_code;

//    if (!nrf_fstorage_is_busy(NULL)) {
        (void) sd_ble_gap_scan_stop();

        memset(whitelist_addrs, 0x00, sizeof(whitelist_addrs));
        memset(whitelist_irks, 0x00, sizeof(whitelist_irks));

//        uint32_t addr_cnt = (sizeof(whitelist_addrs) / sizeof(ble_gap_addr_t));
//        uint32_t irk_cnt = (sizeof(whitelist_irks) / sizeof(ble_gap_irk_t));
        whitelist.addr_count = (sizeof(whitelist_addrs) / sizeof(ble_gap_addr_t));
        whitelist.irk_count = (sizeof(whitelist_irks) / sizeof(ble_gap_irk_t));

        // Reload the whitelist and whitelist all peers.
        //    whitelist_load();

        // Get the whitelist previously set using pm_whitelist_set().
        err_code = pm_whitelist_get(whitelist_addrs, (uint32_t*)&whitelist.addr_count, whitelist_irks,
            (uint32_t*)&whitelist.irk_count);

        m_scan_params.active = 0;
        m_scan_params.interval = SCAN_INTERVAL;
        m_scan_params.window = SCAN_WINDOW;

        if (((whitelist.addr_count == 0) && (whitelist.irk_count == 0))) {
          // Don't use whitelist.
          m_scan_params.timeout = SCAN_TIMEOUT;
#if (NRF_SD_BLE_API_VERSION == 2)
          m_scan_params.selective = 0;
          m_scan_params.p_whitelist = NULL;
#endif
#if (NRF_SD_BLE_API_VERSION == 3)
          m_scan_params.use_whitelist = 0;
#endif
        } else {
          // Use whitelist.
          m_scan_params.timeout = SCAN_TIMEOUT;
#if (NRF_SD_BLE_API_VERSION == 2)
          m_scan_params.selective = 1;
          m_scan_params.p_whitelist = &whitelist;
#endif
#if (NRF_SD_BLE_API_VERSION == 3)
          m_scan_params.use_whitelist = 1;
#endif
        }


        err_code = sd_ble_gap_scan_start(&m_scan_params);
        // It is okay to ignore this error since we are stopping the scan anyway.
        if (err_code != NRF_ERROR_INVALID_STATE)
        {
            APP_ERROR_CHECK(err_code);
        }
//    }
}

__WEAK void ble_nus_packetrcv_handler(ble_switch_state_t* buf, uint8_t len) {

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
static void ble_nus_c_evt_handler(ble_nus_c_t * p_ble_nus_c, const ble_nus_c_evt_t * p_ble_nus_evt)
{
    uint32_t err_code;
    switch (p_ble_nus_evt->evt_type)
    {
        case BLE_NUS_C_EVT_DISCOVERY_COMPLETE:
            err_code = ble_nus_c_handles_assign(p_ble_nus_c, p_ble_nus_evt->conn_handle, &p_ble_nus_evt->handles);
            APP_ERROR_CHECK(err_code);

            // Initiate bonding.
            err_code = pm_conn_secure(p_ble_nus_evt->conn_handle, false);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }

            err_code = ble_nus_c_rx_notif_enable(p_ble_nus_c);
            APP_ERROR_CHECK(err_code);
            NRF_LOG_INFO("The device has the Nordic UART Service\r\n");
            break;

        case BLE_NUS_C_EVT_NUS_RX_EVT:
            NRF_LOG_INFO("NUS:Received'%d\r\n", p_ble_nus_evt->data_len);
            ble_nus_packetrcv_handler((ble_switch_state_t*)p_ble_nus_evt->p_data,
                p_ble_nus_evt->data_len/sizeof(ble_switch_state_t));
            // for (uint32_t i = 0; i < p_ble_nus_evt->data_len; i++)
            // {
            //     while (app_uart_put( p_ble_nus_evt->p_data[i]) != NRF_SUCCESS);
            // }
            break;

        case BLE_NUS_C_EVT_DISCONNECTED:
            NRF_LOG_INFO("NUS:Disconnected\r\n");
            scan_start();
            break;
    }
}


/**@brief Function for searching a given name in the advertisement packets.
 *
 * @details Use this function to parse received advertising data and to find a given
 * name in them either as 'complete_local_name' or as 'short_local_name'.
 *
 * @param[in]   p_adv_report   advertising data to parse.
 * @param[in]   name_to_find   name to search.
 * @return   true if the given name was found, false otherwise.
 */
static bool find_adv_name(const ble_gap_evt_adv_report_t *p_adv_report, const char * name_to_find)
{
    uint32_t err_code;
    data_t   adv_data;
    data_t   dev_name;

    // Initialize advertisement report for parsing
    adv_data.p_data     = (uint8_t *)p_adv_report->data;
    adv_data.data_len   = p_adv_report->dlen;

    //search for advertising names
    err_code = adv_report_parse(BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,
                                &adv_data,
                                &dev_name);
    if (err_code == NRF_SUCCESS)
    {
        if (memcmp(name_to_find, dev_name.p_data, dev_name.data_len )== 0)
        {
            return true;
        }
    }
    else
    {
        // Look for the short local name if it was not found as complete
        err_code = adv_report_parse(BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME,
                                    &adv_data,
                                    &dev_name);
        if (err_code != NRF_SUCCESS)
        {
            return false;
        }
        if (memcmp(m_target_periph_name, dev_name.p_data, dev_name.data_len )== 0)
        {
            return true;
        }
    }
    return false;
}


/**@brief Reads an advertising report and checks if a uuid is present in the service list.
 *
 * @details The function is able to search for 16-bit, 32-bit and 128-bit service uuids.
 *          To see the format of a advertisement packet, see
 *          https://www.bluetooth.org/Technical/AssignedNumbers/generic_access_profile.htm
 *
 * @param[in]   p_target_uuid The uuid to search fir
 * @param[in]   p_adv_report  Pointer to the advertisement report.
 *
 * @retval      true if the UUID is present in the advertisement report. Otherwise false
 */
//static bool is_uuid_present(const ble_uuid_t *p_target_uuid,
//                            const ble_gap_evt_adv_report_t *p_adv_report)
//{
//    uint32_t err_code;
//    uint32_t index = 0;
//    uint8_t *p_data = (uint8_t *)p_adv_report->data;
//    ble_uuid_t extracted_uuid;
//
//    while (index < p_adv_report->dlen)
//    {
//        uint8_t field_length = p_data[index];
//        uint8_t field_type   = p_data[index + 1];
//
//        if ( (field_type == BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE)
//           || (field_type == BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE)
//           )
//        {
//            for (uint32_t u_index = 0; u_index < (field_length / UUID16_SIZE); u_index++)
//            {
//                err_code = sd_ble_uuid_decode(  UUID16_SIZE,
//                                                &p_data[u_index * UUID16_SIZE + index + 2],
//                                                &extracted_uuid);
//                if (err_code == NRF_SUCCESS)
//                {
//                    if ((extracted_uuid.uuid == p_target_uuid->uuid)
//                        && (extracted_uuid.type == p_target_uuid->type))
//                    {
//                        return true;
//                    }
//                }
//            }
//        }
//
//        else if ( (field_type == BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE)
//                || (field_type == BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE)
//                )
//        {
//            for (uint32_t u_index = 0; u_index < (field_length / UUID32_SIZE); u_index++)
//            {
//                err_code = sd_ble_uuid_decode(UUID16_SIZE,
//                &p_data[u_index * UUID32_SIZE + index + 2],
//                &extracted_uuid);
//                if (err_code == NRF_SUCCESS)
//                {
//                    if ((extracted_uuid.uuid == p_target_uuid->uuid)
//                        && (extracted_uuid.type == p_target_uuid->type))
//                    {
//                        return true;
//                    }
//                }
//            }
//        }
//
//        else if ( (field_type == BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE)
//                || (field_type == BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE)
//                )
//        {
//            err_code = sd_ble_uuid_decode(UUID128_SIZE,
//                                          &p_data[index + 2],
//                                          &extracted_uuid);
//            if (err_code == NRF_SUCCESS)
//            {
//                if ((extracted_uuid.uuid == p_target_uuid->uuid)
//                    && (extracted_uuid.type == p_target_uuid->type))
//                {
//                    return true;
//                }
//            }
//        }
//        index += field_length + 1;
//    }
//    return false;
//}


/**@brief Function for handling BLE Stack events concerning central applications.
 *
 * @details This function keeps the connection handles of central applications up-to-date. It
 * parses scanning reports, initiating a connection attempt to peripherals when a target UUID
 * is found, and manages connection parameter update requests. Additionally, it updates the status
 * of LEDs used to report central applications activity.
 *
 * @note        Since this function updates connection handles, @ref BLE_GAP_EVT_DISCONNECTED events
 *              should be dispatched to the target application before invoking this function.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void on_ble_central_conn_evt(const ble_evt_t * const p_ble_evt)
{
    const ble_gap_evt_t   * const p_gap_evt = &p_ble_evt->evt.gap_evt;
    ret_code_t                    err_code;

    switch (p_ble_evt->header.evt_id)
    {
        /** Upon connection, check which peripheral has connected (HR or RSC), initiate DB
         *  discovery, update LEDs status and resume scanning if necessary. */
        case BLE_GAP_EVT_CONNECTED:
        {
            NRF_LOG_INFO("Central Connected \r\n");
            /** If no Heart Rate sensor or RSC sensor is currently connected, try to find them on this peripheral*/
            if (m_conn_handle_nus_c == BLE_CONN_HANDLE_INVALID)
            {
                NRF_LOG_INFO("try to find HRS or RSC on conn_handle 0x%x\r\n", p_gap_evt->conn_handle);

                APP_ERROR_CHECK_BOOL(p_gap_evt->conn_handle < CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT);
                err_code = ble_db_discovery_start(&m_ble_db_discovery[p_gap_evt->conn_handle], p_gap_evt->conn_handle);
                APP_ERROR_CHECK(err_code);
            }

            /** Update LEDs status, and check if we should be looking for more
             *  peripherals to connect to. */
            // bsp_board_led_on(CENTRAL_CONNECTED_LED);
            if (ble_conn_state_n_centrals() == CENTRAL_LINK_COUNT)
            {
                // bsp_board_led_off(CENTRAL_SCANNING_LED);
            }
            else
            {
                // Resume scanning.
                // bsp_board_led_on(CENTRAL_SCANNING_LED);
                scan_start();
            }
        } break; // BLE_GAP_EVT_CONNECTED

        /** Upon disconnection, reset the connection handle of the peer which disconnected, update
         * the LEDs status and start scanning again. */
        case BLE_GAP_EVT_DISCONNECTED:
        {
            ble_nus_on_disconnect();
            uint8_t n_centrals;

            if (p_gap_evt->conn_handle == m_conn_handle_nus_c)
            {
                NRF_LOG_INFO("HRS central disconnected (reason: %d)\r\n",
                       p_gap_evt->params.disconnected.reason);

                m_conn_handle_nus_c = BLE_CONN_HANDLE_INVALID;
            }
            if ((m_conn_handle_nus_c == BLE_CONN_HANDLE_INVALID))
            {
                // Start scanning
                scan_start();

                // Update LEDs status.
                // bsp_board_led_on(CENTRAL_SCANNING_LED);
            }
            n_centrals = ble_conn_state_n_centrals();

            if (n_centrals == 0)
            {
                // bsp_board_led_off(CENTRAL_CONNECTED_LED);
            }
        } break; // BLE_GAP_EVT_DISCONNECTED

        case BLE_GAP_EVT_ADV_REPORT:
        {
            if (strlen(m_target_periph_name) != 0)
            {
                if (find_adv_name(&p_gap_evt->params.adv_report, m_target_periph_name))
                {
                    // Initiate connection.
                  m_scan_params.use_whitelist = 0;
                    err_code = sd_ble_gap_connect(&p_gap_evt->params.adv_report.peer_addr,
                                                  &m_scan_params,
                                                  &m_connection_param);
                    if (err_code != NRF_SUCCESS)
                    {
                        NRF_LOG_INFO("Connection Request Failed, reason %d\r\n", err_code);
                    }
                }
            }
            else
            {
//               /** We do not want to connect to two peripherals offering the same service, so when
//                *  a UUID is matched, we check that we are not already connected to a peer which
//                *  offers the same service. */
//                if (is_uuid_present(&m_nus_uuid, &p_gap_evt->params.adv_report)&&
//                     (m_conn_handle_nus_c == BLE_CONN_HANDLE_INVALID))
//                {
//                    // Initiate connection.
//                    err_code = sd_ble_gap_connect(&p_gap_evt->params.adv_report.peer_addr,
//                                                  &m_scan_params,
//                                                  &m_connection_param);
//                    if (err_code != NRF_SUCCESS)
//                    {
//                        NRF_LOG_INFO("Connection Request Failed, reason %d\r\n", err_code);
//                    }
//                }
            }
        } break; // BLE_GAP_ADV_REPORT

        case BLE_GAP_EVT_TIMEOUT:
        {
            // We have not specified a timeout for scanning, so only connection attemps can timeout.
            if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN)
            {
                NRF_LOG_INFO("Connection Request timed out.\r\n");
            }
        } break; // BLE_GAP_EVT_TIMEOUT

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
        {
            // Accept parameters requested by peer.
            err_code = sd_ble_gap_conn_param_update(p_gap_evt->conn_handle,
                                        &p_gap_evt->params.conn_param_update_request.conn_params);
            APP_ERROR_CHECK(err_code);
        } break; // BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            NRF_LOG_DEBUG("GATT Client Timeout.\r\n");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTC_EVT_TIMEOUT

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            NRF_LOG_DEBUG("GATT Server Timeout.\r\n");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_TIMEOUT

#if (NRF_SD_BLE_API_VERSION == 3)
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
            err_code = sd_ble_gatts_exchange_mtu_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                       NRF_BLE_MAX_MTU_SIZE);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST
#endif

        default:
            // No implementation needed.
            break;
    }
}

void on_ble_central_evt(const ble_evt_t * const p_ble_evt, uint16_t conn_handle) {
  /** on_ble_central_evt will update the connection handles, so we want to execute it
   * after dispatching to the central applications upon disconnection. */
  if (p_ble_evt->header.evt_id != BLE_GAP_EVT_DISCONNECTED) {
    on_ble_central_conn_evt(p_ble_evt);
  }

  if (conn_handle < CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT) {
    ble_db_discovery_on_ble_evt(&m_ble_db_discovery[conn_handle], p_ble_evt);
  }
  ble_nus_c_on_ble_evt(&m_ble_nus_c, p_ble_evt);

  // If the peer disconnected, we update the connection handles last.
  if (p_ble_evt->header.evt_id == BLE_GAP_EVT_DISCONNECTED) {
    on_ble_central_conn_evt(p_ble_evt);
  }
}


/**@brief Heart rate collector initialization.
 */
void nus_c_init(void)
{
    uint32_t         err_code;
    ble_nus_c_init_t nus_c_init_obj;

    nus_c_init_obj.evt_handler = ble_nus_c_evt_handler;

    err_code = ble_nus_c_init(&m_ble_nus_c, &nus_c_init_obj);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling database discovery events.
 *
 * @details This function is callback function to handle events from the database discovery module.
 *          Depending on the UUIDs that are discovered, this function should forward the events
 *          to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
    ble_nus_c_on_db_disc_evt(&m_ble_nus_c, p_evt);
}


/**
 * @brief Database discovery initialization.
 */
void db_discovery_init(void)
{
    ret_code_t err_code = ble_db_discovery_init(db_disc_handler);
    APP_ERROR_CHECK(err_code);
}
