/*
 * hal.c
 *
 * Created: 21.06.2019 23:53:24
 *  Author: ThePetrovich
 */ 

#include "../hal.h"

void setupPins(){
	JUMPER_DDR |= (1<<JUMPER_OUT);
	JUMPER_PORT |= (1<<JUMPER_OUT);
	LED_KRN_DDR |= (1<<LED_KRN);
	LED_WRK_DDR |= (1<<LED_WRK);
	LED_DBG_DDR |= (1<<LED_DBG);
	LED_TX_DDR |= (1<<LED_TX);
	LED_BUILTIN_DDR |= (1<<LED_BUILTIN);
}