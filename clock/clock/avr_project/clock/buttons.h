/*
 * buttons.h
 *
 * Created: 05.10.2014 15:42:16
 *  Author: Evgeny
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <avr/io.h>
#define BUTTON1		2
#define BUTTON2		0
#define BUTTON3		1

#define BUTTON_PRESSED		0x10
#define BUTTON_RETRY		0x20
#define BUTTON_RELEASED		0x30
#define BUTTON_LONGPRESSED	0x40

typedef enum
{
	MODE_TIME = 0,
	MODE_SEL_ALARM,
	MODE_SET_TIME,
	MODE_SET_DATE,
	MODE_SET_HH,
	MODE_SET_MM,
	MODE_SET_ALARM_HH,
	MODE_SET_ALARM_MM,
	MODE_SET_SS,
	MODE_SET_YEAR,
	MODE_SET_MONTH,
	MODE_SET_DAY,
	MODE_SET_ALARM_ENABLED,
	MODE_SET_ALARM_WEEKDAY,
	MODE_SET_ALARM_SOUND,
	MODE_SET_ALARM_SUNRISE,
	MODE_SET_ALARM_ACTION,
	MODE_SHOW_TEMP,
	MODE_SET_BRIGHTNESS
} mode_t;	//отображаемый параметр


uint8_t CheckSensors();
void ButtonHandler(uint8_t ButtonEvent);

void Button1_pressed();
void Button1_LP();
void Button2_pressed();
void Button3_pressed();
void Button3_LP();
void Button2_LP();

#endif /* BUTTONS_H_ */