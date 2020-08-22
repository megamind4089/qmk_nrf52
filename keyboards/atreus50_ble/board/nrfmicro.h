#pragma once

#include <stdbool.h>

bool has_usb(void);

void nrfmicro_update(void);
void nrfmicro_init(void);
void nrfmicro_blink(int times, int delay);
void nrfmicro_charger_enable(bool enable);
void nrfmicro_power_enable(bool enable);
