/*
 * sdcard.c
 *
 * Created: 20.06.2019 17:47:50
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"

void sd_flush();

static char sd_buffer0[512];
static char sd_buffer1[256];
static uint16_t sd_index = 0;
static uint64_t sd_pointer = 0;
static uint64_t sd_ptr_e EEMEM;

void sd_init(){
	FRESULT res;
	res = pf_mount(&fs);
	if(res == FR_OK){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: logging system init complete\r\n"));
		pf_lseek(0);
	}
	else debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("[INIT]initd: logging system init failed\r\n"));
}

void sd_readPtr(){
	eeprom_read_block(&sd_pointer, &sd_ptr_e, 4);
}

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
	//else debug_sendMessage_p(PSTR("sdcardd: could not open /debug.log, SD card failure\r\n"), 3);
}