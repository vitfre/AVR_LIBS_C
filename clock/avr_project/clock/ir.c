/*
 * ir.c
 *
 * Created: 08.10.2014 16:18:46
 *  Author: Evgeny
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ir.h"


#define PULSE_LEN	42
#define LEN_1	PULSE_LEN*2
#define START_LEN	670
#define START_PAUSE 340

#define IR_LED (1<<PORTC3);

extern volatile uint16_t current_time_ms_touch;

static uint16_t time_ms;

static void blink(uint16_t time)
{
	time_ms = current_time_ms_touch;
	volatile uint8_t i;
	
	//cli();
	while(time--)
	{
		PINC |= IR_LED;
		for (i=0;i<5;i++)
		{
			if (time_ms!=current_time_ms_touch)
			{
				i+=5;
				time_ms = current_time_ms_touch;
			}
		}
	}
	PORTC &= ~IR_LED;
	//sei();

}
static void pause(uint16_t time)
{
	time_ms = current_time_ms_touch;
	volatile uint8_t i;
	while(time--)
	{
		for (i=0;i<5;i++)
		{
			if (time_ms!=current_time_ms_touch)
			{
				i+=5;
				time_ms = current_time_ms_touch;
			}
		}
	}
}

extern void SendCommand(uint16_t cmd)
{
	for(uint8_t j=0;j<10;j++)
	{
		blink(START_LEN);
		pause(START_PAUSE);
		blink(PULSE_LEN);
		for (uint8_t state=0;state<4;state++)
		{
			for (uint8_t i=0;i<8;i++)
			{
				pause(PULSE_LEN);
				switch(state)
				{
					case 0:	//неинверированный адрес
						if (cmd & (1<<(15-i)))
						{
							pause(LEN_1);
						}
					break;
					case 1:	//инверированный адрес
						if (!(cmd & (1<<(15-i))))
						{
							pause(LEN_1);
						}
					break;
					case 2:	//неинверированная команда
						if (cmd & (1<<(7-i)))
						{
							pause(LEN_1);
						}
					break;
					case 3:	//инверированная команда
						if (!(cmd & (1<<(7-i))))
						{
							pause(LEN_1);
						}
					break;
				}
				blink(PULSE_LEN);
			}
		}
		pause(START_PAUSE*20);
	}
	//beep(1);
}

extern void init_ir()
{
	DDRC |= IR_LED;
}

