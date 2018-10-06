#include "bootloader.h"
#include "nrf.h"
#include "nrf_power.h"

__attribute__((weak))
void bootloader_jump(void) {
  sd_power_gpregret_set(0, 1);
  NVIC_SystemReset();
}
