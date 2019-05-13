/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "kernel.h"
#include "globals.h"

uint8_t callIndex = 0; //Index of the last task in queue
volatile uint8_t taskIndex = 0; //Index of the last task in queue
uint8_t error = 0;		//Latest task return code
volatile uint8_t flags = 0;		//Common variable for kernel control flags
volatile task callQueue[MAX_QUEUE_SIZE];
volatile struct taskStruct taskQueue[MAX_QUEUE_SIZE];

void idle();
void readData();
void waitForTx();

////////////////////////////////////////////////////////////////////

inline uint8_t addTask(task t_ptr){
	if(SREG & (1 << 7)){
		cli();
	}
	if(taskIndex < MAX_QUEUE_SIZE){
		callIndex++;
		callQueue[callIndex] = t_ptr;
		sei();
		return 0;
	}
	else {
		sei();
		return ERR_QUEUE_OVERFLOW;
	}
}

////////////////////////////////////////////////////////////////////

uint8_t addTimedTask(task t_ptr, uint8_t t_period){
	if(SREG & (1 << 7)){
		cli();
	}
	if(taskIndex < MAX_QUEUE_SIZE){
		taskIndex++;
		taskQueue[taskIndex].pointer = t_ptr;
		taskQueue[taskIndex].period = t_period;
		sei();
		return 0;
	}
	else {
		sei();
		return ERR_QUEUE_OVERFLOW;
	}
	sei();
}

////////////////////////////////////////////////////////////////////

inline uint8_t removeTask(){
	if(SREG & (1 << 7))
		cli();
	if(callIndex != 0){
		callIndex--;
		for(int i = 0; i < MAX_QUEUE_SIZE-1; i++){
			callQueue[i] = callQueue[i+1];
		}
		callQueue[MAX_QUEUE_SIZE-1] = idle;
	}	
	else {
		callQueue[0] = idle;
	}
	sei();
	return 0;
}

////////////////////////////////////////////////////////////////////

inline uint8_t removeTimedTask(uint8_t position){
	if(SREG & (1 << 7))
		cli();
	taskIndex--;
	taskQueue[position].pointer = idle;
	taskQueue[position].period = 0;
	for(int j = position; j < MAX_QUEUE_SIZE-1; j++){
		taskQueue[j].pointer = taskQueue[j+1].pointer;
		taskQueue[j].period = taskQueue[j+1].period;
	}
	taskQueue[MAX_QUEUE_SIZE-1].pointer = idle;
	taskQueue[MAX_QUEUE_SIZE-1].period = 0;	
	sei();
	return 0;
}

////////////////////////////////////////////////////////////////////

void clearCallQueue(){
	if(SREG & (1 << 7))
		cli();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		callQueue[i] = 0;
	}
	callIndex = 0;
	sei();
}

////////////////////////////////////////////////////////////////////

void clearTaskQueue(){
	if(SREG & (1 << 7))
	cli();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
	}
	taskIndex = 0;
	sei();
}

////////////////////////////////////////////////////////////////////

inline void timerService(){
	if(SREG & (1 << 7))
		cli();
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		if(taskQueue[i].pointer == idle) continue;
		else {
			if(taskQueue[i].period != 0)
				taskQueue[i].period--;
			else {
				addTask(taskQueue[i].pointer);
				removeTimedTask(i);
			}
		}
	}
	sei();
}

////////////////////////////////////////////////////////////////////

void startTimer(){
	/*cli();
	TCCR1A |= 0;
	TCNT1 = 0;						 //Resetting timer
	TCCR1B |= (1<<CS12)|(1<<WGM12);  //Timer1 CTC mode, prescaler 256
	OCR1A = 0x271;					 //625 as compare value
	TIMSK |= (1<<OCIE1A);			 //Fire interrupt when compare match, approx. every 20 ms
	sei();*/
	cli();
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
	TCNT1 = 0; // initialize counter
	OCR1A = 5000;  // initialize compare value
	TIMSK |= (1 << OCIE1A); // enable compare interrupt
	sei(); // enable global interrupts
}

////////////////////////////////////////////////////////////////////

inline uint8_t taskManager(){
	cli();
	(callQueue[0])();
	uint8_t code = removeTask();
	sei();
	flags = 0;	//temp
	return code;
}

////////////////////////////////////////////////////////////////////

uint8_t kernel(){
	addTimedTask(readData, 100);
	
	sprintf((char*)&tx0_buffer, "[INIT]Kernel: starting task manager...DONE!\r\n");
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	
	while(1){
		taskManager();
	}
}

////////////////////////////////////////////////////////////////////

uint8_t kernelInit(){
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){
		taskQueue[i].pointer = idle;
		taskQueue[i].period = 0;
		callQueue[i] = idle;
	}
	
	sprintf((char*)&tx0_buffer, "[INIT]Kernel: starting kernel...");
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	
	startTimer();
	
	sprintf((char*)&tx0_buffer, "DONE!\r\n");
	uart0_transmit();
	while(creg0 & (1<<TX0BUSY)){
		;
	}
	
	kernel();
	return 0;
}

////////////////////////////////////////////////////////////////////

ISR(TIMER1_COMPA_vect){
	timerService();
}