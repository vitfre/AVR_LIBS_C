#ifndef DS3231_H
#define DS3231_H

#include <avr/io.h>

typedef struct _dstime
{
	uint8_t hour;
	uint8_t min;
	uint8_t day;

} dstime_t;

extern void init_rtc();
extern uint8_t GetTime(dstime_t* current_time);
extern void SetTime(uint8_t hh, uint8_t mm);
extern void SetDay(uint8_t day);

extern uint16_t GetTemp();

#endif //DS3231_H