
#pragma once

#include <stdint.h>
#include "ble_gattc.h"
#include "ble.h"
#include "nrf_error.h"
#include "ble_srv_common.h"
#include "ble_db_discovery.h"
#include "sdk_errors.h"

#define HIDS_REPORT_COUNT 6

#ifdef __cplusplus
extern "C" {
#endif

/**@brief   Macro for defining a nrf_ble_gatts_c instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define NRF_BLE_HIDS_C_DEF(_name)                                                                  \
static nrf_ble_hids_c_t _name;                                                                     \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     NRF_BLE_GATTS_C_BLE_OBSERVER_PRIO,                                             \
                     nrf_ble_hids_c_on_ble_evt, &_name)

/** @brief Macro for defining multiple nrf_ble_hids_c instances.
 *
 * @param   _name   Name of the array of instances.
 * @param   _cnt    Number of instances to define.
 * @hideinitializer
 */
#define NRF_BLE_HIDS_C_ARRAY_DEF(_name, _cnt)                 \
static nrf_ble_hids_c_t _name[_cnt];                          \
NRF_SDH_BLE_OBSERVERS(_name ## _obs,                           \
                      NRF_BLE_GATTS_C_BLE_OBSERVER_PRIO,       \
                      nrf_ble_hids_c_on_ble_evt, &_name, _cnt)

/**@brief   Type of the hid Service client event. */
typedef enum
{
    NRF_BLE_HIDS_C_EVT_DISCOVERY_COMPLETE, /**< Event indicating that the hid Service and Service Changed Characteristic have been found on the peer. */
    NRF_BLE_HIDS_C_EVT_DISCOVERY_FAILED,   /**< Event indicating that the Service Changed characteristic has not been found on the peer. */
    NRF_BLE_HIDS_C_EVT_DISCONN_COMPLETE,   /**< Event indicating that the hid Service client module has finished processing the BLE_GAP_EVT_DISCONNECTED event. The event can be used by the application to do cleanup related to the hid Service client.*/
    NRF_BLE_HIDS_C_EVT_SRV_CHANGED,        /**< Event indicating that a Service Changed indication has been received. */
} nrf_ble_hids_c_evt_type_t;

typedef struct {
  uint8_t report_count;
  ble_gatt_db_char_t report[HIDS_REPORT_COUNT];
  ble_gatt_db_char_t boot_keyboard;
  ble_gatt_db_char_t boot_mouse;
} hids_chars_t;

/**@brief   Structure containing the event from the Service Changed client module to the application.
 */
typedef struct
{
    nrf_ble_hids_c_evt_type_t evt_type;           /**< Type of event. See @ref nrf_ble_hids_c_evt_type_t. */
    uint16_t                   conn_handle;        /**< Handle of the connection for which this event has occurred. */
    hids_chars_t params;
} nrf_ble_hids_c_evt_t;

/**@brief   Service Changed handler type. */
typedef void (* nrf_ble_hids_c_evt_handler_t)(nrf_ble_hids_c_evt_t * p_evt);

typedef void (*ble_hids_c_data_handler_t)(uint8_t const *dat, uint16_t len,
                                          uint16_t conn_handle,
                                          uint16_t report_idx);

/**@brief   Structure for holding the information related to the Service Changed indication at the server.
 *
 * @details A hid Server will never have more than one instance of the Service Changed Characteristic.
 *          Therefore, you never need more than one instance of the hid Service client structure.
 *
 * @warning This structure must be zero-initialized.
 */
typedef struct
{
    bool                          initialized;      /**< Boolean indicating whether the context has been initialized or not. */
    bool                          char_found;       /**< Boolean indicating whether the Service Changed indication has been found.*/
    hids_chars_t                  hids_chars;
    uint16_t                      conn_handle;      /**< Active connection handle. */
    nrf_ble_hids_c_evt_handler_t  evt_handler;      /**< Pointer to event handler function. */
    ble_srv_error_handler_t       err_handler;      /**< Pointer to error handler function. */
    ble_hids_c_data_handler_t     data_handler;
    bool                          notify_enabled[HIDS_REPORT_COUNT];
} nrf_ble_hids_c_t;

/**@brief   Initialization parameters. These must be supplied when calling @ref nrf_ble_hids_c_init. */
typedef struct
{
    nrf_ble_hids_c_evt_handler_t evt_handler; /**< Event handler that is called by the Service Changed client module when any related event occurs. */
    ble_srv_error_handler_t       err_handler; /**< Error handler that is called by the Service Changed client module if any error occurs. */
    ble_hids_c_data_handler_t     data_handler;
} nrf_ble_hids_c_init_t;


/**@brief   Function for initializing the Service Changed client module.
 *
 * @param[in,out] p_hids_c         Pointer to the hid Service client structure instance.
 * @param[in]     p_hids_c_init    Init parameters containing the event handler that is called by
 *                                  the Service Changed client module when any related event occurs.
 *
 * @retval  NRF_SUCCESS     If the service was initialized successfully.
 * @retval  NRF_ERROR_NULL  If any of the input parameters are NULL.
 * @return                  If functions from other modules return errors to this function,
 *                          the @ref nrf_error are propagated.
 */
ret_code_t nrf_ble_hids_c_init(nrf_ble_hids_c_t      * p_hids_c,
                                nrf_ble_hids_c_init_t * p_hids_c_init);


/**@brief Function for enabling remote indication.
 *
 * @param[in,out]   p_hids_c   Pointer to the Service Changed client structure.
 * @param[in]       enable      True to enable Service Changed remote indication, false to disable.
 *
 * @retval NRF_SUCCESS Operation success.
 * @return             If functions from other modules return errors to this function,
 *                     the @ref nrf_error are propagated.
 */
ret_code_t nrf_ble_hids_c_enable_indication(nrf_ble_hids_c_t * p_hids_c,
                                             bool                enable);


/**@brief   Function for handling events from the database discovery module.
 *
 * @details This function handles an event from the database discovery module, and determine
 *          if it relates to the discovery of Service Changed characteristics at the peer. If so,
 *          it will call the application's event handler indicating that Service Changed
 *          characteristic has been discovered at the peer.
 *
 * @param[in,out] p_hids_c     Pointer to the hid Service client structure instance.
 * @param[in]     p_evt         Pointer to the event received from the database discovery module.
 */
void nrf_ble_hids_c_on_db_disc_evt(nrf_ble_hids_c_t      const * p_hids_c,
                                    ble_db_discovery_evt_t       * p_evt);


/**@brief   Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 * @param[in]   p_context   Context.
 */
void nrf_ble_hids_c_on_ble_evt(ble_evt_t const * p_ble_evt,
                                void            * p_context);

ret_code_t nrf_ble_hids_c_handles_assign(nrf_ble_hids_c_t *const p_hids_c,
                                         uint16_t const conn_handle,
                                         hids_chars_t const *p_hids_chars);
ret_code_t
nrf_ble_hids_c_enable_notification(nrf_ble_hids_c_t *const p_hids_c,
                                   bool const enable);

ret_code_t ble_hids_c_read_report(nrf_ble_hids_c_t const *const p_hids_c,
                       uint8_t report_idx);

#ifdef __cplusplus
}
#endif

/** @} */
