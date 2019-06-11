/*
 * nrf24.c
 *
 * Created: 07.06.2019 23:14:39
 *  Author: ThePetrovich
 */ 
#include "nrf24.h"

void nrf24_init(){
	NRF_CE_DDR |= (1 << NRF_CE);
	NRF_CSN_DDR |= (1 << NRF_CSN);
}

void nrf24_setupTx(){
	spi_busSetup(SPI_PRESCALER_4, MSBFIRST, SPI_MODE0, SPI_1X);
	NRF_CE_PORT |= (1 << NRF_CE);
	_delay_ms(10);
	NRF_CE_PORT &= ~(1 << NRF_CE);
	NRF_CSN_PORT &= ~(1 << NRF_CSN);
	
	spi_write(NRF_FLUSH);
	spi_writeRegister(NRF_STATUS_REG, 0x30, 0x20);
	
	uint8_t buffer[5];
	buffer[0] = 0xE7;
	buffer[1] = 0xE7;
	buffer[2] = 0xE7;
	buffer[3] = 0xE7;
	buffer[4] = 0x01;
	spi_transfer(SPI_WRITE, NRF_TX_REG, buffer, 5, 0x20);
	
	buffer[0] = 0xE7;
	buffer[1] = 0xE7;
	buffer[2] = 0xE7;
	buffer[3] = 0xE7;
	buffer[4] = 0x00;
	spi_transfer(SPI_WRITE, NRF_RX_REG_P0, buffer, 5, 0x20);
	
	spi_writeRegister(NRF_AA_REG, 0x01, 0x20);
	spi_writeRegister(NRF_RXADDR_REG, 0x01, 0x20);
	spi_writeRegister(NRF_CH_REG, 40, 0x20);
	spi_writeRegister(NRF_RFSET_REG, 0x0F, 0x20);
	spi_writeRegister(NRF_CONFIG_REG, 0x0E, 0x20);
	spi_busStop();
}

void nrf24_transmit(char * data, uint8_t size){
	spi_busSetup(SPI_PRESCALER_4, MSBFIRST, SPI_MODE0, SPI_1X);
	spi_transfer(SPI_WRITE, NRF_TX_UPLOAD, (uint8_t *)data, size-1, 0x00);
	_delay_ms(2);
	NRF_CE_PORT |= (1 << NRF_CE);
	_delay_ms(1);
	NRF_CE_PORT &= ~(1 << NRF_CE);
	spi_busStop();
}