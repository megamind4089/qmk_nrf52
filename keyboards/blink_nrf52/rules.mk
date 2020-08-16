NRF_DEBUG = no

MOUSEKEY_ENABLE = yes	# Mouse keys
EXTRAKEY_ENABLE = yes	# Audio control and System control
CONSOLE_ENABLE = yes	# Console for debug
COMMAND_ENABLE = yes    # Commands for debug and configuration
SLEEP_LED_ENABLE = no   # Breathing sleep LED during USB suspend
NKRO_ENABLE = no	    # USB Nkey Rollover
OLED_DRIVER_ENABLE = no	# OLED disabled
CUSTOM_MATRIX = yes # This flag should be on for nrf52

NO_USB_STARTUP_CHECK = yes

## chip/board settings
MCU_FAMILY = NRF52
MCU_SERIES = NRF52840

# Cortex version
# Teensy LC is cortex-m0; Teensy 3.x are cortex-m4
MCU  = cortex-m4

# linker script to use
MCU_LDSCRIPT = nrf52840

# ARM version, CORTEX-M0/M1 are 6, CORTEX-M3/M4/M7 are 7
ARMV = 7

#Path to nRF SDK v15.0.0
NRFSDK_ROOT := $(NRFSDK15_ROOT)

# Build Options
#   comment out to disable the options.
#
NRF_SEPARATE = master

