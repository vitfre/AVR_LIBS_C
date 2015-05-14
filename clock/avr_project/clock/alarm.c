/*
 * alarm.c
 *
 * Created: 06.12.2013 13:41:48
 *  Author: Evgeny
 */ 
#include "eeprom.h"
#include "alarm.h"
#include "ds3231.h"
#include <string.h>

#include "indication.h"
#include "ir.h"

alarm_param Alarms[ALARM_NUM];
extern dstime_t current_time;
extern volatile uint8_t status;

extern void init_alarms()
{
	for(int i=0;i<sizeof(alarm_param)*ALARM_NUM;i++)
	{
		*(((uint8_t*)Alarms)+i) = EEPROM_read(i);
	}
	if (Alarms[0].hh == 0xFF)
	{
		memset(Alarms,0,sizeof(alarm_param)*ALARM_NUM);
		
		for(int i=0;i<sizeof(alarm_param)*ALARM_NUM;i++)
		{
			EEPROM_write(i,*((unsigned char*)Alarms+i));
		}
	}
}

static int16_t bcd2bin(uint8_t val)
{
	return (val >> 4)*10 + (val & 0x0F);
}

int16_t time_after_alarm(uint8_t num)	//возвращает количество минут после срабатывания будильника в диапазоне 0...60
{
	int16_t diff;
	
	if (!(Alarms[num].en_mask & ALARM_ENABLED))
	{
		return -1;
	}
	if (!(Alarms[num].actions & ALARM_SNOOZE))
	{
		return -1;
	}
	if (!(Alarms[num].en_mask & (1<<(current_time.day-1))))
	{
		return -1;
	}

	diff =	(bcd2bin(current_time.hour)*60 + bcd2bin(current_time.min))-
			(bcd2bin(Alarms[num].hh)*60 + bcd2bin(Alarms[num].mm));
			
	if (diff<60)
	{
		return diff;
	}
	return -1;
}

int16_t time_before_alarm(uint8_t num)	//возвращает количество минут до срабатывания будильника, в диапазоне до суток
{
	int16_t diff;
	
	if (!(Alarms[num].en_mask & ALARM_ENABLED))		//будильник выключен
	{
		return -1;
	}
	
	diff =	(bcd2bin(Alarms[num].hh)*60 +	bcd2bin(Alarms[num].mm))-
			(bcd2bin(current_time.hour)*60 +bcd2bin(current_time.min));
			
	if (diff<0)
	{
		uint8_t nextday=current_time.day;
		if (current_time.day==7)
		{
			nextday=0;	//проверить понедельник
		}
		if (Alarms[num].en_mask & (1<<nextday))	//текущее время больше будильника, проверить следущий день
		{
			diff+=(24*60);
			return diff;
		}
		else
			return -1;
	}
	else if (Alarms[num].en_mask & (1<<(current_time.day-1)))	//текущий день
	{
		return diff;
	}
	return -1;
}

void CheckAlarms()
{
	int16_t diff=-1;
	int tomorrow_alarm=0;
	static uint8_t is_dark=0;
	
	if (status & ALL_ALARM_DISABLED)
	{
		return;
	}
	
	status &= ~RINGING;
	
	for(int i=0;i<ALARM_NUM;i++)
	{
		diff = time_before_alarm(i);
		switch(diff)
		{
			case 0:
				if (Alarms[i].actions & ALARM_RING )
				{
					status |= RINGING;
				}
				if (Alarms[i].actions & ALARM_SNOOZE)
				{
					status |= RINGING;
					status |= SNOOZE_ON;
				}
				if (Alarms[i].actions & ALARM_SUNRISE)
				{
					//свет 100%
					if(is_dark)
						SendCommand(CMD_B5);
				}
			break;
			case 7:
				if (Alarms[i].actions & ALARM_SUNRISE)
				{
					//свет 80%
					if(is_dark)
						SendCommand(CMD_B4);
				}
			case 15:
				if (Alarms[i].actions & ALARM_SUNRISE)
				{
					//свет 60%
					if(is_dark)
						SendCommand(CMD_B3);
				}
			break;
			case 22:
				if (Alarms[i].actions & ALARM_SUNRISE)
				{
					//свет 40%
					if(is_dark)
						SendCommand(CMD_B2);
				}
			break;
			case 30:
				if (Alarms[i].actions & ALARM_SUNRISE)
				{
					//свет 20%
					//if(!(ACSR & (1<<ACO)))	//если темно
					//if((status & IS_DARK))	//если темно
					if(1)
					{
						is_dark=1;
						status |= LAMP_ON;
						SendCommand(CMD_B1);
					}
					else
					{
						is_dark = 0;
					}
				}
			break;
		}
		if (diff>0)
		{
			tomorrow_alarm=1;
		}
		
	}
	if (status & RINGING)
	{
		led_off(LED1 | LED2);
	}
	else if (status & SNOOZE_ON)
	{
		for(int i=0;i<ALARM_NUM;i++)
		{
			diff = time_after_alarm(i);
			switch(diff)
			{
				case 4:
				case 8:
				case 12:
				status |= RINGING;
				break;
				case 15:
				status |= RINGING;
				status &=~SNOOZE_ON;
				break;
			}
			
		}
	}
	else if (tomorrow_alarm)
	{
		led_off(LED2);
		led_on(LED1);
	} 
	else
	{
		led_off(LED1);
		led_on(LED2);
	}
}