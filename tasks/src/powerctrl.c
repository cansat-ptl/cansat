/*
 * powerctrl.c
 *
 * Created: 15.06.2019 17:20:59
 *  Author: Admin
 */ 

#include "../tasks.h"

#define DEBUG 1
void powerCtrl(){
	#ifdef DEBUG
		logMessage(PSTR("Power check\r\n"), 1, 1);
	#endif
	uint16_t pwr = adc_read(0);
	sprintf(packetMain.vbat, "VBAT=%d;", pwr);
	if(pwr <=  255){
		#ifdef DEBUG
			logMessage(PSTR("Status: powersave\r\n"), 2, 1);
		#endif
		setBit_m(tflags, PWSAVE);
		writePin(&PORTC, PC0, LOW);
	}
	else{
		#ifdef DEBUG
			logMessage(PSTR("Status: OK\r\n"), 1, 1);
		#endif
		if(checkBit_m(tflags, CAM_ON)){
			writePin(&PORTC, PC0, HIGH);
			#ifdef DEBUG
				logMessage(PSTR("Camera online\r\n"), 1, 1);
			#endif
		}
		clearBit_m(tflags, PWSAVE);
	}
	kernel_addTask(powerCtrl, 5);
	wdt_reset();
}

void checkDeployment(){
	#ifdef DEBUG
		logMessage(PSTR("Reading light sensor\r\n"), 1, 1);
	#endif
	uint16_t light = adc_read(1);
	if(light <= 255){
		setBit_m(tflags, CAM_ON);
		#ifdef DEBUG
			logMessage(PSTR("Status: deployed, removing task\r\n"), 1, 1);
		#endif
		kernel_addTask(powerCtrl, 5);
	}
	else {
		clearBit_m(tflags, CAM_ON);
		#ifdef DEBUG
			logMessage(PSTR("Status: stowed\r\n"), 1, 1);
		#endif
		kernel_addTask(checkDeployment, 5);
	}	
	wdt_reset();
}