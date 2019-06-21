/*
 * watchdog.h
 *
 * Created: 17.06.2019 23:04:25
 *  Author: ThePetrovich
 */ 

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include <avr/wdt.h>

void wdt_saveMCUCSR(void) __attribute__((naked)) __attribute__((section(".init3")));
//void wdt_disableWatchdog();
void wdt_enableWatchdog();

#endif /* WATCHDOG_H_ */