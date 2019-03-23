/*
 * i2c_slave.c
 *
 *  Created on: 2019/03/21
 *      Author: sekigon
 */

#include "nrfx_twis.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#ifndef CONFIG_PIN_SCL
#define CONFIG_PIN_SCL 16
#endif
#ifndef CONFIG_PIN_SDA
#define CONFIG_PIN_SDA 18
#endif

#ifndef CONFIG_I2C_FREQ
#define CONFIG_I2C_FREQ NRF_TWI_FREQ_400K
#endif

static const nrfx_twis_t m_twis= NRFX_TWIS_INSTANCE(1);
static uint8_t* send_buf;
static uint8_t send_len;

void twis_evt_handler(nrfx_twis_evt_t const * p_event) {
  NRF_LOG_DEBUG("Event %d", p_event->type);
  if (p_event->type == NRFX_TWIS_EVT_READ_REQ)
  {
//    if (!nrfx_twis_is_waiting_tx_buff(&m_twis)) {
//      return;
//    }
    nrfx_err_t res = nrfx_twis_tx_prepare (&m_twis, send_buf, send_len);
    APP_ERROR_CHECK(res);
  }
}

int i2cs_init(void) {
  const nrfx_twis_config_t config =
  {
     .addr               = {0x21 >> 1, 0x21},
     .scl                = CONFIG_PIN_SCL,
     .sda                = CONFIG_PIN_SDA,
     .scl_pull = NRF_GPIO_PIN_PULLUP,
     .sda_pull = NRF_GPIO_PIN_PULLUP,
     .interrupt_priority = APP_IRQ_PRIORITY_MID,
  };
  nrfx_err_t res=nrfx_twis_init(&m_twis, &config, twis_evt_handler);
  nrfx_twis_enable(&m_twis);
  return res;
}

void i2cs_uninit(void) {
  nrfx_twis_uninit(&m_twis);
}

void i2cs_prepare(uint8_t* const dat, uint8_t len) {
  send_buf = dat;
  send_len = len;
}
