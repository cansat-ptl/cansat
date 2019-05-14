/*
 * tasks.c
 *
 * Created: 11.05.2019 21:13:13
 *  Author: ThePetrovich
 */ 

#include "tasks.h"
#include "globals.h"

void idle(){
	asm volatile ("NOP");
}

void init(){
	addTimedTask(readData, 100);
	addTimedTask(repeatedTask, 100);
	addTimedTask(repeatedTask1, 100);
}

void readData(){
	debugMessage("[TASK]TaskMgr: Reading\r\n");
	addTimedTask(process, 50);
}

void formPacket(){
	debugMessage("[TASK]TaskMgr: Forming packet\r\n");
}

void repeatedTask(){
	debugMessage("[TASK]TaskMgr: Doing something\r\n");
	addTimedTask(repeatedTask, 5);
}

void repeatedTask1(){
	debugMessage("[TASK]TaskMgr: Doing something 2\r\n");
	addTimedTask(repeatedTask1, 5);
}
void process(){
	debugMessage("[TASK]TaskMgr: Processing\r\n");
	addTimedTask(formPacket, 25);
	addTimedTask(sendData, 45);
}

void sendData(){
	debugMessage("[TASK]TaskMgr: Transmitting\r\n");
	addTimedTask(readData, 100);
}

