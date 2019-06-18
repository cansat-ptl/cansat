/*
 * config.h
 *
 * Created: 24.03.2019 16:41:43
 *  Author: ThePetrovich
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU 8000000L						//CPU frequency

//#define DEBUG 1

//------------------------Project-settings------------------------
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "kernel/drivers.h"
#include "kernel/kernel.h"
#include "tasks/tasks.h"
#include "kernel/debug.h"
//----------------------------------------------------------------

//-------------------------UART-settings--------------------------
#define UARTconfig
#define BAUD 9600							//UART baud rate
#define UBRRVAL ((F_CPU/(16*BAUD)) - 1)		//UBBR value
#define RX0EN 1								//RX0 enable
#define TX0EN 1								//TX0 enable
#define RX0IE 1								//RX0 byte received interrupt enable
#define TX0IE 0								//TX0 byte transmit interrupt enable
#define RX1EN 1								//RX0 enable
#define TX1EN 1								//TX0 enable
#define RX1IE 1								//RX0 byte received interrupt enable
#define TX1IE 0								//TX0 byte transmit interrupt enable
//----------------------------------------------------------------

//-------------------------SPI-settings---------------------------
#define SPIconfig
//SPI module port registers
#define SPI_PORT PORTB
#define SPI_DDR DDRB	
//SPI module pins
#define SPI_MISO 3		//PB3 - MISO
#define SPI_MOSI 2		//PB2 - MOSI
#define SPI_SCK 1		//PB1 - SCLK
#define SPI_SS 0		//PB0 - CS (for slave mode)
//SPI module settings
#define SPIEN 1			//SPE bit of SPCR
#define SPIIEN 0		//SPIE bit of SPCR
#define SPIDORD 0		//DORD bit of SPCR
#define SPIMSTR 1		//MSTR bit of SPCR
#define SPIMODEL 0		//CPOL bit of SPCR
#define SPIMODEH 0		//CPHA bit of SPCR
#define SPISPDL 0		//SPR0 bit of SPCR
#define SPISPDH 0		//SPR1 bit of SPCR
//----------------------------------------------------------------

//-----------------------Device-settings--------------------------
#define NRF24config
//NRF24 pin config
#define NRF_CE_PORT PORTA
#define NRF_CSN_PORT PORTA
#define NRF_CE_DDR DDRA
#define NRF_CSN_DDR DDRA
#define NRF_CE PA5
#define NRF_CSN PA6
//NRF24 register config
#define NRF24_CONFIG 0x0E //something
#define NRF24_CHANNEL 40 //channel, calculated from (DEST_FREQ - 2400), e.g. 2440 - 2400 = 40
#define NRF24_RFSET 0x0F //RF power
#define NRF24_AA_EN 0x01 //Auto ack enable
#define NRF24_RXADDR 0x01 //Destination address

#define ADXL345config
//ADXL345 pin config
#define ADXL345_PORT PORTA
#define ADXL345_DDR DDRA
#define ADXL345_CS PA0
//ADXL345 register config
#define ADXL345_VALUE_POWERCTL 0x08 //main power control register, set to continuous measurement
#define ADXL345_VALUE_FORMAT 0x03 //data format register, set to 16g low accuracy


#define BMP280config
//BMP280 pin config
#define BMP280_PORT PORTA
#define BMP280_DDR DDRA
#define BMP280_CS PA7

#define DS18B20config
//DS18B20 pin config
#define DS18B20_PORT PORTA
#define DS18B20_DDR DDRA
#define DS18B20_PIN PINA
#define DS18B20_IO PA1
//----------------------------------------------------------------

//-----------------------Kernel-settings--------------------------
#define MAX_QUEUE_SIZE 24
#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
//----------------------------------------------------------------
#endif /* CONFIG_H_ */