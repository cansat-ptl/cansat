/*
 * powerctrl.c
 *
 * Created: 15.06.2019 17:20:59
 *  Author: Admin
 */ 

#include "tasks.h"
#define DEBUG 1
void powerctrl(){
	#ifdef DEBUG
		logMessage("Power check\r\n", 1);
	#endif
	uint16_t pwr = adc_read(0);
	if(pwr <=  255){
		#ifdef DEBUG
			logMessage("Status: powersave\r\n", 2);
		#endif
		setBit_m(tflags, PWSAVE);
		writePin(&PORTC, PC0, LOW);
	}
	else{
		#ifdef DEBUG
			logMessage("Status: OK\r\n", 2);
		#endif
		if(checkBit_m(tflags, CAM_ON))
			writePin(&PORTC, PC0, HIGH);
		clearBit_m(tflags, PWSAVE);
	}
	kernel_addTask(powerctrl, 5);
}