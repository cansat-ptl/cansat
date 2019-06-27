/* EN
* Author: bear1ake
* Thanks to: Dave Parson (author nRF_Lite.h), ThePetrovich
* Date: June 2019
* Note: define all pins and ports
** RU
* Автор: bear1ake
* Благодарности: Dave Parson (автор nRF_Lite.h), ThePetrovich
* Дата: June 2019
* Заметка: означить все пины и порты
*/

// Настройки (Config)
#ifndef NRF_H    
#define NRF_H

#define nRF_CSN_port PORTB
#define nRF_CSN_ddr DDRB
#define nRF_CSN_pin PB6

#define nRF_CE_port PORTE
#define nRF_CE_ddr DDRE
#define nRF_CE_pin PE6

#ifndef F_CPU
#define F_CPU 8000000L
#endif

// Ниже рискованно (Below risky)
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

void nRF_write_multi(unsigned char a, unsigned char length);
void nRF_write(unsigned char a, unsigned char b);
unsigned char nRF_send(int len);
unsigned char nRF_send_other(char * data);
void nRF_init(int freq);
void nRF_stdef();
unsigned char nRF_readReg(unsigned char a);
unsigned char nRF_readReg_a(unsigned char a, int length);
uint8_t nRF_TxComplete();

#define nRF_write_reg(a, b) nRF_write((0x20 | (0x1F & a)), b)
#define nRF_write_multi_reg(a, b) nRF_write_multi((0x20 | (0x1F & a)), b)

struct nRF24L01
{
	int trRetWaitUs;
	int maxHasDataIntUs;
	char buf[32];
}nRF;

#endif