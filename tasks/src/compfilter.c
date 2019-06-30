/*
 * compfilter.c
 *
 * Created: 21.06.2019 11:23:26
 *  Author: ThePetrovich
 */

#include "../tasks.h" 
#include "../../kernel/globals.h" 

int16_t ax_g = 0, ay_g = 0, az_g = 0;
float pitch = 0, yaw = 0, roll = 0;

void imu_setupTimer(){
	hal_disableInterrupts();
	TCCR3B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); //prescaler 64
	TCNT3 = 0;
	OCR3A = 31250;
	TIMSK |= (1 << OCIE3A);
	hal_enableInterrupts();
}

void imu_filter(){
	float pitchAcc, rollAcc;
	int16_t gyrData_raw_x = 0, gyrData_raw_y = 0, gyrData_raw_z = 0;
	int16_t accData_raw_x = 0, accData_raw_y = 0, accData_raw_z = 0;
	
	imu_read();
	
	gyrData_raw_x |= (L3GD.XH << 8);
	gyrData_raw_x |= L3GD.XL;
	gyrData_raw_y |= (L3GD.YH << 8);
	gyrData_raw_y |= L3GD.YL;
	gyrData_raw_z |= (L3GD.ZH << 8);
	gyrData_raw_z |= L3GD.ZL;
	
	accData_raw_x |= (LSM.XH_A << 8);
	accData_raw_x |= LSM.XL_A;
	accData_raw_y |= (LSM.YH_A << 8);
	accData_raw_y |= LSM.YL_A;
	accData_raw_z |= (LSM.ZH_A << 8);
	accData_raw_z |= LSM.ZL_A;
	
	pitch += (((float)gyrData_raw_x / 16.384) * 0.07);
	roll -= (((float)gyrData_raw_z / 16.384) * 0.07);
	yaw -= (((float)gyrData_raw_y / 16.384) * 0.07);
	
	int forceMagnitudeApprox = abs(accData_raw_x) + abs(accData_raw_y) + abs(accData_raw_z);
	if (forceMagnitudeApprox > 1024 && forceMagnitudeApprox < 32768)
	{
		pitchAcc = atan2f((float)accData_raw_y, (float)accData_raw_z) * 180 / M_PI;
		pitch = pitch * 0.98 + pitchAcc * 0.02;
		
		rollAcc = atan2f((float)accData_raw_x, (float)accData_raw_y*-1) * 180 / M_PI;
		roll = roll * 0.98 + rollAcc * 0.02;
	}
	/*char msg[32];
	sprintf(msg, "PR: %f %f\r\n", pitch, roll);
	debug_logMessage(msg, 1, 0);
	kernel_addTask(imu_filter, 50);*/
	sprintf(packetOrient.pitch, "PITCH=%d;", (int)(pitch*10));
	sprintf(packetOrient.yaw, "YAW=%d;", 0);
	sprintf(packetOrient.roll, "ROLL=%d;", (int)(roll*10));
}

ISR(TIMER3_COMPA_vect){
	imu_filter();
}