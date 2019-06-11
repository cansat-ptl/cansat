/*
 * nrf24.h
 *
 * Created: 07.06.2019 23:14:48
 *  Author: ThePetrovich
 */ 


#ifndef NRF24_H_
#define NRF24_H_

#include "../../spi.h"
#include <util/delay.h>

#define NRF_CE_PORT PORTA
#define NRF_CSN_PORT PORTA
#define NRF_CE_DDR DDRA
#define NRF_CSN_DDR DDRA
#define NRF_CE 5
#define NRF_CSN 6

#define NRF_TX_UPLOAD 0xA0
#define NRF_FLUSH 0xE1

#define NRF_CONFIG_REG 0x00
#define NRF_AA_REG 0x01
#define NRF_RXADDR_REG 0x02
#define NRF_CH_REG 0x05
#define NRF_RFSET_REG 0x06
#define NRF_STATUS_REG 0x07
#define NRF_RX_REG_P0 0x0A
#define NRF_TX_REG 0x10

void nrf24_init();
void nfr24_setupTx();
void nrf24_transmit(char * data, uint8_t size);

#endif /* NRF24_H_ */
