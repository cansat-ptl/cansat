/*
 * init.c
 *
 * Created: 14.06.2019 19:30:23
 *  Author: Admin
 */ 
#include "../tasks.h"
#include "../../kernel/globals.h"

uint16_t tflags = 0;

void init(){
	logMessage((char *)PSTR("TaskMgr: Init task\r\n"), 1, 1);
	enableInterrupts();
	kernel_addTask(checkDeployment, 5);
	kernel_addTask(readADXL, 6);
	kernel_addTask(readBMP, 7);
	//kernel_addTask(readDS18, 8);
	//addTimedTask(repeatedTask1, 10);
}