PROTOCOL_DIR = protocol
NRF_DIR = $(PROTOCOL_DIR)/nrf

  SRC += $(NRF_DIR)/matrix.c \
  				$(NRF_DIR)/io_expander.c \

  SRC += $(NRF_DIR)/$(NRF_VER_DIR)/ble_common.c \
       $(NRF_DIR)/$(NRF_VER_DIR)/i2c_master.c \
       $(NRF_DIR)/$(NRF_VER_DIR)/i2c_slave.c \
       $(NRF_DIR)/$(NRF_VER_DIR)/spi_master.c \

ifeq ($(MCU_FAMILY),NRF51)
  SRC += $(NRF_DIR)/$(NRF_VER_DIR)/system_nrf51.c
  SRC += $(NRF_DIR)/nrf51/adc.c
endif

ifeq ($(MCU_FAMILY),NRF52)
  SRC += $(NRF_DIR)/nrf52/adc.c
endif


ifeq ($(MCU_SERIES), NRF52840)
 SRC += $(NRF_DIR)/$(NRF_VER_DIR)/usbd.c \
       $(NRF_DIR)/$(NRF_VER_DIR)/usbd_hid_kbd.c \
       $(NRF_DIR)/$(NRF_VER_DIR)/usbd_hid_mouse.c \
       $(NRFSDK_ROOT)/components/libraries/usbd/class/hid/kbd/app_usbd_hid_kbd.c \
       $(NRFSDK_ROOT)/components/libraries/usbd/class/hid/mouse/app_usbd_hid_mouse.c \
       $(NRF_DIR)/$(NRF_VER_DIR)/cli.c \
       $(NRF_DIR)/microshell/core/microshell.c \
       $(NRF_DIR)/microshell/core/mscore.c \
       $(NRF_DIR)/microshell/util/mscmd.c \
       $(NRF_DIR)/microshell/util/msopt.c \
       $(NRF_DIR)/microshell/util/ntlibc.c \

VPATH += $(TMK_PATH)/$(NRF_DIR)/microshell/core
VPATH += $(TMK_PATH)/$(NRF_DIR)/microshell/util
endif

ifeq ($(strip $(NRF_SEPARATE)), slave)
  CFLAGS += -DNRF_SEPARATE_KEYBOARD_SLAVE
  SRC += $(NRF_DIR)/$(NRF_VER_DIR)/ble_slave.c
  SRC += $(NRF_DIR)/main_slave.c
else
  SRC += $(NRF_DIR)/$(NRF_VER_DIR)/ble_master.c
  SRC += $(NRF_DIR)/main_master.c
  ifeq ($(strip $(NRF_SEPARATE)), master)
    CFLAGS += -DNRF_SEPARATE_KEYBOARD_MASTER
    SRC += $(NRF_DIR)/$(NRF_VER_DIR)/ble_central.c
  endif
  ifeq ($(strip $(NRF_SEPARATE)), dongle)
    CFLAGS += -DNRF_SEPARATE_KEYBOARD_DONGLE
    SRC += $(NRF_DIR)/$(NRF_VER_DIR)/ble_dongle.c
    SRC += $(NRF_DIR)/$(NRF_VER_DIR)/ble_hid_c.c
  endif
endif

VPATH += $(TMK_PATH)/$(PROTOCOL_DIR)
VPATH += $(TMK_PATH)/$(NRF_DIR)

ifeq ($(strip $(MIDI_ENABLE)), yes)
  include $(TMK_PATH)/protocol/midi.mk
endif

