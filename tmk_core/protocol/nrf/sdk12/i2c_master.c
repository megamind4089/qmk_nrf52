#include <string.h>

#include "i2c.h"
#include "nrf_drv_twi.h"

static const nrf_drv_twi_t m_twi_master = NRF_DRV_TWI_INSTANCE(0);

#ifndef CONFIG_PIN_SCL
#define CONFIG_PIN_SCL 16
#endif
#ifndef CONFIG_PIN_SDA
#define CONFIG_PIN_SDA 18
#endif

int i2c_init(void)
{
  const nrf_drv_twi_config_t config =
  {
//     .scl                = 16,
//     .sda                = 18,
     .scl                = CONFIG_PIN_SCL, // 18,
     .sda                = CONFIG_PIN_SDA, // 16,
     .frequency          = NRF_TWI_FREQ_400K,
     .interrupt_priority = TWI_DEFAULT_CONFIG_IRQ_PRIORITY,
     .clear_bus_init     = false,
     .hold_bus_uninit    = false
  };
  int ret = nrf_drv_twi_init(&m_twi_master, &config, NULL, NULL); // Initialize as blocking mode
  nrf_drv_twi_enable(&m_twi_master);
  return ret;
}

void i2c_uninit(void) {
  nrf_drv_twi_uninit(&m_twi_master);
}

uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
{
  return nrf_drv_twi_tx(&m_twi_master, address, data, length, false);
}

uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
{
  return nrf_drv_twi_rx(&m_twi_master, address, data, length);
}

uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
  nrf_drv_twi_tx(&m_twi_master, devaddr, &regaddr, 1, true);
  return nrf_drv_twi_rx(&m_twi_master, devaddr, data, length);
}

uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
  static uint8_t buffer[256];
  buffer[0] = regaddr;
  memcpy(&buffer[1], data, length);
  return nrf_drv_twi_tx(&m_twi_master, devaddr, buffer, length+1, false);
}
