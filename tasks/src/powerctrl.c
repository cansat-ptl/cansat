/*
 * powerctrl.c
 *
 * Created: 15.06.2019 17:20:59
 *  Author: Admin
 */ 

#include "../tasks.h"
#include "../../kernel/globals.h"

void powerCtrl(){
	if(debug == 1){
		debug_logMessage((char *)PSTR("Power check\r\n"), 1, 1);
	}
	uint16_t pwr = adc_read(0);
	sprintf(packetMain.vbat, "VBAT=%d;", pwr);
	if(pwr <=  255){
		if(debug == 1){
			debug_logMessage((char *)PSTR("Status: powersave\r\n"), 2, 1);
		}
		hal_setBit_m(tflags, PWSAVE);
		hal_writePin(&PORTB, PB4, LOW);
	}
	else{
		if(debug == 1){
			debug_logMessage((char *)PSTR("Status: OK\r\n"), 1, 1);
		}
		if(hal_checkBit_m(tflags, CAM_ON)){
			hal_writePin(&PORTB, PB4, HIGH);
			if(debug == 1){
				debug_logMessage((char *)PSTR("Camera online\r\n"), 1, 1);
			}
		}
		hal_clearBit_m(tflags, PWSAVE);
	}
	kernel_addTask(powerCtrl, 50);
	wdt_reset();
}

void checkDeployment(){
	if(debug == 1){
		debug_logMessage((char *)PSTR("Reading light sensor\r\n"), 1, 1);
	}
	uint16_t light = adc_read(1);
	if(light <= 20){
		hal_setBit_m(tflags, CAM_ON);
		if(debug == 1){
			debug_logMessage((char *)PSTR("Status: deployed, removing task\r\n"), 1, 1);
		}
		kernel_addTask(powerCtrl, 50);
	}
	else {
		hal_clearBit_m(tflags, CAM_ON);
		if(debug == 1){
			debug_logMessage((char *)PSTR("Status: stowed\r\n"), 1, 1);
		}
		kernel_addTask(checkDeployment, 50);
	}	
	wdt_reset();
}