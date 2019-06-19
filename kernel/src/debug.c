/*
 * debug.c
 *
 * Created: 02.06.2019 20:53:16
 *  Author: ThePetrovich
 */ 
#include "../debug.h"
#include "../globals.h"
#define SD_LOGGING 1

char levels[5][16] = {
	"",
	"[INFO]",
	"[WARN]",
	"[ERROR]",
	"[FATAL]"
};

inline void debugMessage(char* msg, uint8_t level) {
	if(level != 0){
		char buffer[48];
		sprintf(buffer, "%02d.%02d.%02d %02d:%02d:%02d ", GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
		uart0_puts(buffer);
	}
	uart0_puts(levels[level]);
	uart0_puts(msg);
}

inline void debugMessage_p(const char * msg, uint8_t level) {
	char buffer[128];
	if(level != 0){
		sprintf(buffer, "%02d.%02d.%02d %02d:%02d:%02d ", GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
		uart0_puts(buffer);
	}
	sprintf_P(buffer, PSTR("%S"), msg);
	uart0_puts(levels[level]);
	uart0_puts(buffer);
}

inline void debugMessage_i(char* msg, uint8_t level) {
	sprintf((char*)&tx0_buffer, "%s%s", levels[level], msg);
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY));
}

inline void debugMessage_pi(const char * msg, uint8_t level) {
	sprintf((char*)&tx0_buffer, "%s%s", levels[level], msg);
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY));
}

void debugMessageSD(char* msg, uint8_t level){
//	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	/*FRESULT res;
	uint16_t btw;
	res = pf_open("/debug.log");
	if(res == FR_OK){
		if(level != 0){
			char buffer[48];
			sprintf(buffer, "%02d.%02d.%02d %02d:%02d:%02d ", GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
			btw = strlen(buffer);
			pf_write(buffer, btw, &s1);
		}
		btw = strlen(levels[level]);
		pf_write(levels[level], btw, &s1);
		btw = strlen(msg);
		pf_write(msg, btw, &s1);
	}
	else debugMessage_p(PSTR("Could not open /debug.log, SD card failure\r\n"), 3);
	*/
//	spi_busStop();
}

void debugMessageSD_p(const char * msg, uint8_t level){
//	spi_busSetup(SPI_PRESCALER_4, LSBFIRST, SPI_MODE3, SPI_1X);
	/*char buffer[128];
	FRESULT res;
	uint16_t btw;
	res = pf_open("/debug.log");
	pf_lseek(0);
	if(res == FR_OK){
		if(level != 0){
			sprintf(buffer, "%02d.%02d.%02d %02d:%02d:%02d ", GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
			btw = strlen(buffer);
			cli();
			pf_write(buffer, btw, &s1);
			sei();
			sd_ptr += s1;
		}
		btw = strlen(levels[level]);
		cli();
		pf_write(levels[level], btw, &s1);
		sei();
		sprintf_P(buffer, PSTR("%S"), msg);
		btw = strlen(buffer);
		cli();
		pf_write(buffer, btw, &s1);
		sei();
		pf_write(0, 0, &s1);
		sd_ptr++;
	}
	else debugMessage_p(PSTR("Could not open /debug.log, SD card failure\r\n"), 3);
//	spi_busStop();*/
}

inline void logMessage(char* msg, uint8_t level, uint8_t pgm){
	if(!pgm){
		#ifdef UART_LOGGING
			debugMessage(msg, level);
		#endif
		#ifdef UART_LOGGING_I
			debugMessage_i(msg, level);
		#endif
		#ifdef SD_LOGGING
			debugMessageSD(msg, level);
		#endif
	}
	else {
		#ifdef UART_LOGGING
			debugMessage_p(msg, level);
		#endif
		#ifdef UART_LOGGING_I
			debugMessage_pi(msg, level);
		#endif
		#ifdef SD_LOGGING
			debugMessageSD_p(msg, level);
		#endif
	}
}
