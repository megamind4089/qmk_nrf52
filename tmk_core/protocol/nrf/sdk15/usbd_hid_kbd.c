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
#include "sdk_common.h"

#include <string.h>
#include "app_usbd_hid_kbd.h"
#include "app_util_platform.h"
#include "host.h"

#define QUEUE_LEN 16
static report_keyboard_t report_buf[QUEUE_LEN];
static uint8_t widx, ridx, cnt;
static bool flag_usb_sending=false;

/**
 * @brief Auxiliary function to access HID keyboard context data.
 *
 * @param[in] p_inst class instance data.
 *
 * @return HID keyboard instance data context.
 */
static inline app_usbd_hid_kbd_ctx_t * hid_kbd_ctx_get(app_usbd_hid_kbd_t const * p_kbd)
{
    ASSERT(p_kbd != NULL);
    ASSERT(p_kbd->specific.p_data != NULL);
    return &p_kbd->specific.p_data->ctx;
}

/**
 * @brief Returns keyboard report buffer handle.
 *
 * @param[in] p_kbd HID keyboard instance.
 *
 * @return HID report buffer.
 */
static inline
app_usbd_hid_report_buffer_t const * hid_kbd_rep_buffer_get(app_usbd_hid_kbd_t const * p_kbd)
{
    ASSERT(p_kbd != NULL);
    app_usbd_hid_inst_t const    * p_hinst    = &p_kbd->specific.inst.hid_inst;
    app_usbd_hid_kbd_ctx_t       * p_kbd_ctx  = hid_kbd_ctx_get(p_kbd);
    app_usbd_hid_report_buffer_t * p_rep_buff = app_usbd_hid_rep_buff_in_get(p_hinst);

    p_rep_buff->p_buff = p_kbd_ctx->report_buff;
    p_rep_buff->size   = sizeof(p_kbd_ctx->report_buff);

    /*Keyboard has only one report input/output report buffer */
    return p_rep_buff;
}


/**
 * @brief Auxiliary function to prepare report transfer buffer to next transfer.
 *
 * @param[in] p_kbd HID keyboard instance.
 *
 * @retval true     Next transfer is required.
 * @retval false    Next transfer is not required.
 */
static inline bool hid_kbd_transfer_next(app_usbd_hid_kbd_t const * p_kbd)
{
    /*Send report only when state has changed*/
    app_usbd_hid_report_buffer_t const * p_rep_buffer = hid_kbd_rep_buffer_get(p_kbd);
    app_usbd_hid_kbd_ctx_t             * p_kbd_ctx    = hid_kbd_ctx_get(p_kbd);

    if (memcmp(p_rep_buffer->p_buff, &p_kbd_ctx->rep, p_rep_buffer->size))
    {
        memcpy(p_rep_buffer->p_buff, &p_kbd_ctx->rep, p_rep_buffer->size);
        return true;
    }

    return false;
}


/**
 * @brief Triggers IN endpoint transfer.
 *
 * @param[in] p_kbd HID keyboard instance.
 *
 * @return Standard error code.
 */
static inline ret_code_t hid_kbd_transfer_set(app_usbd_hid_kbd_t const * p_kbd)
{
    app_usbd_class_inst_t const * p_inst    = (app_usbd_class_inst_t const *)p_kbd;
    app_usbd_hid_kbd_ctx_t      * p_kbd_ctx = hid_kbd_ctx_get(p_kbd);

    nrf_drv_usbd_ep_t ep_addr = app_usbd_hid_epin_addr_get(p_inst);

    app_usbd_hid_state_flag_clr(&p_kbd_ctx->hid_ctx, APP_USBD_HID_STATE_FLAG_TRANS_IN_PROGRESS);

    if (!hid_kbd_transfer_next(p_kbd))
    {
        /* Transfer buffer hasn't changed since last transfer. No need to setup
         * next transfer.
         * */
        return NRF_SUCCESS;
    }

    app_usbd_hid_report_buffer_t const * p_rep_buffer = hid_kbd_rep_buffer_get(p_kbd);
    NRF_DRV_USBD_TRANSFER_IN(transfer, p_rep_buffer->p_buff, p_rep_buffer->size);

    ret_code_t ret;
    CRITICAL_REGION_ENTER();
    ret = app_usbd_ep_transfer(ep_addr, &transfer);
    if (ret == NRF_SUCCESS)
    {
        app_usbd_hid_state_flag_set(&p_kbd_ctx->hid_ctx, APP_USBD_HID_STATE_FLAG_TRANS_IN_PROGRESS);
    }
    CRITICAL_REGION_EXIT();

    return ret;
}

void usbd_clear_sending_flag() {
  flag_usb_sending = false;
}

int usbd_send_kbd_report(app_usbd_hid_kbd_t const *  p_kbd, report_keyboard_t *report) {
  app_usbd_hid_kbd_ctx_t * p_kbd_ctx   = hid_kbd_ctx_get(p_kbd);
  app_usbd_hid_report_buffer_t const * rep_buffer = hid_kbd_rep_buffer_get(p_kbd);
  uint8_t ret = 0;
  if (flag_usb_sending) {
    memcpy((uint8_t*)&report_buf[widx++], report, sizeof(report_keyboard_t));
    widx %= QUEUE_LEN;
    cnt++;
    return 1;
  }
  memcpy((uint8_t*)&p_kbd_ctx->rep, report, rep_buffer->size);
  ret = hid_kbd_transfer_set(p_kbd);
  if (ret == NRF_SUCCESS &&
    app_usbd_hid_state_flag_test(&p_kbd_ctx->hid_ctx, APP_USBD_HID_STATE_FLAG_TRANS_IN_PROGRESS)) {
    flag_usb_sending = true;
  } else {
    flag_usb_sending = false;
  }
  return ret;
}

int usbd_send_keyboard_buffered(app_usbd_hid_kbd_t const *  p_kbd) {
  report_keyboard_t report;
  if (cnt) {
    memcpy(&report, (uint8_t*)&report_buf[ridx++], sizeof(report_keyboard_t));
    ridx %= QUEUE_LEN;
    cnt--;
    usbd_send_kbd_report(p_kbd, &report);
  }
  return 0;
}
