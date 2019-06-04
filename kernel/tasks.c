/*
 * tasks.c
 *
 * Created: 11.05.2019 21:13:13
 *  Author: ThePetrovich
 */ 
#include "tasks.h"
#include "globals.h"

void idle(){
	nop();
}

void init(){
	logMessage("TaskMgr: Init task\r\n", 1);
	enableInterrupts();
	addTask(readData, 10);
	//addTimedTask(repeatedTask1, 10);
}

void readData(){
//#ifdef DEBUG
	logMessage("TaskMgr: Reading\r\n", 1);
	enableInterrupts();
//#endif
	addTask(process, 5);
}

void formPacket(){
//#ifdef DEBUG
	logMessage("TaskMgr: Forming packet\r\n", 1);
	enableInterrupts();
//#endif
}

void repeatedTask(){
	logMessage("TaskMgr: Doing something\r\n", 1);
	enableInterrupts();
	//addTimedTask(repeatedTask, 5);
}

void repeatedTask1(){
	logMessage("TaskMgr: Doing something 2\r\n", 1);
	enableInterrupts();
	addTask(repeatedTask1, 5);
}
void process(){
	logMessage("TaskMgr: Processing\r\n", 1);
	enableInterrupts();
	addTask(formPacket, 2);
	addTask(sendData, 4);
}

void sendData(){
	logMessage("TaskMgr: Transmitting\r\n", 1);
	enableInterrupts();
	addTask(readData, 1);
}

