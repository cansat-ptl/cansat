/*
 * kernel_config.h
 *
 * Created: 30.07.2019 22:31:10
 *  Author: Admin
 */ 


#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

#ifndef KERNELconfig
#define KERNELconfig

	#define MAX_TIMER_COUNT 5
	#define MAX_TASK_QUEUE_SIZE 32
	#define MAX_HIGHPRIO_CALL_QUEUE_SIZE 14
	#define MAX_NORMPRIO_CALL_QUEUE_SIZE 24
	#define MAX_LOWPRIO_CALL_QUEUE_SIZE 16

	#define KERNEL_TIMER_PRESCALER 3
	#define TICKRATE_MS 1

	/*             Prescaler values             */
	/* 0 - timer disabled                       */
	/* 1 - no prescaling (F_TIMER = F_CPU)      */
	/* 2 - F_CPU / 8                            */
	/* 3 - F_CPU / 64                           */
	/* 4 - F_CPU / 256                          */
	/* 5 - F_CPU / 1024                         */
	/* 6 & 7 - external clock (datasheet)       */
	
	#define FORCE_LOWERPRIO_THRESHOLD 10

	#define KERNEL_SD_MODULE 1
	#define KERNEL_WDT_MODULE 1
	#define KERNEL_UTIL_MODULE 1
	#define KERNEL_DEBUG_MODULE 1

	#define VERBOSE 0
	#define PROFILING 0
	
#endif
#endif /* KERNEL_CONFIG_H_ */