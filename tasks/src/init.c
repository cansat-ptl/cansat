/*
 * init.c
 *
 * Created: 14.06.2019 19:30:23
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"
#include "../../kernel/globals.h"

uint16_t tflags = 0;

void init(){
	logMessage((char *)PSTR("TaskMgr: Init task\r\n"), 1, 1);
	enableInterrupts();
	kernel_addTask(checkDeployment, 10);
	kernel_addTask(readADXL, 20);
	kernel_addTask(readBMP, 40);
	kernel_addTask(readGPS, 60);
	kernel_addTask(sendGPS, 80);
	kernel_addTask(sendMain, 100);
	kernel_addTask(readDS18, 70);
	//addTimedTask(repeatedTask1, 10);
}