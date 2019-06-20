/*
 * sdcard.c
 *
 * Created: 20.06.2019 17:47:50
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"
#include "../globals.h"

void flushToSD();

char sd_buffer0[512];
char sd_buffer1[256];
uint16_t sd_index = 0;
uint64_t sd_pointer = 0;

void addMessage(char * data){
//	char tempBuffer[64];
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
//		if(sd_index == 511 && strlen(data)) tempBuffer[i] = data[i];
	}
	if(sd_index >= 512)
		flushToSD();
	//if(sd_index >= 448) flushToSD();
}

void flushToSD(){
	WORD bw;
	FRESULT res;
	res = pf_open("/debug.log");
	if(res == FR_OK){
		cli();
		pf_lseek(sd_pointer);
		pf_write(sd_buffer0, 512, &bw);
		sei();
		sd_index = strlen(sd_buffer1);
		sprintf(sd_buffer0, "%s", sd_buffer1);
		sd_buffer1[0] = 0;
		sd_pointer += 512;
	}
	else debugMessage_p(PSTR("Could not open /debug.log, SD card failure\r\n"), 3);
		/*btw = strlen(levels[level]);
		cli();
		pf_write(levels[level], btw, &s1);
		sei();
		sprintf_P(buffer, PSTR("%S"), msg);
		btw = strlen(buffer);
		cli();
		pf_write(buffer, btw, &s1);
		sei();
		pf_write(0, 0, &s1);
		sd_ptr++;*/
}