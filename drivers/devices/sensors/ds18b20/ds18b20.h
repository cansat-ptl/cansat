/*
 * ds18b20.h
 *
 * Created: 17.05.2019 17:58:22
 *  Author: ThePetrovich
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_

#define DS18DRV_VER "0.0.2-bleeding"
#define DS18DRV_TIMESTAMP __TIMESTAMP__
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "../../../driver_config.h"

#define DS_CMD_CONVERTTEMP 0x44
#define DS_CMD_RSCRATCHPAD 0xBE
#define DS_CMD_WSCRATCHPAD 0x4E
#define DS_CMD_CPYSCRATCHPAD 0x48
#define DS_CMD_RECEEPROM 0xB8
#define DS_CMD_RPWRSUPPLY 0xB4
#define DS_CMD_SEARCHROM 0xF0
#define DS_CMD_READROM 0x33
#define DS_CMD_MATCHROM 0x55
#define DS_CMD_SKIPROM 0xCC
#define DS_CMD_ALARMSEARCH 0xEC

#define DS_RESOLUTION_12BIT 625

char * ds18b20_readTemperature(void);
void ds18b20_requestTemperature(void);

#endif /* DS18B20_H_ */