/*
 * lcd.c
 *
 * Created: 08.11.2013 12:20:25
 *  Author: Администратор
 */

#include "lcd.h"

//***************************************************************************************
//Procedure sending LCD byte
//Input: Byte, Kommand/Symbol data
//***************************************************************************************
void send_lcd_byte(unsigned char lcd_data,unsigned char rs_status)
{
	//unsigned char lcd_temp_data, preport;
	//lcd_temp_data=lcd_data<<4;
		if(rs_status==lcd_kom)
		{
			ClrBit(PORTB,lcd_rs);
		}
		else
		{
			SetBit(PORTB,lcd_rs);
		}
	//****************************************************************************************
    if (BitVal(lcd_data, 7)) 
	{
        SetBit(PORTB,5);
    } else {
        ClrBit(PORTB,5);
    };
    if (BitVal(lcd_data, 6)) {
        SetBit(PORTB,4);
    } else {
        ClrBit(PORTB,4);
    };
    if (BitVal(lcd_data, 5)) {
        SetBit(PORTB,3);
    } else {
        ClrBit(PORTB,3);
    };
    if (BitVal(lcd_data, 4)) {
        SetBit(PORTB,2);
    } else {
        ClrBit(PORTB,2);
    };
    //****************************************************************************************
	_delay_us(50);//_delay_ms(1);
	SetBit(PORTB,lcd_e);
	_delay_us(50);//_delay_ms(1);
	ClrBit(PORTB,lcd_e);
	_delay_us(50);//_delay_ms(1);
	//****************************************************************************************
    if (BitVal(lcd_data, 3))
    {
	    SetBit(PORTB,5);
	    } else {
	    ClrBit(PORTB,5);
    };
    if (BitVal(lcd_data, 2)) {
	    SetBit(PORTB,4);
	    } else {
	    ClrBit(PORTB,4);
    };
    if (BitVal(lcd_data, 1)) {
	    SetBit(PORTB,3);
	    } else {
	    ClrBit(PORTB,3);
    };
    if (BitVal(lcd_data, 0)) {
	    SetBit(PORTB,2);
	    } else {
	    ClrBit(PORTB,2);
    };
    //****************************************************************************************
	_delay_us(50);//_delay_ms(1);
	SetBit(PORTB,lcd_e);
	_delay_us(50);//_delay_ms(1);
	ClrBit(PORTB,lcd_e);
	_delay_us(50);//_delay_ms(1);
};
void send_lcd_byte(unsigned char lcd_data,unsigned char rs_status);
//***************************************************************************************
//Procedure initialisation LCD
//***************************************************************************************
void init_LCD(void)
{
	SetBit(PORTB,2);
	SetBit(PORTB,3);
	
	SetBit(PORTB,lcd_e);
	_delay_ms(20);
	ClrBit(PORTB,lcd_e);
	_delay_ms(20);
	
	SetBit(PORTB,lcd_e);
	_delay_ms(20);
	ClrBit(PORTB,lcd_e);
	_delay_ms(20);
	
	SetBit(PORTB,lcd_e);
	_delay_ms(20);
	ClrBit(PORTB,lcd_e);
	_delay_ms(20);
	
	ClrBit(PORTB,2);
	_delay_ms(10);
	SetBit(PORTB,lcd_e);
	_delay_ms(20);
	ClrBit(PORTB,lcd_e);
	_delay_ms(20);
	send_lcd_byte(0x28,lcd_kom);					//2 lines, 5*8 pixels, 8bit data
	send_lcd_byte(0x0C,lcd_kom);					//screen enable, 
	send_lcd_byte(0x06,lcd_kom);					//auto shift left enable
	send_lcd_byte(0x01,lcd_kom);
	_delay_ms(10);
};
//***************************************************************************************
//Procedure sending LCD symbol
//Input: Symbol
//***************************************************************************************
void send_lcd_symbol(unsigned char temp_symbol)
{
	send_lcd_byte(temp_symbol,lcd_sym);
};
//***************************************************************************************
//Procedure setting LCD position
//Input: Line(1,2), Position(1..16)
//***************************************************************************************
void set_lcd_position(unsigned char temp_line, unsigned char temp_pos)
{
	if(temp_line==1)
	{
		temp_line=0x80+temp_pos-1;
		send_lcd_byte(temp_line,lcd_kom);
	};
	if(temp_line==2)
	{
		temp_line=0xC0+temp_pos-1;
		send_lcd_byte(temp_line,lcd_kom);
	};
};
//***************************************************************************************
//Procedure send symbol string to LCD
//Input: Symbol string
//***************************************************************************************
void send_lcd_string(const char *temp_string)
{
	unsigned char i=0;
	/* for all chars in string */
	while(temp_string[i]!=0)
	{
		/* Send Write Data Start Byte */
		send_lcd_byte(temp_string[i],lcd_sym);
		i++;
	};
 };
//***************************************************************************************
//Отправить строку на индикатор  16 символов
//задаются все символы, можно в кодах в виде ('A',0x55)
//***************************************************************************************
void send_lcd_code_string(char s1,char s2,char s3,char s4,char s5,char s6,char s7,char s8,char s9,char s10,char s11,char s12,char s13,char s14,char s15,char s16)
{
	send_lcd_byte(s1,lcd_sym);
	send_lcd_byte(s2,lcd_sym);
	send_lcd_byte(s3,lcd_sym);
	send_lcd_byte(s4,lcd_sym);  
	send_lcd_byte(s5,lcd_sym);
	send_lcd_byte(s6,lcd_sym);
	send_lcd_byte(s7,lcd_sym);
	send_lcd_byte(s8,lcd_sym);
	send_lcd_byte(s9,lcd_sym);
	send_lcd_byte(s10,lcd_sym);
	send_lcd_byte(s11,lcd_sym);
	send_lcd_byte(s12,lcd_sym);
	send_lcd_byte(s13,lcd_sym);
	send_lcd_byte(s14,lcd_sym);
	send_lcd_byte(s15,lcd_sym);
	send_lcd_byte(s16,lcd_sym);          
 };
//***************************************************************************************
//Procedure clearing LCD
//***************************************************************************************
void clear_lcd(void)
{
	send_lcd_byte(0x01,lcd_kom);
	_delay_ms(10);//_delay_ms(300);
};
//***************************************************************************************
//Procedure detecting symbol
//Input: Digit,
//Output: Symbol
//***************************************************************************************
unsigned char detect_symbol(unsigned char temp_det_sym)
{
	switch(temp_det_sym)
	{
		case 0:{return '0';}break;
		case 1:{return '1';}break;
		case 2:{return '2';}break;	
		case 3:{return '3';}break;
		case 4:{return '4';}break;
		case 5:{return '5';}break;
		case 6:{return '6';}break;	
		case 7:{return '7';}break;
		case 8:{return '8';}break;
		case 9:{return '9';}break;
		default:return '0';
	};
};
//***************************************************************************************
//Вывести на индикатор короткую строку из 2-х цифр
//***************************************************************************************
void detect_short_string(unsigned long int temp_volt)
{
	unsigned char j;
	char det_string[3]={0,0,0};
	j=1;
	while(j<=2)
	{
		det_string[j]=detect_symbol(temp_volt%10);
		temp_volt=temp_volt/10;
		j++;
	};
	j=2;
	while(j>=1)
	{
		send_lcd_symbol(det_string[j]);
		j--;
	};	
};
//***************************************************************************************
//Procedure detecting string.
//Input: Digit data 4 symbol,
//Output: 4 symbol on LCD on presetting position
//***************************************************************************************
void detect_string(unsigned long int temp_volt)
{
	unsigned char j;
	char det_string[5]={0,0,0,0,0};
	j=1;
	while(j<=4)
	{
		det_string[j]=detect_symbol(temp_volt%10);
		temp_volt=temp_volt/10;
		j++;
	};
	j=4;
	while(j>=1)
	{
		send_lcd_symbol(det_string[j]);
		j--;
	};	
};
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************