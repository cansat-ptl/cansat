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
#include "../kernel/drivers.h"

#define STARTUP 2
#define PWSAVE 1
#define CAM_ON 0

struct GPS_t
{
	unsigned char valid;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char day;
	unsigned char month;
	unsigned char year;
	unsigned int  millisecond;
	unsigned int cource;
	float latitude;
	unsigned char P;
	float longitude;
	float speed;
	float vspeed;
	unsigned char J;
	unsigned char Sats;
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

inline int idle();

inline int idle(){
	hal_nop();
	return 0;
}

void init();
int systemInit();
void initTaskManager();

int adxl345_test();
int bmp280_test();
int ds18b20_test();
int imu_test();
int gps_test();
int adc_test();
int autotest();
void getTestValues();

int powerCtrl();
int checkDeployment();

int readBMP();
int readADXL();
int readDS18();
int requestDS18();
int readGPS();

int sendGPS();
int sendMain();
int sendOrient();

int imu_setupTimer();
int imu_filter();

#endif /* TASKS_H_ */