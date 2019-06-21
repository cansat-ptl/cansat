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
		logMessage((char *)PSTR("Power check\r\n"), 1, 1);
	}
	uint16_t pwr = adc_read(0);
	sprintf(packetMain.vbat, "VBAT=%d;", pwr);
	if(pwr <=  255){
		if(debug == 1){
			logMessage((char *)PSTR("Status: powersave\r\n"), 2, 1);
		}
		setBit_m(tflags, PWSAVE);
		writePin(&PORTC, PC0, LOW);
	}
	else{
		if(debug == 1){
			logMessage((char *)PSTR("Status: OK\r\n"), 1, 1);
		}
		if(checkBit_m(tflags, CAM_ON)){
			writePin(&PORTC, PC0, HIGH);
			if(debug == 1){
				logMessage((char *)PSTR("Camera online\r\n"), 1, 1);
			}
		}
		clearBit_m(tflags, PWSAVE);
	}
	kernel_addTask(powerCtrl, 5);
	wdt_reset();
}

void checkDeployment(){
	if(debug == 1){
		logMessage((char *)PSTR("Reading light sensor\r\n"), 1, 1);
	}
	uint16_t light = adc_read(1);
	if(light <= 255){
		setBit_m(tflags, CAM_ON);
		if(debug == 1){
			logMessage((char *)PSTR("Status: deployed, removing task\r\n"), 1, 1);
		}
		kernel_addTask(powerCtrl, 5);
	}
	else {
		clearBit_m(tflags, CAM_ON);
		if(debug == 1){
			logMessage((char *)PSTR("Status: stowed\r\n"), 1, 1);
		}
		kernel_addTask(checkDeployment, 5);
	}	
	wdt_reset();
}