/*
 * beep.c
 *
 * Created: 06.10.2014 10:50:03
 *  Author: Evgeny
 */ 

#include "indication.h"
#include "buttons.h"
#include "ds3231.h"
#include "alarm.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static uint8_t SEGTABLE[] = {
	SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,					//0
	SEGMENT_B | SEGMENT_C,																	//1
	SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,								//2
	SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,								//3
	SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,											//4
	SEGMENT_A | SEGMENT_G | SEGMENT_D | SEGMENT_C | SEGMENT_F,								//5
	SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,					//6
	SEGMENT_A | SEGMENT_B | SEGMENT_C,														//7
	SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,		//8
	SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,					//9
	SEGMENT_E | SEGMENT_F | SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_G,					//A
	SEGMENT_F | SEGMENT_E | SEGMENT_G | SEGMENT_C | SEGMENT_D,								//b
	SEGMENT_A | SEGMENT_F | SEGMENT_E | SEGMENT_D,											//C
	SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_G,								//d
	SEGMENT_A | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,								//E
	SEGMENT_A | SEGMENT_E | SEGMENT_F | SEGMENT_G,											//F
	SEGMENT_F | SEGMENT_E | SEGMENT_D,														//L
	SEGMENT_E | SEGMENT_G | SEGMENT_C,														//n
	SEGMENT_F | SEGMENT_E | SEGMENT_G | SEGMENT_D,											//t
	SEGMENT_C | SEGMENT_D | SEGMENT_E,														//u
	SEGMENT_G,																				//-
	SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_F,											//°
	0x00																					//OFF
	
};

uint8_t Indicator[4] = {8,8,8,8};
volatile uint8_t dot_mask=0;
volatile uint8_t blink_mask=0;

static volatile uint8_t beep_pattern =0;
static volatile uint8_t port_bits = 0;
static volatile uint8_t blink_on=0;

extern volatile uint8_t status;
extern dstime_t current_time;
extern mode_t Mode;
extern volatile uint16_t current_time_ms_touch;


extern void led_on(uint8_t led)
{
	LED_PORT |= led;
}

extern void led_off(uint8_t led)
{
	LED_PORT &= ~led;
}

extern void blink_led(uint8_t led)
{
	static uint16_t prev_time=0;
	
	if ((current_time_ms_touch-prev_time) > 500)
	{
		prev_time = current_time_ms_touch;
		LED_PORT ^= led;
	}
}


extern void beep(uint8_t pattern)
{
	port_bits |= BEEPER1;
	beep_pattern = pattern;
}

static void beep_isr()
{
	static uint8_t count=0;

	if(beep_pattern)
	{
		if (beep_pattern & 0x01)
		{
			port_bits^=(BEEPER1 |BEEPER2);
		}
		count++;
		if(100==count)
		{
			count=0;
			beep_pattern>>=1;
			if (!beep_pattern)
			{
				port_bits &= ~(BEEPER1 | BEEPER2);
			}
		}
	}
}

extern void init_indication()
{
	DDRA = 0xFF;	//segment port
	DDRB = ANODE1 | ANODE2 | ANODE3 | ANODE4 | BEEPER1 | BEEPER2;	//anode port
	DDRC = LED1 | LED2;	//led port
	
	//interrupt for dot blinking
	PCMSK2 |= (1<<PCINT18);
	PCICR |= (1<<PCIE2);	//enable PCINT23:16

}

extern void indication_isr()
{
	static uint8_t CurDigit=0;
	static uint8_t tick=0;
	

	beep_isr();

	port_bits &=~(ANODE1|ANODE2|ANODE3|ANODE4);
	SEGMENT_PORT = 0;
	if (!(status & IS_DARK) || (tick>=2))
	{
		switch(CurDigit)
		{
			case 4:
			CurDigit=0;
			case 0:
			port_bits |= ANODE1;
			break;
			case 1:
			port_bits |= ANODE2;
			break;
			case 2:
			port_bits |= ANODE3;
			break;
			case 3:
			port_bits |= ANODE4;
			break;
		}
		if (!( blink_mask & (1 << CurDigit)) || blink_on)
		{
			SEGMENT_PORT = ( SEGTABLE[Indicator[CurDigit]] | ((dot_mask & (1<<CurDigit))?(SEGMENT_DP):0x00));
		}
	}
	
	tick++;
	if (tick>2)
	{
		tick=0;
		CurDigit++;
	}
	
	ANODE_PORT = port_bits;
	
}

ISR(PCINT2_vect)		//секундное прерывание, мигание точки
{
	blink_on^=1;
	
	if(blink_on)
	{
		status |= 1;
	}
	if (Mode ==	MODE_TIME)
	{
		dot_mask=(blink_on<<1);
	}
}

extern void ShowTemp(uint8_t show)
{
	static uint16_t time_show=0;
	blink_mask = 0;
	if(show)
	{
		time_show = current_time_ms_touch;
		uint16_t dstemp = GetTemp();
		Mode = MODE_SHOW_TEMP;
		dot_mask = 0b0010;
		if (dstemp & 0b1000000000)	//температура меньше нуля
		{
			Indicator[0] = IND_MINUS;
			dstemp &= ~0b1000000000;
			dstemp*=25;
			if (dstemp>9)
			{
				Indicator[1]=9;
				Indicator[2]=9;
			}
			else
			{
				Indicator[1]=(dstemp % 1000)/100;
				Indicator[2]=(dstemp % 100)/10;
			}
		}
		else
		{
			dstemp*=25;
			Indicator[0]=(dstemp % 10000)/1000;
			Indicator[1]=(dstemp % 1000)/100;
			Indicator[2]=(dstemp % 100)/10;
		}
		Indicator[3] = IND_DEGREE;
	}
	else if ((current_time_ms_touch-time_show)>4000)
	{
		Mode = MODE_TIME;
		blink_mask=0x00;
		dot_mask=0b0010;
		ShowTime();
	}
}

extern void Show(uint8_t HH, uint8_t MM)
{
	uint8_t hour10 = HH >> 4;
	if (!hour10)
	{
		Indicator[0] = IND_OFF;
	}
	else
	{
		Indicator[0] = hour10;
	}
	Indicator[1] = HH & 0x0F;
	Indicator[2] = MM >> 4;
	Indicator[3] = MM & 0x0F;
}

extern void ShowTime()
{
	GetTime(&current_time);
	Show(current_time.hour, current_time.min);
	dot_mask = 0b0010;
}

