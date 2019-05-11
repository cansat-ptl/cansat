/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 
struct taskstruct{
	task taskPtr;
	uint8_t timeout;
} taskqueue[MAX_QUEUE_SIZE];