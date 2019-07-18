/*
 * types.h
 *
 * Created: 12.05.2019 18:36:04
 *  Author: ThePetrovich
 */ 

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

typedef void (*task)(void);
typedef uint8_t byte;

struct taskStruct {
	task pointer;
	uint16_t period;
	uint8_t priority;
};

struct LSM_t
{
	unsigned char XH_A;
	unsigned char XL_A;
	unsigned char YH_A;
	unsigned char YL_A;
	unsigned char ZH_A;
	unsigned char ZL_A;
	unsigned char XH_M;
	unsigned char XL_M;
	unsigned char YH_M;
	unsigned char YL_M;
	unsigned char ZH_M;
	unsigned char ZL_M;
};

struct L3GD_t
{
	unsigned char XH;
	unsigned char XL;
	unsigned char YH;
	unsigned char YL;
	unsigned char ZH;
	unsigned char ZL;
};

#endif /* TYPES_H_ */