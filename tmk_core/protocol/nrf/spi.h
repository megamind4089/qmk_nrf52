/*
 * spi.h
 *
 *  Created on: 2019/04/27
 *      Author: sekigon
 */

#ifndef TMK_CORE_PROTOCOL_NRF_SPI_H_
#define TMK_CORE_PROTOCOL_NRF_SPI_H_

#include <stdint.h>

int spim_init();
int spim_start(uint8_t * p_tx_buffer, size_t tx_length, uint8_t * p_rx_buffer,
    size_t rx_length);


#endif /* TMK_CORE_PROTOCOL_NRF_SPI_H_ */
