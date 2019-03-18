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

#include "ws2812.h"
#include "nrfx_pwm.h"
#include "wait.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrfx_power.h"
#ifndef RGB_DI_PIN
#include "pin_assign.h"
#endif

#define PWM_PERIOD ((int) (1.25f / 0.0625f))
#define PWM_0H_DURATION ((int) (0.4f / 0.0625f))
#define PWM_1H_DURATION ((int) (0.8f / 0.0625f))

nrfx_pwm_t pwm0 = NRFX_PWM_INSTANCE(0);

#define DUMMY_SIGNAL_LEN 10

void pwm_handler(nrfx_pwm_evt_type_t event_type) {

}

void ws2812_setleds(LED_TYPE *ledarray, uint16_t number_of_leds) {
  if (nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_CONNECTED ||
      nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_READY) {
    ws2812_setleds_pin(ledarray, number_of_leds, RGB_DI_PIN);
  }
}

void ws2812_setleds_pin (LED_TYPE *ledarray, uint16_t number_of_leds,uint8_t pinmask){
  static bool flag_init;
  if(!flag_init) {
  nrfx_pwm_config_t pwm0_config = NRFX_PWM_DEFAULT_CONFIG;
  pwm0_config.output_pins[0] = pinmask;
  pwm0_config.output_pins[1] = NRFX_PWM_PIN_NOT_USED;
  pwm0_config.output_pins[2] = NRFX_PWM_PIN_NOT_USED;
  pwm0_config.output_pins[3] = NRFX_PWM_PIN_NOT_USED;
  pwm0_config.top_value = PWM_PERIOD;
  nrfx_pwm_init(&pwm0, &pwm0_config, pwm_handler);
  flag_init = true;
  }

  const uint16_t t0H = PWM_0H_DURATION | (0x8000);
  const uint16_t t1H = PWM_1H_DURATION | (0x8000);
  nrf_pwm_values_common_t led[RGBLED_NUM * 3 * 8 + DUMMY_SIGNAL_LEN*3*8 + 8];
  nrf_pwm_values_t p_led = { .p_common = led };
  nrf_pwm_sequence_t pwm_seq = { .values = p_led,
      .length = number_of_leds*3*8 + DUMMY_SIGNAL_LEN*3*8 +8,
      .repeats = 0, .end_delay = 0, };
  uint16_t* p_dat = &led[0];

  for (int i = 0; i < number_of_leds; i++) {
    uint8_t mask = 0x80;
    for (int j = 0; j < 8; j++) {
      *p_dat++ = (ledarray[i].g & (mask >> j)) ? t1H : t0H;
    }
    mask = 0x80;
    for (int j = 0; j < 8; j++) {
      *p_dat++ = (ledarray[i].r & (mask >> j)) ? t1H : t0H;
    }
    mask = 0x80;
    for (int j = 0; j < 8; j++) {
      *p_dat++ = (ledarray[i].b & (mask >> j)) ? t1H : t0H;
    }
  }

  for (int i = 0; i < DUMMY_SIGNAL_LEN; i++) {
    for (int j = 0; j < 24; j++) {
      *p_dat++ = t0H;
    }
  }
  // reset signal
  *p_dat++ = 0x8000;
  *p_dat++ = 0x8000;
  *p_dat++ = 0x8000;
  *p_dat++ = 0x8000;
  *p_dat++ = 0x8000;
  *p_dat++ = 0x8000;
  *p_dat++ = 0x8000;
  *p_dat++ = 0x8000;
  nrfx_pwm_simple_playback(&pwm0, &pwm_seq, 1, NRFX_PWM_FLAG_STOP);
//  wait_us(50);
}

void ws2812_setleds_rgbw(LED_TYPE *ledarray, uint16_t number_of_leds) {
  const uint16_t t0H = ((int) (0.35f / 0.0625f)) | (0x8000);
  const uint16_t t1H = ((int) (1.36f / 0.0625f)) | (0x8000);
  nrf_pwm_values_common_t led[RGBLED_NUM * 3 * 8 + 1];
  nrf_pwm_values_t p_led = { .p_common = led };
  nrf_pwm_sequence_t pwm_seq = { .values = p_led, .length = sizeof(led)
      / sizeof(led[0]), .repeats = 0, .end_delay = 0, };
  uint16_t* p_dat = &led[0];

  for (int i = 0; i < number_of_leds; i++) {
    uint8_t mask = 1;
    for (int j = 0; j < 8; j++) {
      *p_dat++ = (ledarray[i].r & (mask << j)) ? t1H : t0H;
    }
    mask = 1;
    for (int j = 0; j < 8; j++) {
      *p_dat++ = (ledarray[i].g & (mask << j)) ? t1H : t0H;
    }
    mask = 1;
    for (int j = 0; j < 8; j++) {
      *p_dat++ = (ledarray[i].b & (mask << j)) ? t1H : t0H;
    }
  }
  *p_dat++ = 0x8000;
  nrfx_pwm_simple_playback(&pwm0, &pwm_seq, 1, NRFX_PWM_FLAG_STOP);
}
