/*
 * powerctrl.c
 *
 * Created: 15.06.2019 17:20:59
 *  Author: Admin
 */ 

#include "../tasks.h"
#include "../../kernel/globals.h"

void powerCtrl(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage((char *)PSTR("Power check\r\n"), 1, 1);
	}
	uint16_t pwr = (float)adc_read(0) * 0.98;
	sprintf(packetMain.vbat, "VBAT=%d;", pwr);
	if(pwr <=  780){
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage((char *)PSTR("Status: powersave\r\n"), 2, 1);
		}
		hal_setBit_m(tflags, PWSAVE);
		hal_writePin(&PORTB, PB4, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
	}
	else{
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage((char *)PSTR("Status: OK\r\n"), 1, 1);
		}
		if(hal_checkBit_m(tflags, CAM_ON)){
			hal_writePin(&PORTB, PB4, HIGH);
			hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
			if(kernel_checkFlag(KFLAG_DEBUG)){
				debug_logMessage((char *)PSTR("Camera online\r\n"), 1, 1);
			}
		}
		hal_clearBit_m(tflags, PWSAVE);
	}
	kernel_addTask(powerCtrl, 50, PRIORITY_HIGH);
	wdt_reset();
}

void checkDeployment(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage((char *)PSTR("Reading light sensor\r\n"), 1, 1);
	}
	uint16_t light = adc_read(1);
	char buf[24];
	sprintf(buf, "LIGHT = %d\r\n", light);
	debug_logMessage(buf, 0, 0);
	if(light <= 200 || (altitude - altitude_init) > 150){
		hal_setBit_m(tflags, CAM_ON);
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage((char *)PSTR("Status: deployed, removing task\r\n"), 1, 1);
		}
		kernel_addTask(powerCtrl, 50, PRIORITY_HIGH);
	}
	else {
		hal_clearBit_m(tflags, CAM_ON);
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage((char *)PSTR("Status: stowed\r\n"), 1, 1);
		}
		kernel_addTask(checkDeployment, 50, PRIORITY_HIGH);
	}	
	wdt_reset();
}