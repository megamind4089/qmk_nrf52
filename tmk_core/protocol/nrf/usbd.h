#ifndef TMK_CORE_PROTOCOL_NRF_NRF52_USBD_H_
#define TMK_CORE_PROTOCOL_NRF_NRF52_USBD_H_

#include "host.h"
#if defined(NRF52840_XXAA) && NRF_SDK_MAJOR_VER==15
int usbd_init(void);
void usbd_enable(void);
int usbd_process(void);
int usbd_send_keyboard(report_keyboard_t *report);
int usbd_send_mouse(report_mouse_t *report);
int usbd_send_system(uint16_t data);
int usbd_send_consumer(uint16_t data);
int usbd_send_abs_mouse(uint8_t x, uint8_t y);

#else
int usbd_init(void) {return 0;}
void usbd_enable(void) {}
int usbd_process(void) {return 0;}
int usbd_send_keyboard(report_keyboard_t *report) {return 0;}
int usbd_send_mouse(report_mouse_t *report) {return 0;}
int usbd_send_system(uint16_t data) {return 0;}
int usbd_send_consumer(uint16_t data) {return 0;}
int usbd_send_abs_mouse(uint8_t x, uint8_t y) {return 0;}
#endif /* NRF82400_XXAA */

#endif /* TMK_CORE_PROTOCOL_NRF_NRF52_USBD_H_ */
