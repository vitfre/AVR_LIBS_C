/*
 * indication.h
 *
 * Created: 06.10.2014 13:27:13
 *  Author: Evgeny
 */ 


#ifndef INDICATION_H_
#define INDICATION_H_

#include <avr/io.h>

#define	 LED_PORT		PORTC
#define  ANODE_PORT		PORTB
#define  SEGMENT_PORT	PORTA
#define	 SEGMENT_A		(1<<PORTA5)
#define	 SEGMENT_B		(1<<PORTA7)
#define	 SEGMENT_C		(1<<PORTA2)
#define	 SEGMENT_D		(1<<PORTA1)
#define	 SEGMENT_E		(1<<PORTA0)
#define	 SEGMENT_F		(1<<PORTA3)
#define	 SEGMENT_G		(1<<PORTA6)
#define	 SEGMENT_DP		(1<<PORTA4)
#define  BEEPER1		(1<<PORTB0)
#define  BEEPER2		(1<<PORTB1)
#define  ANODE1			(1<<PORTB7)
#define  ANODE2			(1<<PORTB6)
#define  ANODE3			(1<<PORTB5)
#define  ANODE4			(1<<PORTB4)
#define	 LED2			(1<<PORTC4)
#define	 LED1			(1<<PORTC5)

#define BLINK_ALL	0b1111;
#define BLINK_HH	0b0011;
#define BLINK_MM	0b1100;

enum IndicatorValues
{
	IND_A = 10,
	IND_B,
	IND_C,
	IND_D,
	IND_E,
	IND_F,
	IND_L,
	IND_N,
	IND_T,
	IND_U,
	IND_MINUS,
	IND_DEGREE,
	IND_OFF
};

extern void beep(uint8_t pattern);

extern void led_on(uint8_t led);
extern void led_off(uint8_t led);
extern void blink_led(uint8_t led);

extern void init_indication();

extern void indication_isr();
extern void Show(uint8_t HH, uint8_t MM);
extern void ShowTime();
extern void ShowTemp(uint8_t show);

#endif /* INDICATION_H_ */