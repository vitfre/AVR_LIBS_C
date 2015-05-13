/*
 * sensors_handler.c
 *
 * Created: 05.10.2014 15:41:44
 *  Author: Evgeny
 */ 
#include "buttons.h"
//#include "IncFile1.h"
#include "indication.h"
#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint16_t current_time_ms_touch;
extern mode_t Mode;
//extern volatile uint8_t beep;
#include "touch_api.h"
#include "touch.h"

#define GET_SENSOR_STATE(SENSOR_NUMBER) qt_measure_data.qt_touch_status.sensor_states[(SENSOR_NUMBER/8)] & (1 << (SENSOR_NUMBER % 8))

uint8_t CheckSensors()
{
	static uint8_t pressed = 0;
	static uint16_t button_touch_time;
	int i;
	for(i=0;i<3;i++)
	{
		if (GET_SENSOR_STATE(i))
		{
			if (!pressed) //кнопка нажата первый раз
			{
				pressed = 1;
				button_touch_time=current_time_ms_touch;
				return (i | BUTTON_PRESSED);
			}
			else//кнопка удерживается
			{
				if((2 == pressed) && ((current_time_ms_touch - button_touch_time)>200))
				{
					button_touch_time=current_time_ms_touch;
					return (i | BUTTON_RETRY);		//повтор нажатия
				}
				else if((current_time_ms_touch - button_touch_time)>1500)		//долгое нажатие
				{
					pressed=2;
					button_touch_time=current_time_ms_touch;
					return (i | BUTTON_LONGPRESSED);
				}
			}
			break;
		}
	}
	if (i==3)	//все кнопки отпущены
	{
		pressed = 0;
		return BUTTON_RELEASED;
	}
	return 0;
}

void ButtonHandler(uint8_t ButtonEvent)
{
	if(!ButtonEvent)
		return;
	
	switch (ButtonEvent & 0xF0)
	{
		case BUTTON_PRESSED:
			beep(0b001);
			switch(ButtonEvent & 0x0F)
			{
				case BUTTON1:
					Button1_pressed();
				break;
				case BUTTON2:
					Button2_pressed();
				break;
				case BUTTON3:
					Button3_pressed();
				break;
			}
		break;
		case BUTTON_RETRY:
			if (BUTTON2==(ButtonEvent & 0x0F))
			{
				if (Mode != MODE_TIME)
				{
					Button2_pressed();
				}
			}
		break;
		case BUTTON_LONGPRESSED:
			switch(ButtonEvent & 0x0F)
			{
				case BUTTON1:
					Button1_LP();
				break;
				case BUTTON2:
					Button2_LP();
				break;
				case BUTTON3:
					Button3_LP();
				break;
			}
		break;
		case BUTTON_RELEASED:
		break;
	}
	
}
