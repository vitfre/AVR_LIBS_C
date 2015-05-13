/*
 * comparator.c
 *
 * Created: 26.10.2014 14:36:14
 *  Author: Evgeny
 */ 
#include "comparator.h"
#include "alarm.h"
#include <avr/interrupt.h>

extern uint8_t status;

void init_comparator()
{
	DIDR1 |= ((1<<AIN1D) | (1<<AIN0D));
	if (ACSR & (1<<ACO))
	{
		status |= IS_DARK;
	}
}

void check_comparator()
{
	if (ACSR & (1<<ACO))
	{
		status &= ~IS_DARK;
	}
	else
	{
		status |= IS_DARK;
	}

	return;
	if (PINB & (1<<PINB3))
	{
		status &= ~IS_DARK;
	}
	else
	{
		status |= IS_DARK;
	}
	
	return;
	
	DDRB |= (1<<PORTB3);
	PORTB &= ~(1<<PORTB3);
	DDRB &= ~(1<<PORTB3);

	if (ACSR & (1<<ACO))
	{
		status &= ~IS_DARK;
	}
	else
	{
		status |= IS_DARK;
	}
}

