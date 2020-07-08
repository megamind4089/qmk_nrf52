/*
Copyright 2018 Sekigon

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nrf_drv_saadc.h"
#include "adc.h"
#include "nrf_assert.h"
#include "app_error.h"

//#define NRF_LOG_MODULE_NAME "ADC"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#ifdef USE_BATTERY_PIN
#define APP_VCC_PIN USE_BATTERY_PIN
#else
#define APP_VCC_PIN NRF_SAADC_INPUT_VDD
#endif

static nrf_saadc_value_t       adc_buffer[1]; /**< ADC buffer. */
static void adc_event_handler(nrf_drv_saadc_evt_t const *p_event) {}

void adc_init() {
  nrf_drv_saadc_config_t adccfg = NRF_DRV_SAADC_DEFAULT_CONFIG;
  adccfg.resolution = NRF_SAADC_RESOLUTION_8BIT;
  nrf_drv_saadc_init(&adccfg, adc_event_handler);
  nrf_saadc_channel_config_t pincfg = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(APP_VCC_PIN);
  nrf_drv_saadc_channel_init(0, &pincfg);
}

void adc_start() {
  ret_code_t res = nrf_drv_saadc_sample_convert(0, adc_buffer); // blocking function
  if (res != NRF_SUCCESS) {
    adc_buffer[0] = 0;
  }
#ifdef USE_BATTERY_PIN
    NRF_LOG_DEBUG("Current vbat: %d mV", get_vcc());
#else
    NRF_LOG_DEBUG("Current vcc: %d mV", get_vcc());
#endif
}

uint16_t get_vcc() {
  int16_t v = adc_buffer[0] < 0 ? 0 : adc_buffer[0];
#ifdef USE_BATTERY_PIN
# ifndef BATTERY_VMAX
#   define V_MAX 4200
# else
#   define V_MAX BATTERY_VMAX
# endif
# ifndef BATTERY_R1
#   error If use USE_BATTERY_PIN you must define BATTERY_R1. For VDD=1.8V it is good 18k(18000)
# endif
# ifndef BATTERY_R2
#   error If use USE_BATTERY_PIN you must define BATTERY_R2. Internal pull-up/pull-down resistor = 13k(13000)
# endif
#define V_BAT ((V_MAX * BATTERY_R2) / (BATTERY_R1 + BATTERY_R2))
  return ((uint32_t) v * 6 * 600 / 255) * V_MAX / V_BAT;
#else
  return ((uint32_t) v * 6 * 600 / 255);
#endif
}
