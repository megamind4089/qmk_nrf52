#include "i2c_master.h"

#include <string.h>

#include "nrfx_twi.h"

static const nrfx_twi_t m_twi_master = NRFX_TWI_INSTANCE(0);

void i2c_init(void)
{
  const nrfx_twi_config_t config =
  {
     .scl                = 16,
     .sda                = 18,
     .frequency          = NRF_TWI_FREQ_400K,
     .interrupt_priority = APP_IRQ_PRIORITY_LOW,
     .hold_bus_uninit     = false
  };
  nrfx_twi_init(&m_twi_master, &config, NULL, NULL); // Initialize as blocking mode
  nrfx_twi_enable(&m_twi_master);
}

void i2c_uninit(void) {
  nrfx_twi_uninit(&m_twi_master);
}

uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
{
  return nrfx_twi_tx(&m_twi_master, address, data, length, false);
}

uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
{
  return nrfx_twi_rx(&m_twi_master, address, data, length);
}

uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
  nrfx_twi_tx(&m_twi_master, devaddr, &regaddr, 1, true);
  return nrfx_twi_rx(&m_twi_master, devaddr, data, length);
}
