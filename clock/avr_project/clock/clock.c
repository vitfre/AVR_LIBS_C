/*
 * clock.c
 *
 * Created: 30.09.2014 15:02:42
 *  Author: Evgeny
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
//#include "IncFile1.h"
#include "indication.h"

#define QT_TIMER_PERIOD_MSEC  1u
#define QT_MEASUREMENT_PERIOD_MS 50u
#define __enable_interrupt()  sei()

#include "touch_api.h"
#include "touch.h"
#include "buttons.h"
#include "ds3231.h"
#include "alarm.h"

/* Timer period in msec. */
uint16_t qt_measurement_period_msec = QT_MEASUREMENT_PERIOD_MS;
uint16_t time_ms_inc=0;

//volatile uint8_t beep = 0;

/* flag set by timer ISR when it's time to measure touch */
volatile uint8_t time_to_measure_touch = 0u;

/* current time, set by timer ISR */
volatile uint16_t current_time_ms_touch = 0u;

extern uint8_t Indicator[];
extern uint8_t dot_mask;
//extern uint8_t blink_mask;

extern mode_t Mode;

volatile uint8_t status=0;
dstime_t current_time={0x88,0x88,1};

extern void init_ir();
extern void SendCommand(uint16_t cmd);

int main1( void )
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

	/* enable interrupts */
	__enable_interrupt();
	
	init_rtc();

	Mode = MODE_TIME;
	status |= TIME_UPDATED;

	/* loop forever */
	for( ; ; )
	{
 	  touch_measure();
		//ButtonHandler(CheckSensors());
		
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