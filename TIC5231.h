/*
 * TIC5231.h
 *
 * Created: 16.09.2014 10:52:10
 *  Author: Администратор
 */ 


#ifndef TIC5231_H_
#define TIC5231_H_
//---------------------------------------------------------------------------------------
#define F_CPU 16000000UL // or whatever may be your frequency
//---------------------------------------------------------------------------------------
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
//---------------------------------------------------------------------------------------
#define setbit(adress,bit) (adress |=(1<<bit))		//function is set bit
#define clearbit(adress,bit) (adress &=~(1<<bit))	//function is clear bit
#define LCD_off(void) (PORTB |=(1<<3))				//ON VCC at LCD module
#define LCD_on(void) (PORTB &=~(1<<3))				//OFF VCC at LCD module
//---------------------------------------------------------------------------------------
#define lcd_viv	_delay_us(20);	setbit(PORTB,1);	_delay_us(20);	clearbit(PORTB,1);


#define LCD1_CS  0
#define LCD_DIN  1
#define LCD_CLK  2
//---------------------------------------------------------------------------------------
#define Minus1  0b00001100
#define Min     0b00000000
#define CharC   0b10001111
#define Charo   0b11100011
#define CharP   0b11010111
//---------------------------------------------------------------------------------------


//**************************************************************************
unsigned int MAT(float data,unsigned char t_adr);

void BCDconv(unsigned int a,unsigned char *digit5,unsigned char *digit4,unsigned char *digit3,unsigned char *digit2,unsigned char *digit1,unsigned char *digit0);

unsigned char seg_conv(unsigned char data);

void lcdsnd(unsigned char lcd_bt,unsigned char sym,unsigned char dot);

void lcdclr(unsigned char lcd_adr,unsigned char a);

void viv_data(unsigned int a,unsigned char lcd_adr,unsigned char rec);

void viv_data_l(unsigned int a,unsigned char lcd_adr);










#endif /* TIC5231_H_ */