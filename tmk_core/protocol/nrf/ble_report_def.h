#ifndef TMK_CORE_PROTOCOL_NRF_BLE_REPORT_DEF_H_
#define TMK_CORE_PROTOCOL_NRF_BLE_REPORT_DEF_H_

#include "usb_descriptor.h"
/*******************************************************************************
 * HID Report Descriptors
 ******************************************************************************/
#ifdef NKRO_ENABLE
#define NKRO_BYTE_LEN  KEYBOARD_REPORT_SIZE
const uint8_t NKROReportMap[] =
{
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x06), /* Keyboard */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_USAGE_PAGE(8, 0x07), /* Key Codes */
        HID_RI_USAGE_MINIMUM(8, 0xE0), /* Keyboard Left Control */
        HID_RI_USAGE_MAXIMUM(8, 0xE7), /* Keyboard Right GUI */
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(8, 0x01),
        HID_RI_REPORT_COUNT(8, 0x08),
        HID_RI_REPORT_SIZE(8, 0x01),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),

        HID_RI_USAGE_PAGE(8, 0x08), /* LEDs */
        HID_RI_USAGE_MINIMUM(8, 0x01), /* Num Lock */
        HID_RI_USAGE_MAXIMUM(8, 0x05), /* Kana */
        HID_RI_REPORT_COUNT(8, 0x05),
        HID_RI_REPORT_SIZE(8, 0x01),
        HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
        HID_RI_REPORT_COUNT(8, 0x01),
        HID_RI_REPORT_SIZE(8, 0x03),
        HID_RI_OUTPUT(8, HID_IOF_CONSTANT),

        HID_RI_USAGE_PAGE(8, 0x07), /* Key Codes */
        HID_RI_USAGE_MINIMUM(8, 0x00), /* Keyboard 0 */
        HID_RI_USAGE_MAXIMUM(8, (NKRO_BYTE_LEN-1)*8-1), /* Keyboard Right GUI */
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(8, 0x01),
        HID_RI_REPORT_COUNT(8, (NKRO_BYTE_LEN-1)*8),
        HID_RI_REPORT_SIZE(8, 0x01),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),
};
#endif

#define REPORT_ID_KEYBOARD 0
enum {
  COMPOSITE_REPORT_ID_KEYBOARD = 1,
  COMPOSITE_REPORT_ID_MOUSE,
  COMPOSITE_REPORT_ID_SYSTEM,
  COMPOSITE_REPORT_ID_CONSUMER,
  COMPOSITE_REPORT_ID_ABS_MOUSE,
};
enum {
  COMPOSITE_REPORT_INDEX_KEYBOARD = 0,
  COMPOSITE_REPORT_INDEX_MOUSE,
  COMPOSITE_REPORT_INDEX_SYSTEM,
  COMPOSITE_REPORT_INDEX_CONSUMER,
  COMPOSITE_REPORT_INDEX_ABS_MOUSE,
  COMPOSITE_REPORT_COUNT,
};

const uint8_t CompositeReportMap[] =
{
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x06), /* Keyboard */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, COMPOSITE_REPORT_ID_KEYBOARD),
        HID_RI_USAGE_PAGE(8, 0x07), /* Key Codes */
        HID_RI_USAGE_MINIMUM(8, 0xE0), /* Keyboard Left Control */
        HID_RI_USAGE_MAXIMUM(8, 0xE7), /* Keyboard Right GUI */
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(8, 0x01),
        HID_RI_REPORT_COUNT(8, 0x08),
        HID_RI_REPORT_SIZE(8, 0x01),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),

        HID_RI_REPORT_COUNT(8, 0x01),
        HID_RI_REPORT_SIZE(8, 0x08),
        HID_RI_INPUT(8, HID_IOF_CONSTANT),  /* reserved */

        HID_RI_USAGE_PAGE(8, 0x08), /* LEDs */
        HID_RI_USAGE_MINIMUM(8, 0x01), /* Num Lock */
        HID_RI_USAGE_MAXIMUM(8, 0x05), /* Kana */
        HID_RI_REPORT_COUNT(8, 0x05),
        HID_RI_REPORT_SIZE(8, 0x01),
        HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
        HID_RI_REPORT_COUNT(8, 0x01),
        HID_RI_REPORT_SIZE(8, 0x03),
        HID_RI_OUTPUT(8, HID_IOF_CONSTANT),

        HID_RI_USAGE_PAGE(8, 0x07), /* Keyboard */
        HID_RI_USAGE_MINIMUM(8, 0x00), /* Reserved (no event indicated) */
        HID_RI_USAGE_MAXIMUM(8, 0xFF), /* Keyboard Application */
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(16, 0x00FF),
        HID_RI_REPORT_COUNT(8, 0x06),
        HID_RI_REPORT_SIZE(8, 0x08),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),

    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x02), /* Mouse */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, COMPOSITE_REPORT_ID_MOUSE),
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

    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x80), /* System Control */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, COMPOSITE_REPORT_ID_SYSTEM),
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
        HID_RI_REPORT_ID(8, COMPOSITE_REPORT_ID_CONSUMER),
        HID_RI_LOGICAL_MINIMUM(16, 0x0001),
        HID_RI_LOGICAL_MAXIMUM(16, 0x029C),
        HID_RI_USAGE_MINIMUM(16, 0x0001), /* +10 */
        HID_RI_USAGE_MAXIMUM(16, 0x029C), /* AC Distribute Vertically */
        HID_RI_REPORT_SIZE(8, 16),
        HID_RI_REPORT_COUNT(8, 1),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),

    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x02), /* Mouse */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, COMPOSITE_REPORT_ID_ABS_MOUSE),
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


// TMK includes
#include "host.h"
#include "debug.h"
#include "suspend.h"
#ifdef SLEEP_LED_ENABLE
#include "sleep_led.h"
#include "led.h"
#endif
#include "wait.h"

#ifdef NKRO_ENABLE
  #include "keycode_config.h"

  extern keymap_config_t keymap_config;
#endif

/* ---------------------------------------------------------
 *       Global interface variables and declarations
 * ---------------------------------------------------------
 */


extern uint8_t keyboard_idle;
extern uint8_t keyboard_protocol;
extern uint16_t keyboard_led_stats;

report_keyboard_t keyboard_report_sent = {{0}};
#ifdef MOUSE_ENABLE
report_mouse_t mouse_report_blank = {0};
#define MOUSE_REPORT_SIZE 5
#define ABSMOUSE_REPORT_SIZE 2
#else
#define MOUSE_REPORT_SIZE 0
#define ABSMOUSE_REPORT_SIZE 0
#endif /* MOUSE_ENABLE */
#ifdef EXTRAKEY_ENABLE
#define REPORT_SYSTEM_INDEX   0
#define REPORT_CONSUMER_INDEX 1
#define SYSTEM_REPORT_SIZE 2
#define CONSUMER_REPORT_SIZE 2
#else
#define SYSTEM_REPORT_SIZE 0
#define CONSUMER_REPORT_SIZE 0
uint8_t extra_report_blank[3] = {0};
#endif /* EXTRAKEY_ENABLE */

#endif /* TMK_CORE_PROTOCOL_NRF_BLE_REPORT_DEF_H_ */
