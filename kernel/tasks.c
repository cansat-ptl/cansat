/*
 * tasks.c
 *
 * Created: 11.05.2019 21:13:13
 *  Author: ThePetrovich
 */ 

#include "tasks.h"
#include "globals.h"
extern volatile char tx0_buffer[128]; 

void idle(){
//	sprintf((char*)&tx0_buffer, "Idle\r\n");
//	uart0_transmit();
}

void readData(){
	sprintf((char*)&tx0_buffer, "[TASK]TaskMgr: Reading\r\n");
	uart0_transmit();
	addTimedTask(process, 50);
}

void formPacket(){
	sprintf((char*)&tx0_buffer, "[TASK]TaskMgr: Forming packet\r\n");
	uart0_transmit();
}

void process(){
	sprintf((char*)&tx0_buffer, "[TASK]TaskMgr: Processing\r\n");
	uart0_transmit();
	addTimedTask(formPacket, 25);
	addTimedTask(sendData, 45);
}

void sendData(){
	sprintf((char*)&tx0_buffer, "[TASK]TaskMgr: Transmitting\r\n");
	uart0_transmit();
	addTimedTask(readData, 100);
}

