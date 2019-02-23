#include "i2c_master.h"

#include <string.h>

#include "nrfx_twi.h"

static const nrfx_twi_t m_twi_master = NRFX_TWI_INSTANCE(0);

#ifndef CONFIG_PIN_SCL
#define CONFIG_PIN_SCL 16
#endif
#ifndef CONFIG_PIN_SDA
#define CONFIG_PIN_SDA 18
#endif

#ifndef CONFIG_I2C_FREQ
#define CONFIG_I2C_FREQ NRF_TWI_FREQ_400K
#endif

int i2c_init(void)
{
  const nrfx_twi_config_t config =
  {
//     .scl                = 16,
//     .sda                = 18,
     .scl                = CONFIG_PIN_SCL, // 18,
     .sda                = CONFIG_PIN_SDA, // 16,
     .frequency          = CONFIG_I2C_FREQ,
     .interrupt_priority = APP_IRQ_PRIORITY_LOW,
     .hold_bus_uninit     = false
  };
  int ret = nrfx_twi_init(&m_twi_master, &config, NULL, NULL); // Initialize as blocking mode
  nrfx_twi_enable(&m_twi_master);
  return ret;
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

uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
  static uint8_t buffer[256];
  buffer[0] = regaddr;
  memcpy(&buffer[1], data, length);
  return nrfx_twi_tx(&m_twi_master, devaddr, buffer, length+1, false);
}
//
//#include "nrfx_twim.h"
//
//static const nrfx_twim_t m_twim_master = NRFX_TWIM_INSTANCE(0);
//static int twim_complete = 0;
//void twim_evt_handler(nrfx_twim_evt_t const *p_event, void *p_context) {
//  twim_complete = p_event->type + 1;
//}
//
//int i2c_init(void) {
//  const nrfx_twim_config_t config =
//  {
//     .scl                = CONFIG_PIN_SCL,
//     .sda                = CONFIG_PIN_SDA,
//     .frequency          = CONFIG_I2C_FREQ,
//     .interrupt_priority = APP_IRQ_PRIORITY_HIGHEST,
//     .hold_bus_uninit     = false
//  };
//  return nrfx_twim_init(&m_twim_master, &config, /*twim_evt_handler*/NULL, NULL);
//}
//
//void i2c_uninit(void) {
//  nrfx_twim_uninit(&m_twim_master);
//}
//
//uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
//{
//  nrfx_err_t res;
//  res = nrfx_twim_tx(&m_twim_master, address, data, length, false);
//  if (res == NRFX_SUCCESS) {
////    twim_complete = 0;
////    while (twim_complete == 0) {
////      __WFI();
////    }
//  }
//  return res; //twim_complete == 1 ? 0 : 1;
//}
//
//uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
//{
//  nrfx_err_t res;
//  res = nrfx_twim_rx(&m_twim_master, address, data, length);
//  if (res == NRFX_SUCCESS) {
////    twim_complete = 0;
////    while (twim_complete == 0) {
////      __WFI();
////    }
//  }
//  return res; //twim_complete == 1 ? 0 : 1;
//}
//
//uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
////  nrfx_twi_tx(&m_twi_master, devaddr, &regaddr, 1, true);
////  return nrfx_twi_rx(&m_twi_master, devaddr, data, length);
//  i2c_transmit(devaddr, &regaddr, 1);
//  return i2c_receive(devaddr, data, length);
//}
//
//uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
//  static uint8_t buffer[256];
//  buffer[0] = regaddr;
//  memcpy(&buffer[1], data, length);
//  return i2c_transmit(devaddr, buffer, length+1);
//}
