/*
 * sdcard.c
 *
 * Created: 20.06.2019 17:47:50
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"
#include "../globals.h"

void sd_flush();

char sd_buffer0[512];
char sd_buffer1[256];
uint16_t sd_index = 0;
uint64_t sd_pointer = 0;

void sd_puts(char * data){
	int i;
	for(i = 0; i < strlen(data); i++){
		if(sd_index < 512){
			sd_buffer0[sd_index] = data[i];
			sd_index++;	
		}
		else {
			if(sd_index >= 512 && sd_index < 768-1){
				sd_buffer1[sd_index - 512] = data[i];
				sd_buffer1[(sd_index - 512)+1] = 0;
				sd_index++;
			}
		}
	}
	if(sd_index >= 512)
		sd_flush();
}

void sd_flush(){
	WORD bw;
	FRESULT res;
	uint8_t cs_flags = 0;
	if(checkBit_m(ADXL345_PORT,ADXL345_CS)){
		writePin(&ADXL345_PORT, ADXL345_CS, 0);
		setBit_m(cs_flags, 0);
	}
	if(checkBit_m(BMP280_PORT,BMP280_CS)){
		writePin(&BMP280_PORT, BMP280_CS, 0);
		setBit_m(cs_flags, 1);
	}
	if(checkBit_m(NRF_CSN_PORT,NRF_CSN)){
		writePin(&NRF_CSN_PORT, NRF_CSN, 0);
		setBit_m(cs_flags, 2);
	}	
	
	res = pf_open("/debug.log");
	if(res == FR_OK){
		cli();
		pf_lseek(sd_pointer);
		pf_write(sd_buffer0, 512, &bw);
		pf_write(0, 0, &bw);
		sei();
		sd_index = strlen(sd_buffer1);
		sprintf(sd_buffer0, "%s", sd_buffer1);
		sd_buffer1[0] = 0;
		sd_pointer += 512;
	}
	else debugMessage_p(PSTR("Could not open /debug.log, SD card failure\r\n"), 3);
	if(checkBit_m(cs_flags, 0)){
		writePin(&ADXL345_PORT, ADXL345_CS, 1);
	}
	if(checkBit_m(cs_flags, 1)){
		writePin(&BMP280_PORT, BMP280_CS, 1);
	}
	if(checkBit_m(cs_flags, 2)){
		writePin(&NRF_CSN_PORT, NRF_CSN, 1);
	}
}