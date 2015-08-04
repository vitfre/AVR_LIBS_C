//#include <iom16v.h>
#include <iom8v.h>
#include <macros.h>
#include <stdlib.h>
#include <eeprom.h>
#include <math.h>

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
#define LCD_DBI PIND
#define LCD_IO DDRD
#define LCD_COM PORTC

#include "delay.h"
#include "ks0108.h"

void main()
{
 unsigned char x;
 DDRC=0xFF;
 DDRD=0xFF;
 Delay_ms(100);
 SetBit(LCD_COM,LCD_RST);
 Delay_ms(10);  
 init_lcd(); 
 Delay_ms(10);
 
 clear();
 
 Delay_ms(10);
 *textx=5;
 texty=3;
 puts("–‡‰ËÓ Œ“",8,0);
 
 textx=3;
 texty=4;
 puts("www.radiokot.ru",15,0xff);
  
 while(1)
 {
  SetCur(5,3);
  Delay_ms(500);
  SetCur(6,3);
  Delay_ms(500);
  SetCur(7,3);
  Delay_ms(500);
  SetCur(8,3);
  Delay_ms(500);
  SetCur(9,3);
  Delay_ms(500);
  SetCur(10,3);
  Delay_ms(500);
  SetCur(11,3);
  Delay_ms(500);
  SetCur(12,3);
  Delay_ms(500); 
 } 
}