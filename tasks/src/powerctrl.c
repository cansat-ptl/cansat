/*
 * powerctrl.c
 *
 * Created: 15.06.2019 17:20:59
 *  Author: Admin
 */ 

#include "../tasks.h"

int powerCtrl(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("powerd: Power check\r\n"));
	}
	uint16_t pwr = (float)adc_read(0) * 0.98;
	sprintf(packetMain.vbat, "VBAT=%d;", pwr);
	if(pwr <=  780){
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage(PGM_ON, L_WARN, (char *)PSTR("powerd: Status: powersave\r\n"));
		}
		hal_setBit_m(tflags, PWSAVE);
		hal_writePin(&PORTB, PB4, LOW);
		hal_writePin(&LED_WRK_PORT, LED_WRK, LOW);
	}
	else{
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("powerd: Status: OK\r\n"));
		}
		if(hal_checkBit_m(tflags, CAM_ON)){
			hal_writePin(&PORTB, PB4, HIGH);
			hal_writePin(&LED_WRK_PORT, LED_WRK, HIGH);
			if(kernel_checkFlag(KFLAG_DEBUG)){
				debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("powerd: Camera online\r\n"));
			}
		}
		hal_clearBit_m(tflags, PWSAVE);
	}
	kernel_addTask(powerCtrl, 250, PRIORITY_HIGH, KSTATE_ACTIVE);
	wdt_reset();
	return 0;
}

int checkDeployment(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("powerd: Reading light sensor\r\n"));
	}
	uint16_t light = adc_read(1);
	debug_logMessage(PGM_OFF, L_INFO, "powerd: LIGHT = %d\r\n", light);
	if(light <= 200 || (altitude - altitude_init) > 150){
		hal_setBit_m(tflags, CAM_ON);
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("powerd: Status: deployed, removing task\r\n"));
		}
		kernel_addTask(powerCtrl, 250, PRIORITY_HIGH, KSTATE_ACTIVE);
	}
	else {
		hal_clearBit_m(tflags, CAM_ON);
		if(kernel_checkFlag(KFLAG_DEBUG)){
			debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("powerd: Status: stowed\r\n"));
		}
		kernel_addTask(checkDeployment, 250, PRIORITY_HIGH, KSTATE_ACTIVE);
	}	
	wdt_reset();
	return 0;
}