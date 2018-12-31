
# it should exist either in <this_dir>/ld/
MCU_LDSCRIPT = nrf52832_slave
MCU_SERIES = NRF52832
NRFSDK_ROOT := $(NRFSDK12_ROOT) # /c/dev/nRF5_SDK_12.3.0_d7731ad# Path to nRF SDK v12.3.0

NRF_DEBUG = no

# project specific files
#SRC =	matrix.c

NRF_SEPARATE = slave
#CFLAGS += -DNRF_XTAL_32MHZ