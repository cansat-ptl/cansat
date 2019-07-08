/*
 * tasks.h
 *
 * Created: 11.05.2019 23:54:14
 *  Author: ThePetrovich
 */


#ifndef TASKS_H_
#define TASKS_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../kernel/kernel.h"

#define STARTUP 2
#define PWSAVE 1
#define CAM_ON 0

extern volatile struct GPS_t GPS;
extern volatile struct LSM_t LSM;
extern volatile struct L3GD_t L3GD;

extern uint16_t altitude_init;
extern uint16_t altitude;

extern uint16_t tflags;
extern uint16_t tests;

struct packetMain_t {
	char marker[31];
	char n[16];
	char et[16];
	char vbat[16];
	char alt[16];
	char prs[32];
	char t1[16];
	char t2[16];
	char end[4];
} packetMain;

struct packetOrient_t {
	char marker[31];
	char n[16];
	char et[16];
	char ax[16];
	char ay[16];
	char az[16];
	char pitch[16];
	char yaw[16];
	char roll[16];
	char end[4];
} packetOrient;

struct packetGPS_t {
	char marker[31];
	char n[16];
	char et[16];
	char sat[16];
	char lat[32];
	char lon[32];
	char alt[16];
	char end[4];
} packetGPS;

#define ADXL_TESTED 0
#define BMP_TESTED 1
#define DS_TESTED 2
#define GPS_TESTED 3
#define IMU_TESTED 4
#define ADC_TESTED 5

inline void idle();

inline void idle(){
	hal_nop();
}

void init();
void initTaskManager();

void adxl345_test();
void bmp280_test();
void ds18b20_test();
void imu_test();
void gps_test();
void adc_test();
void autotest();
void getTestValues();

void powerCtrl();
void checkDeployment();

void readBMP();
void readADXL();
void readDS18();
void requestDS18();
void readGPS();

void sendGPS();
void sendMain();
void sendOrient();

void imu_setupTimer();
void imu_filter();

#endif /* TASKS_H_ */