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
#include "../kernel/kernel.h"

inline void idle();

inline void idle(){
	nop();
}

void init();
void adxl345_test();
void bmp280_test();
void ds18b20_test();

#endif /* TASKS_H_ */