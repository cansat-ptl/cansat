/*
 * config.h
 *
 * Created: 24.03.2019 16:41:43
 *  Author: ThePetrovich
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU 8000000L						//CPU frequency

//------------------------Project settings------------------------
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "kernel/drivers.h"
#include "kernel/kernel.h"
#include "kernel/tasks.h"
//----------------------------------------------------------------

//-------------------------UART settings--------------------------
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

//-------------------------SPI settings---------------------------
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

//-----------------------Kernel settings--------------------------
#define MAX_QUEUE_SIZE 16
#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
//----------------------------------------------------------------
#endif /* CONFIG_H_ */