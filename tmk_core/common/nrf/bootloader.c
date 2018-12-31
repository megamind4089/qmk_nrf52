#include "bootloader.h"
#include "nrf.h"
#include "nrf_power.h"

__attribute__((weak))
void bootloader_jump(void) {
#if NRF_SDK_MAJOR_VER==15
  sd_power_gpregret_set(0, 1);
  NVIC_SystemReset();
#elif NRF_SDK_MAJOR_VER==12
  NRF_POWER->GPREGRET |= 1;
  NVIC_SystemReset();
#else
#error "Invalid nRF_SDK version."
#endif
}
