/*
 * sendpacket.c
 *
 * Created: 16.06.2019 0:52:18
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"
#include "../../kernel/globals.h"
/*
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
} packetGPS;*/

uint32_t gpsn = 0, mainn = 0, orientn = 0;

void sendGPS(){
	sprintf(packetGPS.marker, "YKTSAT5:GPS:");
	sprintf(packetGPS.n, "N=%lu;", ++gpsn);
	sprintf(packetGPS.et, "ET=%d;", (int)e_time/1000);
	sprintf(packetGPS.end, "\r\n");
	logMessage((char *)PSTR("GPS telemetry: "), 1, 1);
	logMessage(packetGPS.marker, 0, 0);
	logMessage(packetGPS.n, 0, 0);
	logMessage(packetGPS.et, 0, 0);
	logMessage(packetGPS.sat, 0, 0);
	logMessage(packetGPS.lat, 0, 0);
	logMessage(packetGPS.lon, 0, 0);
	logMessage(packetGPS.alt, 0, 0);
	logMessage(packetGPS.end, 0, 0);
	kernel_addTask(sendGPS, 50);
}

void sendMain(){
	sprintf(packetMain.marker, "YKTSAT5:MAIN:");
	sprintf(packetMain.n, "N=%lu;", ++mainn);
	sprintf(packetMain.et, "ET=%d;", (int)e_time/1000);
	sprintf(packetMain.end, "\r\n");
	logMessage((char *)PSTR("Main telemetry: "), 1, 1);
	logMessage(packetMain.marker, 0, 0);
	logMessage(packetMain.n, 0, 0);
	logMessage(packetMain.et, 0, 0);
	logMessage(packetMain.vbat, 0, 0);
	logMessage(packetMain.alt, 0, 0);
	logMessage(packetMain.prs, 0, 0);
	logMessage(packetMain.t1, 0, 0);
	logMessage(packetMain.t2, 0, 0);
	logMessage(packetMain.end, 0, 0);
	kernel_addTask(sendMain, 100);
}