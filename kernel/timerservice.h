/*
 * timerservice.h
 *
 * Created: 12.05.2019 17:36:28
 *  Author: Admin
 */ 


#ifndef TIMERSERVICE_H_
#define TIMERSERVICE_H_

#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <stdint.h>
#include "kernel.h"

void startTimer();
void timerService();

#endif /* TIMERSERVICE_H_ */