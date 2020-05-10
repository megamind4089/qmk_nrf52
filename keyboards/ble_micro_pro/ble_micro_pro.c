
#include "config.h"
#include "quantum.h"

#include "ble_conn_state.h"
#include "nrf_gpio.h"

void matrix_init_kb() {
  nrf_gpio_cfg_output(LED_PIN);
  BLE_LED_OFF();
}

static uint32_t led_pattern = 0;

bool set_led_pattern(uint32_t pattern) {
  if (led_pattern != pattern) {
    led_pattern = pattern;
    return true;
  }

  return false;
}

static void led_pattern0(uint32_t base_time);
static void led_pattern1(uint32_t base_time);

void (*led_patterns[])(uint32_t base_time) = {led_pattern0, led_pattern1};

void matrix_scan_kb() {
  static uint32_t base_time = 0;
  bool pattern_change;

  if (ble_conn_state_central_conn_count() > 0) {
    pattern_change = set_led_pattern(1);
  } else {
    pattern_change = set_led_pattern(0);
  }

  if (pattern_change) {
    base_time = timer_read32();
  }

  if (led_pattern < sizeof(led_patterns) / sizeof(led_patterns[0])) {
    led_patterns[led_pattern](base_time);
  } else {
    led_pattern0(base_time);
  }

  matrix_scan_user();
}

static void led_pattern0(uint32_t base_time) {
  const uint32_t period = 3000;
  uint32_t time = timer_elapsed32(base_time) % period;

  if (time > 0 && time < 50) {
    BLE_LED_ON();
  } else if (time > 150 && time < 200) {
    BLE_LED_ON();
  } else {
    BLE_LED_OFF();
  }
}

static void led_pattern1(uint32_t base_time) {
  const uint32_t period = 3000;
  uint32_t time = timer_elapsed32(base_time) % period;

  if (time > 0 && time < 100) {
    BLE_LED_ON();
  } else {
    BLE_LED_OFF();
  }
}
