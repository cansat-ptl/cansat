/*
 * init.c
 *
 * Created: 14.06.2019 19:30:23
 *  Author: Admin
 */ 
#include "tasks.h"
#include "../kernel/globals.h"

void init(){
	logMessage("TaskMgr: Init task\r\n", 1);
	enableInterrupts();
	//addTask(readData, 10);
	//addTimedTask(repeatedTask1, 10);
}