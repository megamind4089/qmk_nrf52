#ifndef TMK_CORE_PROTOCOL_NRF_NRF52_USBD_H_
#define TMK_CORE_PROTOCOL_NRF_NRF52_USBD_H_

#include "host.h"
#if defined(NRF52840_XXAA) && NRF_SDK_MAJOR_VER==15
int usbd_init(void);
int usbd_enable(void);
void usbd_process(void);
int usbd_send_keyboard(report_keyboard_t *report);
int usbd_send_mouse(report_mouse_t *report);
int usbd_send_system(uint16_t data);
int usbd_send_consumer(uint16_t data);
int usbd_send_abs_mouse(int8_t x, int8_t y);
void usbd_send_cdc_acm(uint8_t *dat, uint8_t cnt);

// Functions for CLI
int cdc_acm_byte_to_read();
void cdc_acm_putc(char c);
char cdc_acm_getc();

#else
int usbd_init(void) {return 0;}
int usbd_enable(void) {}
void usbd_process(void) {return 0;}
int usbd_send_keyboard(report_keyboard_t *report) {return 0;}
int usbd_send_mouse(report_mouse_t *report) {return 0;}
int usbd_send_system(uint16_t data) {return 0;}
int usbd_send_consumer(uint16_t data) {return 0;}
int usbd_send_abs_mouse(int8_t x, int8_t y) {return 0;}
#endif /* NRF82400_XXAA */

#endif /* TMK_CORE_PROTOCOL_NRF_NRF52_USBD_H_ */
