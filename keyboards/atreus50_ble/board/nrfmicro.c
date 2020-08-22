#include "nrfmicro.h"

#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_power.h"
#include "nrfx_power.h"
#include "nrfx_pwm.h"
#include "nrf.h"

#include "timer.h"

#include "app_ble_func.h"

#define DFU_MAGIC_UF2_RESET             0x57

#define POWER_TIMEOUT  900000

uint32_t power_timeout;
bool power_enabled = false;

void bootloader_jump(void) {
    sd_power_gpregret_set(0, DFU_MAGIC_UF2_RESET);
    NVIC_SystemReset();
}

bool has_usb(void) {
    return (nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_CONNECTED
            || nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_READY);
}

void nrfmicro_power_enable(bool enable) {
    if (enable) {
        power_timeout = timer_read32() + POWER_TIMEOUT;
    }

    if (power_enabled == enable)
        return;

    int pin_value = has_usb() || enable ? 0: 1;
    nrf_gpio_pin_write(POWER_PIN, pin_value);

    power_enabled = enable;
}

void nrfmicro_charger_enable(bool enable) {
    if (enable) {
        nrf_gpio_cfg_output(PROG_PIN);
        nrf_gpio_pin_clear(PROG_PIN);
    } else {
        nrf_gpio_cfg_input(PROG_PIN, NRF_GPIO_PIN_NOPULL);
    }
}

void nrfmicro_blink(int times, int delay) {
    for (int i=0; i<times*2; i++) {
        nrf_gpio_pin_toggle(LED_PIN);
        nrf_delay_ms(delay);
    }
}

static void check_ble_switch(bool init) {
    static bool is_usb_enabled = false;

    if (init) {
        is_usb_enabled = has_usb() ? true : false;
        set_usb_enabled(is_usb_enabled);
        set_ble_enabled(!is_usb_enabled);
    } else if (is_usb_enabled && (false != has_usb())) {
        is_usb_enabled = false;
        set_usb_enabled(is_usb_enabled);
        set_ble_enabled(!is_usb_enabled);
    }
}

void nrfmicro_init(void) {
    nrf_gpio_cfg_output(LED_PIN);

    nrf_gpio_cfg_output(POWER_PIN);
    nrfmicro_power_enable(true);

    nrfmicro_charger_enable(true);

    check_ble_switch(true);

    nrfmicro_blink(2, 100);
}

void nrfmicro_update(void) {
    check_ble_switch(false);

    if (power_enabled && timer_read32() > power_timeout) {
        sleep_mode_enter();
    }
}

