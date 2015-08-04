
#include <iom8v.h>
#include <macros.h>
#include <stdlib.h>

#define SetBit(x,y) (x|=y)
#define ClrBit(x,y) (x&=~y)
#define TestBit(x,y) (x&y)

#define LCD_RST 0b00000001
#define LCD_E   0b00000010
#define LCD_RW  0b00000100
#define LCD_RS  0b00001000
#define LCD_CS2 0b00010000
#define LCD_CS1 0b00100000

#define LCD_DB PORTD
#define LCD_COM PORTC

#include "delay.h"
#include "ks0108.h"

void main()
{
 
 DDRC=0xFF;
 DDRD=0xFF; 
  
 init_lcd();
 
 Delay_ms(10);
 
 clear();
 
 Delay_ms(1);
 
 WriteXY(6,2,LCD_CS1); 
 Delay_ms(1); 
 WriteData(0b01000100,LCD_CS1);
 WriteData(0b01001000,LCD_CS1);
 WriteData(0b11110000,LCD_CS1);
 WriteData(0b00001000,LCD_CS1);
 WriteData(0b00000100,LCD_CS1);
 WriteData(0b01100010,LCD_CS1);
 WriteData(0b01100001,LCD_CS1);
 WriteData(0b00000010,LCD_CS1);
 WriteData(0b00000100,LCD_CS1);
 WriteData(0b00001000,LCD_CS1);
 WriteData(0b10010000,LCD_CS1);
 WriteData(0b01010000,LCD_CS1);
 WriteData(0b10010000,LCD_CS1);
 WriteData(0b00001000,LCD_CS1);
 WriteData(0b00000100,LCD_CS1);
 WriteData(0b01100010,LCD_CS1);
 WriteData(0b01100001,LCD_CS1);
 WriteData(0b00000010,LCD_CS1);
 WriteData(0b00000100,LCD_CS1);
 WriteData(0b00001000,LCD_CS1);
 WriteData(0b11110000,LCD_CS1);
 WriteData(0b01001000,LCD_CS1);
 WriteData(0b01000100,LCD_CS1);
 
 WriteXY(6,3,LCD_CS1);
 WriteData(0b00001000,LCD_CS1);
 WriteData(0b00000100,LCD_CS1);
 WriteData(0b00000011,LCD_CS1);
 WriteData(0b00000100,LCD_CS1);
 WriteData(0b00001000,LCD_CS1);
 WriteData(0b00010000,LCD_CS1);
 WriteData(0b00100000,LCD_CS1);
 WriteData(0b01000100,LCD_CS1);
 WriteData(0b10001000,LCD_CS1);
 WriteData(0b10010000,LCD_CS1);
 WriteData(0b10010101,LCD_CS1);
 WriteData(0b10010010,LCD_CS1);
 WriteData(0b10010101,LCD_CS1);
 WriteData(0b10010000,LCD_CS1);
 WriteData(0b10001000,LCD_CS1);
 WriteData(0b01000100,LCD_CS1);
 WriteData(0b00100000,LCD_CS1);
 WriteData(0b00010000,LCD_CS1);
 WriteData(0b00001000,LCD_CS1);
 WriteData(0b00000100,LCD_CS1);
 WriteData(0b00000011,LCD_CS1);
 WriteData(0b00000100,LCD_CS1);
 WriteData(0b00001000,LCD_CS1); 
 
 while(1); 
}