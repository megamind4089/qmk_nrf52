/**
 * Copyright (c) 2012 - 2018, Nordic Semiconductor ASA
 * Copyright (c) 2018, Sekigon
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "host.h"
#include "usb_descriptor.h"
#include "cli.h"

#ifdef RGBLIGHT_ENABLE
#include "rgblight.h"
#endif

#ifdef EXTRAKEY_ENABLE
const uint8_t ExtrakeyReport[] =
{
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x80), /* System Control */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, REPORT_ID_SYSTEM),
        HID_RI_LOGICAL_MINIMUM(16, 0x0001),
        HID_RI_LOGICAL_MAXIMUM(16, 0x0003),
        HID_RI_USAGE_MINIMUM(16, 0x0081), /* System Power Down */
        HID_RI_USAGE_MAXIMUM(16, 0x0083), /* System Wake Up */
        HID_RI_REPORT_SIZE(8, 16),
        HID_RI_REPORT_COUNT(8, 1),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),

    HID_RI_USAGE_PAGE(8, 0x0C), /* Consumer */
    HID_RI_USAGE(8, 0x01), /* Consumer Control */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, REPORT_ID_CONSUMER),
        HID_RI_LOGICAL_MINIMUM(16, 0x0001),
        HID_RI_LOGICAL_MAXIMUM(16, 0x029C),
        HID_RI_USAGE_MINIMUM(16, 0x0001), /* +10 */
        HID_RI_USAGE_MAXIMUM(16, 0x029C), /* AC Distribute Vertically */
        HID_RI_REPORT_SIZE(8, 16),
        HID_RI_REPORT_COUNT(8, 1),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),
};
#endif

const uint8_t AbsMouseReport[] = {
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x02), /* Mouse */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_USAGE(8, 0x01), /* Pointer */
        HID_RI_COLLECTION(8, 0x00), /* Physical */
            HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
            HID_RI_USAGE(8, 0x30), /* Usage X */
            HID_RI_USAGE(8, 0x31), /* Usage Y */
            HID_RI_LOGICAL_MINIMUM(8, -127),
            HID_RI_LOGICAL_MAXIMUM(8, 127),
            HID_RI_REPORT_COUNT(8, 0x02),
            HID_RI_REPORT_SIZE(8, 0x08),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
        HID_RI_END_COLLECTION(0),
    HID_RI_END_COLLECTION(0),
};


const uint8_t MouseReport[] = {
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x02), /* Mouse */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_USAGE(8, 0x01), /* Pointer */
        HID_RI_COLLECTION(8, 0x00), /* Physical */

            HID_RI_USAGE_PAGE(8, 0x09), /* Button */
            HID_RI_USAGE_MINIMUM(8, 0x01),  /* Button 1 */
            HID_RI_USAGE_MAXIMUM(8, 0x05),  /* Button 5 */
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(8, 0x01),
            HID_RI_REPORT_COUNT(8, 0x05),
            HID_RI_REPORT_SIZE(8, 0x01),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x03),
            HID_RI_INPUT(8, HID_IOF_CONSTANT),

            HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
            HID_RI_USAGE(8, 0x30), /* Usage X */
            HID_RI_USAGE(8, 0x31), /* Usage Y */
            HID_RI_LOGICAL_MINIMUM(8, -127),
            HID_RI_LOGICAL_MAXIMUM(8, 127),
            HID_RI_REPORT_COUNT(8, 0x02),
            HID_RI_REPORT_SIZE(8, 0x08),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),

            HID_RI_USAGE(8, 0x38), /* Wheel */
            HID_RI_LOGICAL_MINIMUM(8, -127),
            HID_RI_LOGICAL_MAXIMUM(8, 127),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x08),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),

            HID_RI_USAGE_PAGE(8, 0x0C), /* Consumer */
            HID_RI_USAGE(16, 0x0238), /* AC Pan (Horizontal wheel) */
            HID_RI_LOGICAL_MINIMUM(8, -127),
            HID_RI_LOGICAL_MAXIMUM(8, 127),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x08),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),

        HID_RI_END_COLLECTION(0),
    HID_RI_END_COLLECTION(0),
};

#undef CONCAT
#include "nrf.h"
#include "nrf_drv_usbd.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_drv_power.h"

#include "app_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd_hid_mouse.h"
#include "app_usbd_hid_kbd.h"
#include "app_usbd_hid_generic.h"
#include "app_usbd_cdc_acm.h"
//#include "boards.h"

//#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#undef APP_USBD_HID_KBD_REPORT_DSC
#define APP_USBD_HID_KBD_REPORT_DSC() {                                                    \
        0x05, 0x01,                    /* USAGE_PAGE (Generic Desktop)                   */\
        0x09, 0x06,                    /* USAGE (Keyboard)                               */\
        0xa1, 0x01,                    /* COLLECTION (Application)                       */\
        0x05, 0x07,                    /*   USAGE_PAGE (Keyboard)                        */\
        0x19, 0xe0,                    /*   USAGE_MINIMUM (Keyboard LeftControl)         */\
        0x29, 0xe7,                    /*   USAGE_MAXIMUM (Keyboard Right GUI)           */\
        0x15, 0x00,                    /*   LOGICAL_MINIMUM (0)                          */\
        0x25, 0x01,                    /*   LOGICAL_MAXIMUM (1)                          */\
        0x75, 0x01,                    /*   REPORT_SIZE (1)                              */\
        0x95, 0x08,                    /*   REPORT_COUNT (8)                             */\
        0x81, 0x02,                    /*   INPUT (Data,Var,Abs)                         */\
        0x95, 0x01,                    /*   REPORT_COUNT (1)                             */\
        0x75, 0x08,                    /*   REPORT_SIZE (8)                              */\
        0x81, 0x03,                    /*   INPUT (Cnst,Var,Abs)                         */\
        0x95, 0x05,                    /*   REPORT_COUNT (5)                             */\
        0x75, 0x01,                    /*   REPORT_SIZE (1)                              */\
        0x05, 0x08,                    /*   USAGE_PAGE (LEDs)                            */\
        0x19, 0x01,                    /*   USAGE_MINIMUM (Num Lock)                     */\
        0x29, 0x05,                    /*   USAGE_MAXIMUM (Kana)                         */\
        0x91, 0x02,                    /*   OUTPUT (Data,Var,Abs)                        */\
        0x95, 0x01,                    /*   REPORT_COUNT (1)                             */\
        0x75, 0x03,                    /*   REPORT_SIZE (3)                              */\
        0x91, 0x03,                    /*   OUTPUT (Cnst,Var,Abs)                        */\
        0x95, 0x06,                    /*   REPORT_COUNT (6)                             */\
        0x75, 0x08,                    /*   REPORT_SIZE (8)                              */\
        0x15, 0x00,                    /*   LOGICAL_MINIMUM (0)                          */\
        0x25, 0xFF,                    /*   LOGICAL_MAXIMUM (255)                        */\
        0x05, 0x07,                    /*   USAGE_PAGE (Keyboard)                        */\
        0x19, 0x00,                    /*   USAGE_MINIMUM (Reserved (no event indicated))*/\
        0x29, 0xFF,                    /*   USAGE_MAXIMUM (Keyboard Application)         */\
        0x81, 0x00,                    /*   INPUT (Data,Ary,Abs)                         */\
        0xc0                           /* END_COLLECTION                                 */\
}


/**
 * @brief User event handler, HID mouse
 * */
static void hid_mouse_user_ev_handler(app_usbd_class_inst_t const * p_inst,
    app_usbd_hid_user_event_t event);

/**
 * @brief User event handler, HID keyboard
 * */
static void hid_kbd_user_ev_handler(app_usbd_class_inst_t const * p_inst,
    app_usbd_hid_user_event_t event);

/**
 * @brief Enable HID mouse class
 * */
#define CONFIG_HAS_MOUSE    1

/**
 * @brief Enable HID keyboard class
 * */
#define CONFIG_HAS_KBD      1

/**
 * @brief USB composite interface enumerator
 * */
enum {
  APP_USBD_INTERFACE_KBD=0,
  APP_USBD_INTERFACE_MOUSE,
  APP_USBD_INTERFACE_EXTRA,
  APP_USBD_INTERFACE_ABSMOUSE,
  APP_USBD_INTERFACE_CDC_ACM_COMM,
  APP_USBD_INTERFACE_CDC_ACM_DATA,
};

#define EXTRA_ENDPOINT_LIST()                                      \
(                                                            \
        NRF_DRV_USBD_EPIN3                                   \
)

#define ABS_MOUSE_ENDPOINT_LIST()                                      \
(                                                            \
        NRF_DRV_USBD_EPIN4                                   \
)
///**
// * @brief Mouse button count
// * */
//static uint8_t m_generic_rep_dsc[] = APP_USBD_HID_MOUSE_REPORT_DSC_BUTTON(MOUSE_BUTTON_COUNT);

static const app_usbd_hid_subclass_desc_t extra_dsc = {
  sizeof(ExtrakeyReport),
  APP_USBD_DESCRIPTOR_REPORT,
  ExtrakeyReport
};

static const app_usbd_hid_subclass_desc_t * reps[] = {&extra_dsc};

/**
 * @brief Number of reports defined in report descriptor
 */
#define EXTRA_REPORT_COUNT        1

/**
 * @brief Size of maximum output report. HID generic class will reserve
 *        this buffer size + 1 memory space. */
#define EXTRA_REPORT_OUT_MAXSIZE  0

/*lint -save -e26 -e64 -e123 -e505 -e651*/
#define REPORT_IN_QUEUE_SIZE    1
#define REPORT_OUT_MAXSIZE  0
#define HID_GENERIC_EP_COUNT  1

APP_USBD_HID_GENERIC_GLOBAL_DEF(m_app_hid_extra,
                                APP_USBD_INTERFACE_EXTRA,
                                hid_mouse_user_ev_handler,
                                EXTRA_ENDPOINT_LIST(),
                                reps,
                                REPORT_IN_QUEUE_SIZE,
                                REPORT_OUT_MAXSIZE,
                                APP_USBD_HID_SUBCLASS_NONE,
                                APP_USBD_HID_PROTO_GENERIC);

static const app_usbd_hid_subclass_desc_t abs_mouse_dsc = {
  sizeof(AbsMouseReport),
  APP_USBD_DESCRIPTOR_REPORT,
  AbsMouseReport
};

static const app_usbd_hid_subclass_desc_t * abs_mouse_reps[] = {&abs_mouse_dsc};

APP_USBD_HID_GENERIC_GLOBAL_DEF(m_app_hid_abs_mouse,
                                APP_USBD_INTERFACE_ABSMOUSE,
                                hid_mouse_user_ev_handler,
                                ABS_MOUSE_ENDPOINT_LIST(),
                                abs_mouse_reps,
                                REPORT_IN_QUEUE_SIZE,
                                REPORT_OUT_MAXSIZE,
                                APP_USBD_HID_SUBCLASS_NONE,
                                APP_USBD_HID_PROTO_GENERIC);
/*lint -save -e26 -e64 -e123 -e505 -e651*/

/**
 * @brief Global HID mouse instance
 * */
#define MOUSE_BUTTON_COUNT 5
/**
 * @brief Global HID keyboard instance
 * */
//APP_USBD_HID_MOUSE_GLOBAL_DEF(m_app_hid_mouse,
//                              APP_USBD_INTERFACE_MOUSE,
//                              NRF_DRV_USBD_EPIN2,
//                              MOUSE_BUTTON_COUNT,
//                              hid_mouse_user_ev_handler,
//                              APP_USBD_HID_SUBCLASS_BOOT
//);
static const app_usbd_hid_subclass_desc_t mouse_dsc = {
  sizeof(MouseReport),
  APP_USBD_DESCRIPTOR_REPORT,
  MouseReport
};

static const app_usbd_hid_subclass_desc_t * mouse_reps[] = {&mouse_dsc};

APP_USBD_HID_GENERIC_GLOBAL_DEF(m_app_hid_mouse,
                                APP_USBD_INTERFACE_MOUSE,
                                hid_mouse_user_ev_handler,
                                (NRF_DRV_USBD_EPIN2),
                                mouse_reps,
                                REPORT_IN_QUEUE_SIZE,
                                REPORT_OUT_MAXSIZE,
                                APP_USBD_HID_SUBCLASS_NONE,
                                APP_USBD_HID_PROTO_GENERIC);

/**
 * @brief Global HID keyboard instance
 */
APP_USBD_HID_KBD_GLOBAL_DEF(m_app_hid_kbd,
                            APP_USBD_INTERFACE_KBD,
                            NRF_DRV_USBD_EPIN1,
                            hid_kbd_user_ev_handler,
                            APP_USBD_HID_SUBCLASS_BOOT
);


static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
    app_usbd_cdc_acm_user_event_t event);

#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN6
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN5
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT2

/**
 * @brief CDC_ACM class instance
 * */
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm, cdc_acm_user_ev_handler,
    APP_USBD_INTERFACE_CDC_ACM_COMM, APP_USBD_INTERFACE_CDC_ACM_DATA, CDC_ACM_COMM_EPIN,
    CDC_ACM_DATA_EPIN, CDC_ACM_DATA_EPOUT, APP_USBD_CDC_COMM_PROTOCOL_AT_V250);

typedef struct {
  uint8_t* const buf;
  uint16_t widx, ridx, cnt;
  const uint16_t len;
} struct_queue;

#define USB_SERIAL_TX_QUEUE_SIZE 1024UL
#define USB_SERIAL_RX_QUEUE_SIZE 64
static uint8_t tx_buf[USB_SERIAL_TX_QUEUE_SIZE], rx_buf[USB_SERIAL_RX_QUEUE_SIZE];
struct_queue tx_queue={tx_buf, 0, 0, 0, USB_SERIAL_TX_QUEUE_SIZE},
    rx_queue={rx_buf, 0, 0, 0, USB_SERIAL_RX_QUEUE_SIZE};

static size_t push_queue(struct_queue *q, uint8_t dat) {
  if (q->cnt < q->len) {
    q->buf[q->widx++] = dat;
    q->widx %= q->len;
    q->cnt++;
    return 1;
  } else {
    return 0;
  }
}

static size_t pop_queue(struct_queue *q, uint8_t *dat) {
  if (q->cnt) {
    *dat = q->buf[q->ridx++];
    q->ridx %= q->len;
    q->cnt--;
    return 1;
  } else {
    return 0;
  }
}

#define READ_SIZE 1
static char m_rx_buffer[NRF_DRV_USBD_EPSIZE];
static char m_tx_buffer[NRF_DRV_USBD_EPSIZE];
static bool m_send_flag = 0;

static void usbd_send_cdc_acm_internal(void const* p_context, char const *buf, size_t len) {
  ret_code_t ret;
  if (!m_send_flag) {
    len = len & 0xFF;
    memcpy(m_tx_buffer, buf, len);
    ret = app_usbd_cdc_acm_write(&m_app_cdc_acm, m_tx_buffer, len);
    if (ret == NRF_SUCCESS) {
      m_send_flag = true;
    }
  } else {
    uint8_t *p = (uint8_t*)buf;
    while (len--) {
      push_queue(&tx_queue, *p++);
    }
  }
}

void usbd_send_cdc_acm(uint8_t *buf, uint8_t len) {
  usbd_send_cdc_acm_internal(NULL, (char const*)buf, len);
}

void usbd_send_cdc_acm_queued(struct_queue *q) {
  uint8_t buf[NRF_DRV_USBD_EPSIZE];
  uint8_t dat;
  uint8_t idx=0;
  while (idx<sizeof(buf) && pop_queue(q, &dat)==1) {
    buf[idx++] = dat;
  }
  usbd_send_cdc_acm(buf, idx);
}

#include "nrf_log_backend_serial.h"
#include "nrf_log_internal.h"

static void nrf_log_backend_cdc_acm_put(nrf_log_backend_t const * p_backend,
                                     nrf_log_entry_t * p_msg){
  nrf_log_backend_serial_put(p_backend, p_msg, (uint8_t*)m_tx_buffer,
                             NRF_DRV_USBD_EPSIZE, usbd_send_cdc_acm_internal);
}
static void nrf_log_backend_cdc_acm_flush(nrf_log_backend_t const * p_backend)
{
}

static void nrf_log_backend_cdc_acm_panic_set(nrf_log_backend_t const * p_backend)
{
}

const nrf_log_backend_api_t nrf_log_backend_cdc_acm_api = {
        .put       = nrf_log_backend_cdc_acm_put,
        .flush     = nrf_log_backend_cdc_acm_flush,
        .panic_set = nrf_log_backend_cdc_acm_panic_set,
};

/**
 * @brief User event handler @ref app_usbd_cdc_acm_user_ev_handler_t (headphones)
 * */
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
    app_usbd_cdc_acm_user_event_t event) {
  app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

  switch (event) {
  case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN: {
    /*Setup first transfer*/
    ret_code_t ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, m_rx_buffer,
    READ_SIZE);
    UNUSED_VARIABLE(ret);
    break;
  }
  case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
    m_send_flag = false;
    break;
  case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
    m_send_flag = false;
    if (tx_queue.cnt) {
      usbd_send_cdc_acm_queued(&tx_queue);
    }
    break;
  case APP_USBD_CDC_ACM_USER_EVT_RX_DONE: {
    // Echoback
    UNUSED_VARIABLE(p_cdc_acm);
    ret_code_t ret;
    uint8_t idx=0;
    size_t size = app_usbd_cdc_acm_rx_size(p_cdc_acm);
    while (size--) {
      push_queue(&rx_queue, m_rx_buffer[idx++]);
    }

    /*Get amount of data waiting*/
    size = app_usbd_cdc_acm_bytes_stored(p_cdc_acm);
    if (size) {
      size = NRF_DRV_USBD_EPSIZE > size ? size : NRF_DRV_USBD_EPSIZE;
      /* Fetch data until internal buffer is empty */
      ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, m_rx_buffer, size);
      idx = 0;
      while (size--) {
        push_queue(&rx_queue, m_rx_buffer[idx++]);
      }
      UNUSED_VARIABLE(ret);
//      usbd_send_cdc_acm((uint8_t*) m_rx_buffer, size);
    }

    // wait next transfer
    ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, m_rx_buffer,
    READ_SIZE);
    UNUSED_VARIABLE(ret);

    break;
  }
  default:
    break;
  }
}

int cdc_acm_byte_to_read() {
  return (int)rx_queue.cnt;
}

void cdc_acm_putc(char c) {
  usbd_send_cdc_acm((uint8_t*)&c, 1);
}

char cdc_acm_getc() {
  uint8_t dat=0;
  pop_queue(&rx_queue, &dat);
  return (char)dat;
}

/*lint -restore*/

/**
 * @brief Button debounce time
 *
 * Number of microseconds to wait after button press detection
 */
#define DEBOUNCE_DELAY 5000

#define LED_USB_RESUME     (0)
#define LED_USB_START      (1)
#define LED_HID_REP_IN     (2)
#define LED_HID_REP_OUT    (3)

#define BTN_MOUSE_X_POS    (0)
#define BTN_MOUSE_LEFT     (1)
#define BTN_KBD_SHIFT      (2)
#define BTN_KBD_LETTER     (3)

/**
 * @brief Enable power USB detection
 *
 * Configure if example supports USB port connection
 */
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif

/**
 * @brief Mouse speed (value send via HID when board button is pressed).
 * */
#define PRESS_OFFSET (3)

//static void mouse_action(uint32_t button_state) {
//}
//
//static void kbd_action(uint32_t button_state) {
//}
//
static void kbd_status(void) {
//    bool v;
//    v = app_usbd_hid_kbd_led_state_get(&m_app_hid_kbd, APP_USBD_HID_KBD_LED_NUM_LOCK);
//    v ? bsp_board_led_on(LED_HID_REP_IN) : bsp_board_led_off(LED_HID_REP_IN);
//
//    v = app_usbd_hid_kbd_led_state_get(&m_app_hid_kbd, APP_USBD_HID_KBD_LED_CAPS_LOCK);
//    v ? bsp_board_led_on(LED_HID_REP_OUT) : bsp_board_led_off(LED_HID_REP_OUT);
}


/**
 * @brief Class specific event handler.
 *
 * @param p_inst    Class instance.
 * @param event     Class specific event.
 * */
static void hid_mouse_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                      app_usbd_hid_user_event_t event)
{
    UNUSED_PARAMETER(p_inst);
    switch (event) {
        case APP_USBD_HID_USER_EVT_OUT_REPORT_READY:
            /* No output report defined for HID mouse.*/
            ASSERT(0);
            break;
        case APP_USBD_HID_USER_EVT_IN_REPORT_DONE:
            break;
        default:
            break;
    }
}

int usbd_send_keyboard_buffered(app_usbd_hid_kbd_t const *  p_kbd);
void usbd_clear_sending_flag();
/**
 * @brief Class specific event handler.
 *
 * @param p_inst    Class instance.
 * @param event     Class specific event.
 * */
static void hid_kbd_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_hid_user_event_t event)
{
    UNUSED_PARAMETER(p_inst);
    switch (event) {
        case APP_USBD_HID_USER_EVT_OUT_REPORT_READY:
            /* Only one output report IS defined for HID keyboard class. Update LEDs state. */
            kbd_status();
            break;
        case APP_USBD_HID_USER_EVT_IN_REPORT_DONE:
            usbd_clear_sending_flag();
            NRF_LOG_DEBUG("APP_USBD_HID_USER_EVT_IN_REPORT_DONE");
            usbd_send_keyboard_buffered(&m_app_hid_kbd);
            break;
        default:
            break;
    }
}


/**
 * @brief USBD library specific event handler.
 *
 * @param event     USBD library event.
 * */
static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_DRV_SOF:
            break;
        case APP_USBD_EVT_DRV_SUSPEND:
            app_usbd_suspend_req(); // Allow the library to put the peripheral into sleep mode
            break;
        case APP_USBD_EVT_DRV_RESUME:
            kbd_status(); /* Restore LED state - during SUSPEND all LEDS are turned off */
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            NRF_LOG_DEBUG("USB disable");
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            NRF_LOG_DEBUG("USB power detected");
            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
                NRF_LOG_INFO("USB enable");
            }
#ifdef RGBLIGHT_ENABLE
//            rgblight_enable_noeeprom();
//            rgblight_mode_noeeprom(35);
#endif
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            NRF_LOG_INFO("USB power removed");
            app_usbd_stop();
#ifdef RGBLIGHT_ENABLE
//            rgblight_disable();
#endif
            break;
        case APP_USBD_EVT_POWER_READY:
            NRF_LOG_DEBUG("USB ready");
            app_usbd_start();
            break;
        default:
            break;
    }
}

int usbd_init(void) {
  ret_code_t ret;
  static const app_usbd_config_t usbd_config = {
      .ev_state_proc = usbd_user_ev_handler,
  };
//
  ret = nrf_drv_clock_init();
  APP_ERROR_CHECK(ret);

  ret = app_usbd_init(&usbd_config);
  APP_ERROR_CHECK(ret);

  NRF_LOG_DEBUG("Hello USB!\r\n");

  app_usbd_class_inst_t const * class_inst_kbd;
  class_inst_kbd = app_usbd_hid_kbd_class_inst_get(&m_app_hid_kbd);
  ret = app_usbd_class_append(class_inst_kbd);
  APP_ERROR_CHECK(ret);
  ret = app_usbd_class_rwu_register(class_inst_kbd);
  APP_ERROR_CHECK(ret);

  app_usbd_class_inst_t const * class_inst_mouse;
//  class_inst_mouse = app_usbd_hid_mouse_class_inst_get(&m_app_hid_mouse);
  class_inst_mouse = app_usbd_hid_generic_class_inst_get(&m_app_hid_mouse);
  ret = app_usbd_class_append(class_inst_mouse);
  APP_ERROR_CHECK(ret);

  app_usbd_class_inst_t const * class_inst_extra;
  class_inst_extra = app_usbd_hid_generic_class_inst_get(&m_app_hid_extra);
  ret = app_usbd_class_append(class_inst_extra);
  APP_ERROR_CHECK(ret);

  app_usbd_class_inst_t const * class_inst_abs_mouse;
  class_inst_abs_mouse = app_usbd_hid_generic_class_inst_get(&m_app_hid_abs_mouse);
  ret = app_usbd_class_append(class_inst_abs_mouse);
  APP_ERROR_CHECK(ret);

  app_usbd_class_inst_t const * class_cdc_acm;
  class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
  ret = app_usbd_class_append(class_cdc_acm);
  APP_ERROR_CHECK(ret);

  cli_init();

  return 0;
}

int usbd_enable(void) {
  uint32_t  ret = app_usbd_power_events_enable();
  APP_ERROR_CHECK(ret);
  return ret;
}

void usbd_process(void) {
  while (app_usbd_event_queue_process()) {
    continue;/* Nothing to do */
  }
  cli_exec();
}

int usbd_send_kbd_report(app_usbd_hid_kbd_t const *  p_kbd, report_keyboard_t *report);
int usbd_send_mouse_report(app_usbd_hid_mouse_t const *  p_mouse, report_mouse_t *report);
int usbd_send_keyboard(report_keyboard_t *report) {
  NRF_LOG_DEBUG("%d", report->keys[0]);
  uint8_t res = usbd_send_kbd_report(&m_app_hid_kbd, report);
  NRF_LOG_DEBUG("res:%d", res);

  if (nrf_drv_usbd_suspend_check()) {
    app_usbd_wakeup_req();
  }

  return res;
}

int usbd_send_mouse(report_mouse_t *report) {
//  return usbd_send_mouse_report(&m_app_hid_mouse, report);
  return app_usbd_hid_generic_in_report_set(&m_app_hid_mouse, report, 5);
}

int usbd_send_abs_mouse(int8_t x, int8_t y) {
  uint8_t report[2];
  report[0] = x;
  report[1] = y;
  return app_usbd_hid_generic_in_report_set(&m_app_hid_abs_mouse, report, 2);
}

typedef struct {
  uint8_t report_id;
  uint16_t usage;
}__attribute__ ((packed)) report_extra_t;

int usbd_send_system(uint16_t data) {
  report_extra_t report;
  report.report_id = REPORT_ID_SYSTEM;
  report.usage = data;
  uint32_t ret = app_usbd_hid_generic_in_report_set(&m_app_hid_extra, &report,
      sizeof(report_extra_t));
  NRF_LOG_INFO("Send system:%d", data);
  return ret;
}

int usbd_send_consumer(uint16_t data) {
  report_extra_t report;
  report.report_id = REPORT_ID_CONSUMER;
  report.usage = data;
  uint32_t ret = app_usbd_hid_generic_in_report_set(&m_app_hid_extra, &report,
      sizeof(report_extra_t));
  return ret;
}
