/*
 * nmea_process.c
 *
 * Created: 09.06.2019 0:38:03
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"
#include "../../drivers/devices/sensors/nmea/nmea_parser.h"

/*void nmea_process(){
	for(int i = 0; i < rx1_pointer; i++){
		nmea_load(rx1_buffer[i]);
	}
}*/

#define UART1_ISR
ISR(USART1_RX_vect){
	char data = UDR1;
	nmea_load(data);
}