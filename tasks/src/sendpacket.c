/*
 * sendpacket.c
 *
 * Created: 16.06.2019 0:52:18
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"
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

int sendGPS(){
	hal_writePin(&PORTC, PC6, 1);
	sprintf(packetGPS.marker, "YKTSAT5:GPS:");
	sprintf(packetGPS.n, "N=%lu;", ++gpsn);
	sprintf(packetGPS.et, "ET=%lu;", (uint32_t)kernel_getUptime()/1000);
	sprintf(packetGPS.end, "\r\n");
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("sensord: Sent GPS packet\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("%s%s%s%s%s%s%s%s"), packetGPS.marker, packetGPS.n, packetGPS.et, packetGPS.sat, packetGPS.lat, packetGPS.lon, packetGPS.alt, packetGPS.end);
	//nRF_send_other(packetGPS.marker);
	//nRF_send_other(packetGPS.n);
	//nRF_send_other(packetGPS.et);
	//nRF_send_other(packetGPS.sat);
	//nRF_send_other(packetGPS.lat);
	//nRF_send_other(packetGPS.lon);
	//nRF_send_other(packetGPS.alt);
	//nRF_send_other(packetGPS.end);*/
	kernel_addTask(sendGPS, 250, PRIORITY_LOW, KSTATE_ACTIVE);
	hal_writePin(&PORTC, PC6, 0);
	return 0;
}

int sendMain(){
	hal_writePin(&PORTC, PC6, 1);
	sprintf(packetMain.marker, "YKTSAT5:MAIN:");
	sprintf(packetMain.n, "N=%lu;", ++mainn);
	sprintf(packetMain.et, "ET=%lu;", (uint32_t)kernel_getUptime()/1000);
	sprintf(packetMain.end, "\r\n");
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("sensord: Sent main packet\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("%s%s%s%s%s%s%s%s%s"), packetMain.marker, packetMain.n, packetMain.et, packetMain.vbat, packetMain.alt, packetMain.prs, packetMain.t1, packetMain.t2, packetMain.end);
	/*nRF_send_other(packetMain.marker);
	nRF_send_other(packetMain.n);
	nRF_send_other(packetMain.et);
	nRF_send_other(packetMain.vbat);
	nRF_send_other(packetMain.alt);
	nRF_send_other(packetMain.prs);
	nRF_send_other(packetMain.t1);
	nRF_send_other(packetMain.t2);
	nRF_send_other(packetMain.end);*/
	kernel_addTask(sendMain, 750, PRIORITY_LOW, KSTATE_ACTIVE);
	hal_writePin(&PORTC, PC6, 0);
	return 0;
}

int sendOrient(){
	hal_writePin(&PORTC, PC6, 1);
	sprintf(packetOrient.marker, "YKTSAT5:ORIENT:");
	sprintf(packetOrient.n, "N=%lu;", ++orientn);
	sprintf(packetOrient.et, "ET=%lu;", (uint32_t)kernel_getUptime()/1000);
	sprintf(packetOrient.end, "\r\n");
	debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("sensord: Sent orientation packet\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("%s%s%s%s%s%s%s%s%s\r\n"), packetOrient.marker, packetOrient.n, packetOrient.et, packetOrient.ax, packetOrient.ay, packetOrient.az, packetOrient.pitch, packetOrient.yaw, packetOrient.roll);
	/*nRF_send_other(packetOrient.marker);
	nRF_send_other(packetOrient.n);
	nRF_send_other(packetOrient.et);
	nRF_send_other(packetOrient.ax);
	nRF_send_other(packetOrient.ay);
	nRF_send_other(packetOrient.az);
	nRF_send_other(packetOrient.pitch);
	nRF_send_other(packetOrient.yaw);
	nRF_send_other(packetOrient.roll);
	nRF_send_other(packetOrient.end);*/
	kernel_addTask(sendOrient, 250, PRIORITY_LOW, KSTATE_ACTIVE);
	hal_writePin(&PORTC, PC6, 0);
	return 0;
}