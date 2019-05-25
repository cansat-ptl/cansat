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
	addTimedTask(readData, 100);
	addTimedTask(repeatedTask, 100);
	addTimedTask(repeatedTask1, 100);
}

void readData(){
#ifdef DEBUG
	debugMessage("[INFO]TaskMgr: Reading\r\n");
#endif
	addTimedTask(process, 50);
}

void formPacket(){
#ifdef DEBUG
	debugMessage("[INFO]TaskMgr: Forming packet\r\n");
#endif
}

void repeatedTask(){
#ifdef DEBUG
	debugMessage("[INFO]TaskMgr: Doing something\r\n");
#endif
	addTimedTask(repeatedTask, 5);
}

void repeatedTask1(){
#ifdef DEBUG
	debugMessage("[INFO]TaskMgr: Doing something 2\r\n");
#endif
	addTimedTask(repeatedTask1, 5);
}
void process(){
#ifdef DEBUG
	debugMessage("[INFO]TaskMgr: Processing\r\n");
#endif
	addTimedTask(formPacket, 25);
	addTimedTask(sendData, 45);
}

void sendData(){
#ifdef DEBUG
	debugMessage("[INFO]TaskMgr: Transmitting\r\n");
#endif
	addTimedTask(readData, 100);
}

