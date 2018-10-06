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

#include "nrf_drv_adc.h"
#include "adc.h"
#include "nrf_assert.h"
#include "app_error.h"

#define NRF_LOG_MODULE_NAME "ADC"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

static nrf_adc_value_t       adc_buffer[1]; /**< ADC buffer. */
static void adc_event_handler(nrf_drv_adc_evt_t const * p_event)
{
  NRF_LOG_INFO("conversion start\r\n");
    if (p_event->type == NRF_DRV_ADC_EVT_DONE)
    {
        uint32_t i;
        for (i = 0; i < p_event->data.done.size; i++)
        {
            NRF_LOG_INFO("Current sample value: %d mV\r\n", get_vcc());
        }
//        nrf_adc_disable();
    }
}

void adc_init() {
  ret_code_t ret_code;
  const nrf_drv_adc_config_t config = NRF_DRV_ADC_DEFAULT_CONFIG;

  ret_code = nrf_drv_adc_init(&config, adc_event_handler);
  APP_ERROR_CHECK(ret_code);
//  ret_code_t ret_code;
//  const nrf_drv_adc_config_t config = NRF_DRV_ADC_DEFAULT_CONFIG;
//
//  ret_code = nrf_drv_adc_init(&config, adc_event_handler);
//  APP_ERROR_CHECK(ret_code);
//  nrf_drv_adc_channel_enable(&m_channel_config);
//  APP_ERROR_CHECK(nrf_drv_adc_buffer_convert(adc_buffer, 1));
}

void adc_start() {
//  nrf_adc_enable();
  nrf_drv_adc_channel_t m_channel_config = {{{
      .resolution = NRF_ADC_CONFIG_RES_8BIT,
      .input      = NRF_ADC_CONFIG_SCALING_SUPPLY_ONE_THIRD,
      .reference  = NRF_ADC_CONFIG_REF_VBG,
      .ain        = (NRF_ADC_CONFIG_INPUT_DISABLED)
   }}, NULL};
  nrf_drv_adc_channel_enable(&m_channel_config);
  APP_ERROR_CHECK(nrf_drv_adc_buffer_convert(adc_buffer, 1));
  nrf_drv_adc_sample();
}

uint16_t get_vcc() {
  return ((uint32_t)adc_buffer[0]*3*1200/255);
}
