/*
 * tasks.c
 *
 * Created: 11.05.2019 21:13:13
 *  Author: ThePetrovich
 */ 

#include "tasks.h"
extern volatile char tx0_buffer[128]; 

void idle(){
//	sprintf((char*)&tx0_buffer, "Idle\r\n");
//	uart0_transmit();
}

void readData(){
	sprintf((char*)&tx0_buffer, "Reading\r\n");
	uart0_transmit();
	addTimedTask(process, 10);
}

void formPacket(){
	sprintf((char*)&tx0_buffer, "Forming packet\r\n");
	uart0_transmit();
	addTimedTask(sendData, 10);
}

void process(){
	sprintf((char*)&tx0_buffer, "Processing\r\n");
	uart0_transmit();
	addTimedTask(formPacket, 10);
}

void sendData(){
	sprintf((char*)&tx0_buffer, "Transmitting\r\n");
	uart0_transmit();
	addTimedTask(readData, 10);
}