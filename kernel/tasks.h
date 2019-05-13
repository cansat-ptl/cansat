/*
 * tasks.h
 *
 * Created: 11.05.2019 23:54:14
 *  Author: ThePetrovich
 */


#ifndef TASKS_H_
#define TASKS_H_

#include <string.h>
#include <stdio.h>
#include "../drivers/uart.h"
#include "kernel.h"

void idle();
void readData();
void formPacket();
void process();
void sendData();

#endif /* TASKS_H_ */