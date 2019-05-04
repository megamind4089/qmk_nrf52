/*
 * spim.c
 *
 *  Created on: 2019/04/27
 *      Author: sekigon
 */


#include "nrfx_config.h"
#include "nrfx_spim.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#ifndef CONFIG_SCK_PIN
#define CONFIG_SCK_PIN 46
#endif

#ifndef CONFIG_MISO_PIN
#define CONFIG_MISO_PIN 47
#endif

#ifndef CONFIG_MOSI_PIN
#define CONFIG_MOSI_PIN 10
#endif

#ifndef CONFIG_SS_PIN
#define CONFIG_SS_PIN 3
#endif

#ifndef CONFIG_SPI_FREQ
#define CONFIG_SPI_FREQ NRF_SPIM_FREQ_500K
#endif

static const nrfx_spim_t m_spim = NRFX_SPIM_INSTANCE(2);
static int spim_complete;

void nrfx_spim_evt_handler(nrfx_spim_evt_t const * p_event,
                                         void *                  p_context)
{
  if (p_event->type == NRFX_SPIM_EVENT_DONE)
  {
    spim_complete = 1;
  }
}

nrfx_err_t res_init;

int spim_init()
{
  nrfx_spim_config_t config =
  {
      .sck_pin = CONFIG_SCK_PIN,
      .mosi_pin = CONFIG_MOSI_PIN,
      .miso_pin = CONFIG_MISO_PIN,
      .ss_pin = CONFIG_SS_PIN,
      .ss_active_high = false,
      .orc = 0xFF,
      .frequency = CONFIG_SPI_FREQ,
      .irq_priority = APP_IRQ_PRIORITY_HIGH,
      .mode = NRF_SPIM_MODE_0,
      .bit_order = NRF_SPIM_BIT_ORDER_MSB_FIRST,
  };
  res_init = nrfx_spim_init(&m_spim, &config, nrfx_spim_evt_handler, NULL);

  return 0;
}

int spim_start(uint8_t * p_tx_buffer, size_t tx_length, uint8_t * p_rx_buffer, size_t rx_length)
{
  static nrfx_spim_xfer_desc_t xfer_desc;
      xfer_desc.p_tx_buffer = p_tx_buffer;
      xfer_desc.tx_length = tx_length;
      xfer_desc.p_rx_buffer = p_rx_buffer;
      xfer_desc.rx_length = rx_length;

  spim_complete = 0;
  nrfx_err_t err_code = NRFX_ERROR_BUSY;
  if (res_init == NRFX_SUCCESS)
  {
    err_code = nrfx_spim_xfer(&m_spim, &xfer_desc, 0);
  }
  else
  {
    NRF_LOG_INFO("Fail");
  }
  int retry_cnt = 20;
  if (err_code == NRFX_SUCCESS)
  {
    while (!spim_complete)
    {
      nrf_pwr_mgmt_run();
      if (retry_cnt-- == 0) {
//        nrfx_spim_abort(&m_spim);
        NRF_LOG_INFO("TIMEOUT");
        break;
      }

    }
  }

  return 0;
}
