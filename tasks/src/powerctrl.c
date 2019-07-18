/*
 * powerctrl.c
 *
 * Created: 15.06.2019 17:20:59
 *  Author: Admin
 */ 

#include "../tasks.h"

void powerCtrl(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Power check\r\n"));
	}
	uint16_t pwr = (float)adc_read(0) * 0.98;
	sprintf(packetMain.vbat, "VBAT=%d;", pwr);
	if(pwr <=  780){
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage(PGM_ON, L_WARN, (char *)PSTR("Status: powersave\r\n"));
		}
		hal_setBit_m(tflags, PWSAVE);
		hal_writePin(&PORTB, PB4, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
	}
	else{
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Status: OK\r\n"));
		}
		if(hal_checkBit_m(tflags, CAM_ON)){
			hal_writePin(&PORTB, PB4, HIGH);
			hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
			if(kernel_checkFlag(KFLAG_DEBUG)){
				debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Camera online\r\n"));
			}
		}
		hal_clearBit_m(tflags, PWSAVE);
	}
	kernel_addTask(powerCtrl, 10, PRIORITY_HIGH);
	wdt_reset();
}

void checkDeployment(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Reading light sensor\r\n"));
	}
	uint16_t light = adc_read(1);
	debug_logMessage(PGM_OFF, L_INFO, "LIGHT = %d\r\n", light);
	if(light <= 200 || (altitude - altitude_init) > 150){
		hal_setBit_m(tflags, CAM_ON);
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Status: deployed, removing task\r\n"));
		}
		kernel_addTask(powerCtrl, 10, PRIORITY_HIGH);
	}
	else {
		hal_clearBit_m(tflags, CAM_ON);
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Status: stowed\r\n"));
		}
		kernel_addTask(checkDeployment, 10, PRIORITY_HIGH);
	}	
	wdt_reset();
}