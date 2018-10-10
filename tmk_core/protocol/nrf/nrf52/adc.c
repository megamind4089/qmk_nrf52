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

static nrf_saadc_value_t       adc_buffer[1]; /**< ADC buffer. */
static void adc_event_handler(nrf_drv_saadc_evt_t const * p_event)
{
  NRF_LOG_DEBUG("conversion start\r\n");
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        uint32_t i;
        for (i = 0; i < p_event->data.done.size; i++)
        {
            NRF_LOG_DEBUG("Current sample value: %d mV\r\n", get_vcc());
        }
//        nrf_adc_disable();
    }
}

void adc_init() {
  nrf_drv_saadc_config_t adccfg = NRF_DRV_SAADC_DEFAULT_CONFIG;
  adccfg.resolution = NRF_SAADC_RESOLUTION_8BIT;
  nrf_drv_saadc_init(&adccfg, adc_event_handler);
}

void adc_start() {
  nrf_saadc_channel_config_t pincfg = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_VDD);
  nrf_drv_saadc_channel_init(0, &pincfg);
  nrf_drv_saadc_buffer_convert(adc_buffer, 1);
  nrf_drv_saadc_sample();
}

uint16_t get_vcc() {
  return ((uint32_t)adc_buffer[0]*6*600/255);
}
