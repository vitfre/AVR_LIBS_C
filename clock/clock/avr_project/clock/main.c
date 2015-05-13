/*******************************************************************************
*   $FILE:  main.c
*   Atmel Corporation:  http://www.atmel.com \n
*   Support email:  touch@atmel.com
******************************************************************************/

/*  License
*   Copyright (c) 2010, Atmel Corporation All rights reserved.
*
*   Redistribution and use in source and binary forms, with or without
*   modification, are permitted provided that the following conditions are met:
*
*   1. Redistributions of source code must retain the above copyright notice,
*   this list of conditions and the following disclaimer.
*
*   2. Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
*   3. The name of ATMEL may not be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
*   THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
*   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
*   SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
*   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
*   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*----------------------------------------------------------------------------
                            compiler information
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                include files
----------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#define __delay_cycles(n)     __builtin_avr_delay_cycles(n)
#define __enable_interrupt()  sei()

#include "touch_api.h"
#include "touch.h"

#include "indication.h"
#include "buttons.h"
#include "ds3231.h"
#include "alarm.h"
#include "ir.h"

/*----------------------------------------------------------------------------
                                    macros
----------------------------------------------------------------------------*/

#define GET_SENSOR_STATE(SENSOR_NUMBER) qt_measure_data.qt_touch_status.sensor_states[(SENSOR_NUMBER/8)] & (1 << (SENSOR_NUMBER % 8))
#define GET_ROTOR_SLIDER_POSITION(ROTOR_SLIDER_NUMBER) qt_measure_data.qt_touch_status.rotor_slider_values[ROTOR_SLIDER_NUMBER]

/*----------------------------------------------------------------------------
                                prototypes
----------------------------------------------------------------------------*/
extern void touch_measure(void);
extern void touch_init( void );
extern void init_system( void );
extern void init_timer_isr(void);
extern void set_timer_period(uint16_t);

extern uint8_t Indicator[];
extern uint8_t dot_mask;

extern mode_t Mode;

volatile uint8_t status=0;
dstime_t current_time={0x88,0x88,1};

extern void init_ir();
extern void SendCommand(uint16_t cmd);
extern void init_comparator();
extern void check_comparator();

/*----------------------------------------------------------------------------
                                global variables
----------------------------------------------------------------------------*/
/* Timer period in msec. */
uint16_t qt_measurement_period_msec = QT_MEASUREMENT_PERIOD_MS;
uint16_t time_ms_inc=0;

/*----------------------------------------------------------------------------
                                static variables
----------------------------------------------------------------------------*/

/* flag set by timer ISR when it's time to measure touch */
volatile uint8_t time_to_measure_touch = 0u;

/* current time, set by timer ISR */
volatile uint16_t current_time_ms_touch = 0u;

/*============================================================================
Name    :   main
------------------------------------------------------------------------------
Purpose :   main code entry point
Input   :   n/a
Output  :   n/a
Notes   :
============================================================================*/

int main( void )
{

	/* initialise host app, pins, watchdog, etc */
	init_system();

	/* configure timer ISR to fire regularly */
	init_timer_isr();

	/* Initialize Touch sensors */
	touch_init();
	
	init_indication();
	init_alarms();
	init_ir();
	init_comparator();

	/* enable interrupts */
	__enable_interrupt();
	
	init_rtc();

	Mode = MODE_TIME;
	status |= TIME_UPDATED;

	/* loop forever */
	for( ; ; )
	{
		touch_measure();
		ButtonHandler(CheckSensors());
		
		if (status & TIME_UPDATED)	//1 сек прерывание от DS
		{
			if(GetTime(&current_time))	//изменились минуты
			{
				//проверить будильники
				CheckAlarms();
				
				if(Mode==MODE_TIME)
				{
					ShowTime();
					//Show(current_time.hour, current_time.min);
				}
			}
			//звонки
			if (status & RINGING)
			{
				beep(0b1001);
			}
			//проверить яркость внешнего освещения
			check_comparator();
			status &= ~TIME_UPDATED;
		}
		if (status & (RINGING | SNOOZE_ON))
		{
			blink_led(LED1);
		}
		if(Mode==MODE_SHOW_TEMP)
		{
			ShowTemp(0);
		}
	}
}
