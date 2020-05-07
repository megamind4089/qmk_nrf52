

#pragma once

void ble_hids_c_init(void);
void scan_start_wo_whitelist(void);
void hids_c_data_handler(uint8_t const *dat, uint16_t len, uint16_t conn_handle,
                         uint16_t report_idx);
void hids_c_on_disconnect(void);
