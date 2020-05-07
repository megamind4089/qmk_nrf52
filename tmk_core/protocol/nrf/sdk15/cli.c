
#include <stdlib.h>

#include "microshell/core/microshell.h"
#include "microshell/core/msconf.h"
#include "microshell/util/msopt.h"
#include "microshell/util/mscmd.h"
#include "microshell/util/ntlibc.h"

#include "nrf_log.h"
#include "nrf_log.h"
#include "peer_manager.h"

#include "usbd.h"
#include "cli.h"
#include "bootloader.h"
#include "app_ble_func.h"
#include "ble_common.h"
#include "ble_central.h"
#include "ble_dongle.h"

void cli_puts(char *str);
static MSCMD_USER_RESULT usrcmd_reset(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_advertise(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_scan(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_bootloader(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_bonding_information(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_delete_bonding(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_help(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);

static const MSCMD_COMMAND_TABLE table[] = {
    {   "reset",   usrcmd_reset, "Reset system"   },
    {   "adv",   usrcmd_advertise, "Start advertising"   },
    {   "scan",   usrcmd_scan, "Start scanning. scan <wl_enable>"   },
    {   "dfu",   usrcmd_bootloader, "Jump to bootloader"   },
    {   "show",   usrcmd_bonding_information, "Show bonded devices"  },
    {   "del",   usrcmd_delete_bonding, "Delete bond information"   },
    {   "help",     usrcmd_help, "Show this message"     },
};

void cli_puts(char *str) {
  usbd_send_cdc_acm((uint8_t*)str, strlen(str));
}
static MSCMD_USER_RESULT usrcmd_reset(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  NVIC_SystemReset();
  return 0;
}
static MSCMD_USER_RESULT usrcmd_advertise(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  NRF_LOG_DEBUG("Adv");
#ifndef NRF_SEPARATE_KEYBOARD_SLAVE
  char arg[4];
  if (msopt->argc >= 2) {
    msopt_get_argv(msopt, 1, arg, sizeof(arg));
    if(arg[0] >='0' && arg[0] <= '9') {
      restart_advertising_id(arg[0]-'0');
    }
  } else {
    restart_advertising_wo_whitelist();
  }
#endif
  return 0;
}

static MSCMD_USER_RESULT usrcmd_scan(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
#ifdef NRF_SEPARATE_KEYBOARD_DONGLE
  if (msopt->argc >= 2) {
    scan_start_wo_whitelist();
  } else {
    scan_start();
  }
#endif
  return 0;
}

static MSCMD_USER_RESULT usrcmd_bootloader(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  NRF_LOG_DEBUG("DFU");
  bootloader_jump();
  return 0;
}

static MSCMD_USER_RESULT usrcmd_bonding_information(MSOPT *msopt,
    MSCMD_USER_OBJECT usrobj) {
  pm_peer_id_t peers[8];
  uint32_t peer_cnt;

  memset(peers, PM_PEER_ID_INVALID, sizeof(peers));
  peer_cnt = (sizeof(peers) / sizeof(pm_peer_id_t));

  // Load all peers from flash and whitelist them.
  peer_list_get(peers, &peer_cnt);
  for (int i = 0; i < peer_cnt; i++) {
    cli_puts("Device ");
    cdc_acm_putc(peers[i]+'0');
    cli_puts("\r\n");
  }
  return 0;
}

static MSCMD_USER_RESULT usrcmd_delete_bonding(MSOPT *msopt,
    MSCMD_USER_OBJECT usrobj) {
  char arg[4];
  if (msopt->argc >= 2) {
    msopt_get_argv(msopt, 1, arg, sizeof(arg));
    if(arg[0] >= '0' && arg[0] <='9') {
      delete_bond_id(arg[0]-'0');
    }
  } else {
    delete_bonds();
  }
  return 0;
}

static MSCMD_USER_RESULT usrcmd_help(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  NRF_LOG_DEBUG("Help");
  for (int i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
    cli_puts(table[i].argv0);
    cdc_acm_putc('\t');
    cli_puts(table[i].desc);
    cli_puts("\r\n");
  }
  return 0;
}

static MICROSHELL microshell;
static MSCMD mscmd;
void cli_init() {

  microshell_init(&microshell, cdc_acm_putc, cdc_acm_byte_to_read, cdc_acm_getc,
      NULL);
  mscmd_init(&mscmd, table, sizeof(table) / sizeof(table[0]), NULL);
}

void cli_exec() {
  static char buf[MSCONF_MAX_INPUT_LENGTH];
  static char* cur=buf;
  static bool show_info=false;

  if(!show_info) {
    cdc_acm_putc('>');
    show_info=true;
  }
  cur = microshell_getline(&microshell, buf, cur, MSCONF_MAX_INPUT_LENGTH - (cur-buf));
  if (cur == buf+MSCONF_MAX_INPUT_LENGTH) {
    NRF_LOG_DEBUG("Exec command");
    MSCMD_USER_RESULT r;
    mscmd_execute(&mscmd, buf, &r);
    memset(buf, 0, MSCONF_MAX_INPUT_LENGTH);
    cur = buf;
    show_info=false;
  }
}
