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
	hal_writePin(&LED_TX_PORT, LED_TX, HIGH);
	sprintf(packetGPS.marker, "YKTSAT5:GPS:");
	sprintf(packetGPS.n, "N=%lu;", ++gpsn);
	sprintf(packetGPS.et, "ET=%lu;", (uint32_t)e_time/1000);
	sprintf(packetGPS.end, "\r\n");
	debug_logMessage((char *)PSTR("Sent GPS packet\r\n"), 1, 1);
	debug_logMessage(packetGPS.marker, 0, 0);
	nRF_send_other(packetGPS.marker);
	debug_logMessage(packetGPS.n, 0, 0);
	nRF_send_other(packetGPS.n);
	debug_logMessage(packetGPS.et, 0, 0);
	nRF_send_other(packetGPS.et);
	debug_logMessage(packetGPS.sat, 0, 0);
	nRF_send_other(packetGPS.sat);
	debug_logMessage(packetGPS.lat, 0, 0);
	nRF_send_other(packetGPS.lat);
	debug_logMessage(packetGPS.lon, 0, 0);
	nRF_send_other(packetGPS.lon);
	debug_logMessage(packetGPS.alt, 0, 0);
	nRF_send_other(packetGPS.alt);
	debug_logMessage(packetGPS.end, 0, 0);
	nRF_send_other(packetGPS.end);
	kernel_addTask(sendGPS, 250);
	hal_writePin(&LED_TX_PORT, LED_TX, LOW);
}

void sendMain(){
	hal_writePin(&LED_TX_PORT, LED_TX, HIGH);
	sprintf(packetMain.marker, "YKTSAT5:MAIN:");
	sprintf(packetMain.n, "N=%lu;", ++mainn);
	sprintf(packetMain.et, "ET=%lu;", (uint32_t)e_time/1000);
	sprintf(packetMain.end, "\r\n");
	debug_logMessage((char *)PSTR("Sent main packet\r\n"), 1, 1);
	debug_logMessage(packetMain.marker, 0, 0);
	nRF_send_other(packetMain.marker);
	debug_logMessage(packetMain.n, 0, 0);
	nRF_send_other(packetMain.n);
	debug_logMessage(packetMain.et, 0, 0);
	nRF_send_other(packetMain.et);
	debug_logMessage(packetMain.vbat, 0, 0);
	nRF_send_other(packetMain.vbat);
	debug_logMessage(packetMain.alt, 0, 0);
	nRF_send_other(packetMain.alt);
	debug_logMessage(packetMain.prs, 0, 0);
	nRF_send_other(packetMain.prs);
	debug_logMessage(packetMain.t1, 0, 0);
	nRF_send_other(packetMain.t1);
	debug_logMessage(packetMain.t2, 0, 0);
	nRF_send_other(packetMain.t2);
	debug_logMessage(packetMain.end, 0, 0);
	nRF_send_other(packetMain.end);
	kernel_addTask(sendMain, 750);
	hal_writePin(&LED_TX_PORT, LED_TX, LOW);
}

void sendOrient(){
	hal_writePin(&LED_TX_PORT, LED_TX, HIGH);
	sprintf(packetOrient.marker, "YKTSAT5:ORIENT:");
	sprintf(packetOrient.n, "N=%lu;", ++orientn);
	sprintf(packetOrient.et, "ET=%lu;", (uint32_t)e_time/1000);
	sprintf(packetOrient.end, "\r\n");
	debug_logMessage((char *)PSTR("Sent orientation packet\r\n"), 1, 1);
		debug_logMessage(packetOrient.marker, 0, 0);
	nRF_send_other(packetOrient.marker);
		debug_logMessage(packetOrient.n, 0, 0);
	nRF_send_other(packetOrient.n);
		debug_logMessage(packetOrient.et, 0, 0);
	nRF_send_other(packetOrient.et);
		debug_logMessage(packetOrient.ax, 0, 0);
	nRF_send_other(packetOrient.ax);
		debug_logMessage(packetOrient.ay, 0, 0);
	nRF_send_other(packetOrient.ay);
		debug_logMessage(packetOrient.az, 0, 0);
	nRF_send_other(packetOrient.az);
		debug_logMessage(packetOrient.pitch, 0, 0);
	nRF_send_other(packetOrient.pitch);
		debug_logMessage(packetOrient.yaw, 0, 0);
	nRF_send_other(packetOrient.yaw);
		debug_logMessage(packetOrient.roll, 0, 0);
	nRF_send_other(packetOrient.roll);
		debug_logMessage(packetOrient.end, 0, 0);
	nRF_send_other(packetOrient.end);
	kernel_addTask(sendOrient, 250);
	hal_writePin(&LED_TX_PORT, LED_TX, LOW);
}