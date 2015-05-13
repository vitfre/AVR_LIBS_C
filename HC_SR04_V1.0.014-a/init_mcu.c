/*
 * init_mcu.c
 *
 * Created: 08.11.2013 8:58:37
 *  Author: Администратор
 */ 

#include "init_mcu.h"

//***************************************************************************************
void init_mcu(void)
{
	// Input/Output Ports initialization
	// Port B initialization
	// Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
	// State7=T State6=T State5=0 State4=0 State3=0 State2=0 State1=0 State0=0
	PORTB=0x00;
	DDRB=0x3F;

	// Port D initialization
	// Func7=In Func6=Out Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
	// State7=P State6=0 State5=T State4=T State3=T State2=P State1=T State0=T
	PORTD=0x84;
	DDRD=0x40;
	
	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 15,625 kHz
	TCCR0=0x05;
	TCNT0=0x00;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 16000,000 kHz
	// Mode: Normal top=0xFFFF
	// OC1A output: Discon.
	// OC1B output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer1 Overflow Interrupt: On
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A=0x00;
	TCCR1B=0x00;//TCCR1B=0x01;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: Timer2 Stopped
	// Mode: Normal top=0xFF
	// OC2 output: Disconnected
	ASSR=0x00;
	TCCR2=0x00;
	TCNT2=0x00;
	OCR2=0x00;

	// External Interrupt(s) initialization
	// INT0: On
	// INT0 Mode: Rising Edge
	// INT1: Off
	GICR=(1<<INT0);
	MCUCR=(1<<ISC01)|(1<<ISC00);											//Interrupt on rising edge of INT0
	GIFR=(1<<INTF0);
	
	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK=(1<<TOIE1)|(1<<TOIE0);

	// USART initialization
	// USART disabled
	UCSRB=0x00;

	// Analog Comparator initialization
	// Analog Comparator: Off
	// Analog Comparator Input Capture by Timer/Counter 1: Off
	ACSR=0x80;
	SFIOR=0x00;

	// ADC initialization
	// ADC disabled
	ADCSRA=0x00;

	// SPI initialization
	// SPI disabled
	SPCR=0x00;

	// TWI initialization
	// TWI disabled
	TWCR=0x00;
	
	_delay_ms(10);
};
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************