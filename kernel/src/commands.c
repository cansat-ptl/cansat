/*
 * CFile1.c
 *
 * Created: 18.08.2019 22:44:40
 *  Author: WorldSkills-2019
 */ 
#include "../kernel.h"

#ifndef CMD_COMMAND_AMOUNT
#define CMD_COMMAND_AMOUNT 8
#endif

volatile static char recvBuffer[RX0_BUFFER_SIZE];
volatile static uint8_t recvBuffer_i = 0;
static struct commandStruct commands[CMD_COMMAND_AMOUNT];
static uint8_t registeredCmds = 0;

static void kernel_clearRecvBuffer(){
	for(int i = 0; i < RX0_BUFFER_SIZE; i++) recvBuffer[i] = 0;
	recvBuffer_i = 0;
	rx0_enableInterrupt();
}
static int kernel_processCommand()
{
	//uint8_t c_argc = 0;
	//debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Received string: %s\r\n"), recvBuffer);
	char * token = strtok((char *)recvBuffer, " ");
	if(token == NULL){
		kernel_clearRecvBuffer();
		debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Enter a command\r\n"));
		debug_logMessage(PGM_ON, L_NONE, PSTR("\r\nroot@cansat:< "));
		return 0;
	}
	debug_logMessage(PGM_ON, L_NONE, PSTR("\r\n"));
	//debug_logMessage(PGM_ON, L_NONE, PSTR("Here 2\r\n"));
	for(int i = 0; i < registeredCmds; i++){
		//debug_logMessage(PGM_ON, L_NONE, PSTR("Comparing %s and %s\r\n"), token, commands[i].keyword);
		if(strcmp(commands[i].keyword, token) == 0){
			//while(token != NULL){
			//	strtok(NULL, " ");
			//	c_argc++;
			//}
			//debug_logMessage(PGM_ON, L_INFO, PSTR("cli: Parsing result: %s\r\n"), token);
			(commands[i].handler)();
			kernel_clearRecvBuffer();
			debug_logMessage(PGM_ON, L_NONE, PSTR("\r\nroot@cansat:< "));
			return 0;
		}
		else {
			//debug_logMessage(PGM_ON, L_INFO, PSTR("cli: Parsing result: %s\r\n"), token);
			//debug_logMessage(PGM_ON, L_ERROR, PSTR("cli: Buffer length: %d\r\n"), recvBuffer_i);
		}
	}
	
	kernel_clearRecvBuffer();
	debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Command not found\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("\r\nroot@cansat:< "));
	return ERR_COMMAND_NOT_FOUND;
}

void kernel_registerCommand(const char * c_keyword, cmdHandler c_ptr)
{
	if(registeredCmds < CMD_COMMAND_AMOUNT+1){
		debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Registered command %s, handler at 0x%X\r\n"), c_keyword, (int)c_ptr);
		commands[registeredCmds].handler = c_ptr;
		strcpy(commands[registeredCmds].keyword, c_keyword);
		commands[registeredCmds].length = strlen(c_keyword);
		registeredCmds++;
	}
}

char kernel_parseCmdArgs(char * token, char * arglist, uint8_t arglist_len){
	for(int i = 0; i < arglist_len; i++){
		if(token[1] == arglist[i]){
			return arglist[i];
			//debug_logMessage(PGM_ON, L_INFO, PSTR("cli: Parsed argument %c\r\n"), arglist[i]);
		}
		else if(i == arglist_len-1){
			return '\0';
		}
	}
	return '\0';
}

static void config()
{
	char * token = (char *)recvBuffer;
	char arglist[] = "abcdef";
	char activeArgument = '\0';
	uint8_t arglist_len = strlen(arglist);

	while(token != NULL){
		token = strtok(NULL, " ");
		
		if(token[0] == '-'){
			activeArgument = kernel_parseCmdArgs(token, arglist, arglist_len);
			uint8_t a = 0;
			switch(activeArgument){
				case '\0':
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Unknown argument: %s\r\n"), token);
					return;
				break;
				case 'a':
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: '-a' specified, changing variable A\r\n"));
					token = strtok(NULL, " ");
					if(sscanf(token, "%d", (int*)&a) == 1){
						debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Received argument: %s\r\n"), token);
						debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Parsed argument: %d\r\n"), a);
						debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Variable A has been set to %d\r\n"), a);
						debug_logMessage(PGM_ON, L_NONE, PSTR("cli: success\r\n"));
					}
					else debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Error: no argument value specified\r\n"));
					
				break;
				case 'b':
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: '-b' specified, executing B subroutine\r\n"));
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: success\r\n"));
				break;
				case 'c':
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: '-c' specified, executing C subroutine\r\n"));
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: success\r\n"));
				break;
				default:
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Mishandled argument\r\n"));
				break;
			}
		}
	}
}

static void debug()
{
	char * token = (char *)recvBuffer;
	char arglist[] = "dv";
	char activeArgument = '\0';
	uint8_t arglist_len = strlen(arglist);
	
	while(token != NULL){
		token = strtok(NULL, " ");
		if(token[0] == '-'){
			activeArgument = kernel_parseCmdArgs(token, arglist, arglist_len);
			char ds[5];
			switch(activeArgument){
				case '\0':
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Unknown argument: %s\r\n"), token);
					return;
				break;
				case 'd':
					token = strtok(NULL, " ");
					if(sscanf(token, "%s", ds) != 0){
						if(strcmp(ds, "on") == 0){
							debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Enabling debug output\r\n"));
							kernel_setFlag(KFLAG_DEBUG, 1);
						} 
						else if(strcmp(ds, "off") == 0){
							debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Disabling debug output\r\n"));
							kernel_setFlag(KFLAG_DEBUG, 0);
						}
						else {
							debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Unknown argument value, should be either 'on' or 'off'\r\n"));
						}
			
					}
					else debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Error: no argument value specified\r\n"));
				break;
				case 'v':
					if(strcmp(ds, "on") == 0){
						debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Enabling verbose output\r\n"));
						//kernel_setFlag(KFLAG_DEBUG, 1);
					}
					if(strcmp(ds, "off") == 0){
						debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Disabling debug output\r\n"));
						//kernel_setFlag(KFLAG_DEBUG, 0);
					}
				break;
				default:
					debug_logMessage(PGM_ON, L_NONE, PSTR("cli: Mishandled argument\r\n"));
				break;
			}
		}
		//c_argc++;
	}
}

static void reboot()
{
	debug_logMessage(PGM_ON, L_NONE, PSTR("Reboot command\r\n"));
}

static void datetime()
{
	//debug_logMessage(PGM_ON, L_NONE, PSTR("Current time: %02d.%02d.20%02d %02d:%02d:%02d UTC\r\n"), GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
}

static void clear()
{
	debug_logMessage(PGM_ON, L_NONE, PSTR("\x0C"));
}

static void help()
{
	debug_logMessage(PGM_ON, L_NONE, PSTR("Available commands:\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  config [-a <n>] [-b] [-c] - configuration tool. Run 'config -h' for more details.\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  clear - clears screen.\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  reboot - reboots the device.\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  datetime - displays GPS date and time (UTC).\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  sysinfo - displays system information.\r\n"));
}

static void sysinfo()
{
	debug_logMessage(PGM_ON, L_NONE, PSTR("Device information:\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  MCU: AVR ATmega128, device signature: 0x1E9702\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  Frequency: %d MHz\r\n"), F_CPU/1000000L);
	debug_logMessage(PGM_ON, L_NONE, PSTR("  SRAM: 4K x 8 (4 KB)\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  EEPROM: 1K x 32 (4 KB)\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  Flash: 64K x 16 (128 KB)\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  Peripherals: GPIO, 1 x SPI, 2 x UART, 1 x TWI, 8c ADC, JTAG, EMI\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("Software:\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("  Kernel: CanSatKernel version %s built %s\r\n"), KERNEL_VER, KERNEL_TIMESTAMP);
	debug_logMessage(PGM_ON, L_NONE, PSTR("  HAL: CanSatHAL version %s built %s\r\n"), KERNEL_VER, KERNEL_TIMESTAMP);
	debug_logMessage(PGM_ON, L_NONE, PSTR("  FatFS: Petit FatFS version R0.02\r\n"), KERNEL_VER, KERNEL_TIMESTAMP);
}


void kernel_initCLI()
{
	delay(1000);
	debug_logMessage(PGM_ON, L_NONE, PSTR("\x0C"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("Initializing shell...\r\n\r\n"));
	kernel_registerCommand("config", config);	
	kernel_registerCommand("debug", debug);
	kernel_registerCommand("reboot", reboot);
	kernel_registerCommand("datetime", datetime);
	kernel_registerCommand("clear", clear);
	kernel_registerCommand("sysinfo", sysinfo);
	kernel_registerCommand("help", help);
	delay(500);
	debug_logMessage(PGM_ON, L_NONE, PSTR("\x0C"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("+-----------------------------------------------------------------+\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("|                       CanSat shell v0.0.1                       |\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("|  Run 'help' to see the list of available commands.              |\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("|  Run 'config' to change the device's settings.                  |\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("+-----------------------------------------------------------------+\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("\r\nroot@cansat:< "));
	rx0_enableInterrupt();
}

ISR(USART0_RX_vect)
{
	char data = UDR0;
	if(strlen((char*)&recvBuffer) < RX0_BUFFER_SIZE){
		if(data == 0x7f || data == 0x08) {
			if(recvBuffer_i >= 1) recvBuffer_i--; //backspace character handling
		}
		else {
			recvBuffer[recvBuffer_i] = data;
			recvBuffer_i++;
		}
	}
	if(data == '\r' || data == '\n' || data == ';'){
		recvBuffer[recvBuffer_i-1] = '\0';
		rx0_disableInterrupt();
		//kernel_processCommand();
		kernel_addCall(kernel_processCommand, PRIORITY_LOW);
	}
	//debug_logMessage(PGM_ON, L_INFO, PSTR("RX0_ISR\r\n"));
}
