/*
 * main.h
 *
 * Created: 11.02.2014 17:51:42
 *  Author: Администратор
 */ 



#ifndef LD_H_
#define LD_H_
//---------------------------------------------------------------------------------------
#define F_CPU 16000000UL // or whatever may be your frequency
//---------------------------------------------------------------------------------------
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/io.h>
//---------------------------------------------------------------------------------------
#include "usart_debug/buart.h"
#include "init_mcu.h"
#include "meas.h"
#include "4094.h"
//---------------------------------------------------------------------------------------
signed int TEMP = 0;
unsigned int PRES = 0;
unsigned int HUMID = 0;
//---------------------------------------------------------------------------------------
/*
void(* resetFunc) (void) = 0; // Reset MC function
//---------------------------------------------------------------------------------------
void Func_reset(void)
{
	// Watchdog Timer initialization
	// Watchdog Timer Prescaler: OSC/16k
	WDTCR=0x08;
	while (1)
	{
	};
};
*/
//---------------------------------------------------------------------------------------
#endif /* LD_H_ */