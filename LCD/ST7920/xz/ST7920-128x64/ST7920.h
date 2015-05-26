/****************************************************/
/* Модуль для работы с LCD                          */
/* Автор:       SypH3r                              */
/* Website:     MCUS.RU                             */
/* Компиляторы: HI-TECH PICC18                      */
/****************************************************/

#ifndef ST7920_H
#define	ST7920_H

#define	X_FREQ      24

#define DelayUs(x) { unsigned int i=x/(40/X_FREQ) ; while(i--); }
#define DelayMs(x) { unsigned int i=x; while(i--){unsigned int a=830/(40/X_FREQ);  while(a--); }  }


// LCD Pin Connection in 4-bit mode
#define LCD_RS 		LATBbits.LATB2
#define	LCD_RW  	LATBbits.LATB0
#define	LCD_EN 		LATBbits.LATB3
#define	LCD_REST	LATBbits.LATB1
#define	LCD_DB4		LATBbits.LATB4
#define DB4_TRIS        TRISB4
#define	LCD_DB5		LATBbits.LATB5
#define DB5_TRIS        TRISB5
#define	LCD_DB6		LATBbits.LATB6
#define DB6_TRIS        TRISB6
#define	LCD_DB7		LATBbits.LATB7
#define DB7_TRIS        TRISB7

// LCD Functions
void lcd_init(void);
void lcd_puts(unsigned char line, char* str);
void lcd_image(const unsigned char* graphic);
void lcd_clear(void);
void lcd_putc_big(unsigned char line, unsigned char column, unsigned char ch);



#endif	

