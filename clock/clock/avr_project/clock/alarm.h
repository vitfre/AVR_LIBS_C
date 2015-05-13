/*
 * alarm.h
 *
 * Created: 06.12.2013 15:01:49
 *  Author: Evgeny
 */ 


#ifndef ALARM_H_
#define ALARM_H_

#include <avr/io.h>

//status flags
#define TIME_UPDATED		0x01
#define RINGING				0x02
#define SNOOZE_ON			0x04
#define ALL_ALARM_DISABLED	0x80
#define IS_DARK				0x08
#define LAMP_ON				0x10

//alarm flags
#define ALARM_RING		0x01
#define ALARM_SNOOZE	0x02
#define ALARM_SUNRISE	0x04
#define ALARM_SUNRISE_OFFSET	2
#define ALARM_ENABLED	0x80
//#define ALARM_ACTIONS	0x18
//#define ALARM_ACTIONS_OFFSET	3

#define ALARM_NUM 5

typedef struct
{
	uint8_t hh;
	uint8_t mm;
	uint8_t en_mask;
	uint8_t actions;
} alarm_param;

int8_t time_diff(uint8_t HH, uint8_t  MM, uint8_t weekday, uint8_t Number);
void CheckAlarms();
extern void init_alarms();



#endif /* ALARM_H_ */