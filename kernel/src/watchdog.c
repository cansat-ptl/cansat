/*
 * watchdog.c
 *
 * Created: 17.06.2019 23:04:09
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"
#include "../drivers.h"

uint8_t mcucsr_mirror __attribute__ ((section (".noinit")));

void wdt_saveMCUCSR(void)
{
	mcucsr_mirror = MCUCSR;
	MCUCSR = 0;
	wdt_disable();
}

void wdt_enableWatchdog()
{
//	WDTCR = (1<<WDCE)|(1<<WDE);
//	WDTCR = 0;
//	WDTCR = (1<<WDIE) | (1<<WDP2) | (1<<WDP0); AWW THERES NO WDT INTERRUPT
	wdt_enable(WDTO_2S);
}

//TODO: write ISR for atmega1281
/*
ISR(WDT_vect){
	uart0_puts("Watchdog reset");
}
*/