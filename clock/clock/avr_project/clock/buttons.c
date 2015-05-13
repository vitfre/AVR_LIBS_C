/*
 * Buttons.c
 *
 * Created: 02.12.2013 11:46:23
 *  Author: Evgeny
 */ 

#include "buttons.h"
#include "indication.h"
#include "ir.h"
#include "alarm.h"
#include "ds3231.h"
#include "eeprom.h"


static uint8_t alarm_index=0;

volatile mode_t Mode;

extern uint8_t Indicator[4];
extern volatile int8_t blink_mask;
extern volatile int8_t dot_mask;
extern dstime_t current_time;
extern alarm_param Alarms[];
extern volatile uint8_t status;


void ShowAlarmWeekday()
{
	Indicator[0] = IND_D;
	Indicator[2] = IND_OFF;
	if(Alarms[alarm_index].en_mask & (0x01<<(Indicator[1]-1)))
		Indicator[3] = 1;
	else
		Indicator[3] = 0;
}

void IncHH()
{
	if(Indicator[0]==2 && Indicator[1]==3)
	{
		Indicator[0]=IND_OFF;
		Indicator[1]=0;
	}
	else if(Indicator[1]==9)
	{
		if (Indicator[0]!=IND_OFF)
		{
			Indicator[0]++;
		}
		else
		{
			Indicator[0]=1;
		}
		Indicator[1]=0;
	}
	else
		Indicator[1]++;
}

void IncMM()
{
	if(Indicator[2]==5 && Indicator[3]==9)
	{
		Indicator[2]=0;
		Indicator[3]=0;
		//IncHH();
	}
	else if(Indicator[3]==9)
	{
		Indicator[2]++;
		Indicator[3]=0;
	}
	else
		Indicator[3]++;
}

void Button1_LP()
{
	Mode = MODE_TIME;
	blink_mask=0x00;
	ShowTime();
}
void Button1_pressed()
{
	if (status & RINGING)
	{
		status &=~RINGING;
		return;
	}
	
	switch (Mode)
	{
		case MODE_SET_TIME:
			Mode=MODE_SET_DAY;
			dot_mask = 0b0010;
			blink_mask = BLINK_MM;
			Indicator[0]=IND_OFF;
			Indicator[1]=IND_D;
			Indicator[2]=current_time.day;
			Indicator[3]=IND_OFF;
			break;
		case MODE_SET_DAY:
			//запись скорректированного дня недели
			SetDay(Indicator[2]);
			Mode = MODE_TIME;
			blink_mask = 0x00;
			ShowTime();
			break;
		case MODE_TIME:
			Mode=MODE_SEL_ALARM;
			dot_mask =0;
			alarm_index = 0;
			Indicator[3]=1;
			blink_mask = BLINK_ALL;
			Indicator[0] = IND_A;
			Indicator[1] = IND_L;
			Indicator[2] = IND_OFF;
			break;
		case MODE_SEL_ALARM:
			if(Indicator[3]<ALARM_NUM)
			{
				alarm_index++;
				Indicator[3]++;
			}
			else
			{
				alarm_index=0;
				Mode = MODE_SET_TIME;
				blink_mask = BLINK_ALL;
				ShowTime();
			}
			break;
		case MODE_SET_ALARM_ENABLED:
			Mode=MODE_SET_ALARM_HH;
			blink_mask = BLINK_HH;
			dot_mask = 0b0010;
			Show(Alarms[alarm_index].hh, Alarms[alarm_index].mm);
			break;
		case MODE_SET_ALARM_HH:
			Mode=MODE_SET_ALARM_MM;
			blink_mask = BLINK_MM;
			dot_mask = 0b0010;
			break;
		case MODE_SET_ALARM_MM:
			Mode=MODE_SET_ALARM_WEEKDAY;
			blink_mask=0x08;
			dot_mask = 0b010;
			Indicator[1]=1;
			ShowAlarmWeekday();
		break;
		case MODE_SET_HH:
			Mode=MODE_SET_MM;
			blink_mask = BLINK_MM;
			dot_mask = 0b0010;
		break;
		case MODE_SET_MM:
		{
			//здесь запись скорректированного времени
			uint8_t hh = Indicator[1];
			if (Indicator[0]<=2)
			{
				hh	|= (Indicator[0]<<4);
			}
			SetTime(hh, (Indicator[2]<<4) | Indicator[3]);
			Mode = MODE_TIME;
			blink_mask = 0x00;
			ShowTime();
		}
		break;
		case MODE_SET_ALARM_WEEKDAY:
			if(Indicator[1]<7)
			{
				Indicator[1]++;
				ShowAlarmWeekday();
			}
			else
			{
				Mode = MODE_SET_ALARM_SOUND;
				blink_mask=0x08;
				dot_mask = 0b0100;
				Indicator[0] = 0x05;
				Indicator[1] = IND_N;
				Indicator[2] = IND_D;
				Indicator[3] = (Alarms[alarm_index].actions & (ALARM_RING | ALARM_SNOOZE));
			}
		break;
		case MODE_SET_ALARM_SOUND:
			Mode=MODE_SET_ALARM_SUNRISE;
			blink_mask=0x08;
			dot_mask = 0b0100;
			Indicator[0] = 0x05;
			Indicator[1] = IND_U;
			Indicator[2] = IND_N;
			Indicator[3] = ((Alarms[alarm_index].actions & ALARM_SUNRISE) >> ALARM_SUNRISE_OFFSET);
			break;
		case MODE_SET_ALARM_SUNRISE:
			Mode=MODE_TIME;
			blink_mask=0x00;
			ShowTime();
		break;
	}
	
}

void Button2_pressed()
{
	if (status & RINGING)
	{
		status &=~RINGING;
		return;
	}
	
	switch(Mode)
	{
	case MODE_TIME:
		ShowTemp(1);
	break;
	case MODE_SEL_ALARM:
		Mode=MODE_SET_ALARM_ENABLED;
		blink_mask = BLINK_ALL;
		dot_mask=0;
		Indicator[0] = IND_OFF;
		Indicator[1] = 0;
		if(Alarms[alarm_index].en_mask & ALARM_ENABLED)
		{
			Indicator[2] = IND_N;
			Indicator[3] = IND_OFF;
		}
		else
		{
			Indicator[2] = IND_F;
			Indicator[3] = IND_F;
		}
		break;
	case MODE_SET_TIME:
		Mode=MODE_SET_HH;
		blink_mask=BLINK_HH;
		break;
	case MODE_SET_ALARM_HH:
		IncHH();
		if (Indicator[0]==IND_OFF)
		{
			Alarms[alarm_index].hh= Indicator[1];
		}
		else
		{
			Alarms[alarm_index].hh=((Indicator[0] << 4) | Indicator[1]);
		}
		EEPROM_write((uint8_t*)&Alarms[alarm_index].hh-(uint8_t*)Alarms, Alarms[alarm_index].hh);
		break;
	case MODE_SET_ALARM_MM:
		IncMM();
		Alarms[alarm_index].mm=((Indicator[2] << 4) | Indicator[3] );
		EEPROM_write((uint8_t*)&Alarms[alarm_index].mm-(uint8_t*)Alarms, Alarms[alarm_index].mm);
		break;
	case MODE_SET_HH:
		IncHH();
		break;
	case MODE_SET_MM:
		IncMM();
		break;
	case MODE_SET_DAY:
		if (Indicator[2]<7)
		{
			Indicator[2]++;
		}
		else
		{
			Indicator[2]=1;
		}
		break;
	case MODE_SET_ALARM_ENABLED:
		if(Indicator[2]==IND_N)
		{
			Indicator[2]=IND_F;
			Indicator[3]=IND_F;
			Alarms[alarm_index].en_mask &= ~ALARM_ENABLED;
		}
		else
		{
			Indicator[2]=IND_N;
			Indicator[3]=IND_OFF;
			Alarms[alarm_index].en_mask |= ALARM_ENABLED;
		}
		EEPROM_write((uint8_t*)&Alarms[alarm_index].en_mask-(uint8_t*)Alarms, Alarms[alarm_index].en_mask);
		CheckAlarms();
		break;
	case MODE_SET_ALARM_WEEKDAY:
		dot_mask = 0;
		if(Indicator[3]==1)
		{
			Indicator[3]=0;
			Alarms[alarm_index].en_mask &= ~(0x01<<(Indicator[1]-1));
		}
		else
		{
			Indicator[3]=1;
			Alarms[alarm_index].en_mask |= (0x01<<(Indicator[1]-1));
		}
		EEPROM_write((uint8_t*)&Alarms[alarm_index].en_mask-(uint8_t*)Alarms, Alarms[alarm_index].en_mask);
		dot_mask = 0b0010;
		CheckAlarms();
		break;
	case MODE_SET_ALARM_SOUND:
		if(Indicator[3]==2)
			Indicator[3]=0;
		else
			Indicator[3]++;
		Alarms[alarm_index].actions &= ~(ALARM_RING | ALARM_SNOOZE);
		Alarms[alarm_index].actions |= Indicator[3];
		EEPROM_write((uint8_t*)&Alarms[alarm_index].actions-(uint8_t*)Alarms, Alarms[alarm_index].actions);
		break;
	case MODE_SET_ALARM_SUNRISE:
		if(Indicator[3]==1)
		{
			Indicator[3]=0;
			Alarms[alarm_index].actions &= ~ALARM_SUNRISE;
		}
		else
		{
			Indicator[3]++;
			Alarms[alarm_index].actions |= ALARM_SUNRISE;
		}
		EEPROM_write((uint8_t*)&Alarms[alarm_index].actions-(uint8_t*)Alarms, Alarms[alarm_index].actions);
		break;
	}
    
}
void Button3_pressed()
{
	status &= ~RINGING;
}
void Button3_LP()
{
	beep(0b11);
	
	if (status & SNOOZE_ON)
	{
		status &=~ SNOOZE_ON;
		led_off(LED2);
		led_on(LED1);
	}
	else
	{
		status ^= ALL_ALARM_DISABLED;
		if (status & ALL_ALARM_DISABLED)
		{
			led_off(LED1 | LED2);
			return;
		}
		CheckAlarms();
	}
}

void Button2_LP()
{
	if(Mode != MODE_SHOW_TEMP)
		return;

	beep(0b11);
	//static uint8_t light=0;
	//if (!(ACSR & (1<<ACO)))	//если темно
	if(status & LAMP_ON)
	{
		SendCommand(CMD_OFF);
	}
	else
	{
		SendCommand(CMD_ON);
	}
	status ^= LAMP_ON;
	//light^=1;
}