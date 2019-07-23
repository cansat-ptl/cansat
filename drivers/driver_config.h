/*	
 * driver_config.h	
 *	
 * Created: 22.06.2019 0:23:42	
 *  Author: ThePetrovich	
 */ 	

 #ifndef DRIVER_CONFIG_H_	
#define DRIVER_CONFIG_H_	

#include "../kernel/kernel.h"
#include "../kernel/hal.h"

#ifndef F_CPU 	
#define F_CPU 8000000L		
#endif	

 //-------------------------SPI-settings---------------------------//	
#ifndef SPIconfig	
#define SPIconfig	
//SPI module port registers	
#define SPI_PORT PORTB	
#define SPI_DDR DDRB	
//SPI module pins	
#define SPI_MISO 3	
#define SPI_MOSI 2	
#define SPI_SCK 1	
#define SPI_SS 0	
//SPI module default settings	
#define SPIEN 1			//SPE bit of SPCR - SPI module enable	
#define SPIIEN 0		//SPIE bit of SPCR - SPI interrupt enable	
#define SPIDORD 0		//DORD bit of SPCR - default SPI bit order	
#define SPIMSTR 1		//MSTR bit of SPCR - SPI master mode enable	
#define SPIMODEL 0		//CPOL bit of SPCR - SPI clock polarity	
#define SPIMODEH 0		//CPHA bit of SPCR - SPI clock phase	
#define SPISPDL 0		//SPR0 bit of SPCR - SPI speed prescaler bit 0	
#define SPISPDH 0		//SPR1 bit of SPCR - SPI speed prescaler bit 1	
#endif	
//----------------------------------------------------------------//	

 //-------------------------UART-settings--------------------------//	
#ifndef UARTconfig	
#define UARTconfig	
#define BAUD 9600							//UART baud rate	
#define UBRRVAL ((F_CPU/(16*BAUD)) - 1)		//UBBR value	
//UART module default settings	
#define RX0EN 1								//RX0 enable	
#define TX0EN 1								//TX0 enable	
#define RX0IE 1								//RX0 byte received interrupt enable	
#define TX0IE 0								//TX0 byte transmit interrupt enable	
#define RX1EN 1								//RX0 enable	
#define TX1EN 1								//TX0 enable	
#define RX1IE 1								//RX0 byte received interrupt enable	
#define TX1IE 0								//TX0 byte transmit interrupt enable	
#endif	
//----------------------------------------------------------------//	

 //-----------------------Device-settings--------------------------//	
#ifndef ADXL345config	
#define ADXL345config	
#define ADXL345_VALUE_POWERCTL 0x08	
#define ADXL345_VALUE_FORMAT 0x03	

 #define ADXL345_PORT PORTE	
#define ADXL345_DDR DDRE	
#define ADXL345_CS PE3	
#endif	
//----------------------------------------------------------------//	
#ifndef BMP280config	
#define BMP280config	

 #define BMP280_PORT PORTE	
#define BMP280_DDR DDRE	
#define BMP280_CS PE4	
#endif	
//----------------------------------------------------------------//	
#ifndef DS18B20config	
#define DS18B20config	

#define DS18B20_PORT PORTE	
#define DS18B20_DDR DDRE	
#define DS18B20_PIN PINE	
#define DS18B20_IO PE2	
#endif	
//----------------------------------------------------------------//	
#ifndef NRF24config	
#define NRF24config	
#define NRF24_CONFIG 0x0E	
#define NRF24_CHANNEL 40	
#define NRF24_RFSET 0x0F	
#define NRF24_AA_EN 0x01	
#define NRF24_RXADDR 0x01	

 #define NRF_CE_PORT PORTA	
#define NRF_CSN_PORT PORTA	
#define NRF_CE_DDR DDRA	
#define NRF_CSN_DDR DDRA	
#define NRF_CE PA5	
#define NRF_CSN PA6	
#endif	
//----------------------------------------------------------------//	

 #endif /* DRIVER_CONFIG_H_ */ 