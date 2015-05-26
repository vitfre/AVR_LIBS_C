/*
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXI                             IXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXI     B   u   r     A   n     IXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXI                             IXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXI                                                     IXXXI
IXXXI                  LCD USART monitor                  IXXXI
IXXXI                                                     IXXXI
IXXXI                       v 0.20                        IXXXI
IXXXI                                                     IXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXXXXXXXXXI                                     IXXXXXXXXXXXI
IXXXXXXXXXXXI          target : atmega 8          IXXXXXXXXXXXI
IXXXXXXXXXXXI           clock : 8000000 Hz        IXXXXXXXXXXXI
IXXXXXXXXXXXI        compiler : AVR-GCC 3.3.4     IXXXXXXXXXXXI
IXXXXXXXXXXXI                                     IXXXXXXXXXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXI                                                     IXXXI
IXXXI             last change : 19/03/2013                IXXXI
IXXXI               copyright : BurAn                     IXXXI
IXXXI                                                     IXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
IXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXI
*/
 
 
 // LCD display 128x64 w ST7920 driver connection:
 // D0..D7 - PB0..7
 // E      - PD7
 // RS     - PD6
 
 // pushbutton connection:
 // PD2
 
 // LEDs connections:
 // lost     - PD3
 // warning  - PD4
 // received - PD5
 
 


#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "font5x5full.h"

//#define CLOCK_PIN PD3 // E
//#define DATA_PIN  PD2 // RW

#define E_PIN PD7
#define RS_PIN PD6

#define LOST_LED (1<<PD3)
#define WARN_LED (1<<PD4)
#define RCV_LED (1<<PD5)

#define BUTTON (1<<PD2) // int0 pin

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) -1 )

#define MODE TWBR

#define FLAGS EEDR
#define FLAG_MODE_CHNG  0b00000001
#define FLAG_BUTT_STATE 0b00000010
#define FLAG_BUTT_DBCED 0b00001000


uint8_t screen[21][10];				
uint8_t line_pointer = 0;

volatile uint8_t serial_buffer[64];
volatile uint8_t buff_pointer = 0;



int modulo(uint16_t a, uint16_t b)
{
    uint16_t c = a - b * (a / b);
    return (c);
}

/*
void write_serial_byte(uint8_t data)
{
    uint8_t i;
    for ( i = 0; i < 8;  i ++ )
    {
        if (data & (1 << (7-i)))
        {
			PORTD |= (1 << DATA_PIN);
        }
        else
        {
            PORTD &= ~(1 << DATA_PIN);
        }
		PORTD |= (1 << CLOCK_PIN);
		_delay_us(5);
		PORTD &= ~(1 << CLOCK_PIN);
    }
}

*/

void lcd_instruction(uint8_t instruction)
{
	PORTB = instruction;
	PORTD &= ~(1 << RS_PIN);
	
	PORTD |= (1 << E_PIN);
	_delay_us(5);
	PORTD &= ~(1 << E_PIN);
	_delay_us(40);
	
	/*
    int high_n, low_n;
    high_n = instruction;
    high_n &= 0xF0;
    low_n = instruction;
    low_n &= 0x0F;
    low_n <<= 4;
    write_serial_byte(0b11111000);
    write_serial_byte(high_n);
    write_serial_byte(low_n);
	//_delay_us(20);
	*/
}

void lcd_data(uint8_t data)
{
	PORTB = data;
	PORTD |= (1 << RS_PIN);
	
	PORTD |= (1 << E_PIN);
	_delay_us(5);
	PORTD &= ~(1 << E_PIN);
	_delay_us(40);
	
	/*
    int high_n, low_n;
    high_n = data;
    high_n &= 0xF0;
    low_n = data;
    low_n &= 0x0F;
    low_n <<= 4;
    write_serial_byte(0b11111010);
    write_serial_byte(high_n);
    write_serial_byte(low_n);
	//_delay_us(20);
	*/
}

void init_ST7920()
{
    _delay_ms(50);
	lcd_instruction(0b00110000);  // function set	
	lcd_instruction(0b00110000);  // function set
	lcd_instruction(0b00001100);  // display ON
	lcd_instruction(0b00000001);  // display clear
	_delay_ms(10);
	lcd_instruction(0b00000110);  // entry mode set
	lcd_instruction(0b00110000);  // function set
	lcd_instruction(0b00110100); // extended set
	lcd_instruction(0b00110110); // extended set graphic display ON
}

void init_screen()
{
	uint8_t x,y;
	for (y = 0; y < 10; y++)
	{
		for (x = 0; x<21; x++)
		{
			screen[x][y] = 32; // 0 je mezera
		}
	}
}

void clear_gLCD()
{
	/*
	uint8_t y_pos, x_pos;   
    for (y_pos = 0; y_pos < 32; y_pos ++)
    {
        for (x_pos = 0; x_pos < 16; x_pos ++)
        {
            lcd_instruction(y_pos + 0x80);
            lcd_instruction(x_pos + 0x80);
            lcd_data(0xff);
            lcd_data(0xff);
        }
    }
	
	_delay_ms(500);
	*/
	uint8_t y_pos, x_pos;   
	for (y_pos = 0; y_pos < 32; y_pos ++)
    {
        for (x_pos = 0; x_pos < 16; x_pos ++)
        {
            lcd_instruction(y_pos + 0x80);
            lcd_instruction(x_pos + 0x80);
            lcd_data(0x00);
            lcd_data(0x00);
        }
    }
	
}        

void clear_cLCD()
{
	lcd_instruction(0b00000001);
	_delay_ms(1);
}

void render_line(uint8_t line)
{
	uint8_t l;					// radek pixelu, je jich 6, posledni prazdny
	for (l = 0; l < 6; l++)
	{		
		uint16_t segment = 0;  // techto segmentu je na jednom radku 8
			
		uint8_t ch_pointer = 0;  // ch_pointer je pozice pixelu co se zrovna pocita
		
		uint8_t s;					// 8 segmentu
		for (s = 0; s < 8; s ++)
		{			
			uint8_t b = 15;               // v kazdym segmentu 16 bitu
			do 
			{
				uint8_t row = ch_pointer / 6;      
				
				uint8_t znak = screen [row] [line]; // znak co se bude kreslit
				
				if ((modulo(ch_pointer,6) == 5) || (l ==5) || (ch_pointer > 125)) // mezery 
				{
					segment &= ~(1 << b);
				}
				else 
				if ((pgm_read_byte(&font[(znak*5) + (l*640) + modulo(ch_pointer,6)])))
				{
					segment |= (1 << b);
				}
				else
				{
					segment &= ~(1 << b);
				}
				
				ch_pointer ++;	
				
			} while (b--);
			
			
			uint8_t posx, posy;
			posy = (line*6) + l;
			if (posy > 53) posy = posy + 4;
			if (posy < 32)
			{
				posy = (posy + 0x80);
				posx = s + 0x80;
			}
			else
			{
				posy = (posy + 0x80 - 32);
				posx = s + 0x88;
			}
			lcd_instruction(posy); 
			lcd_instruction(posx); 			
			lcd_data((segment >> 8) & 0xff);
			lcd_data (segment       & 0xff);		
			
		}
	}
}

void render_screen()
{
	uint8_t l = 8;
	do 
	{
		render_line(l);
	} while (l--);
}

void new_line()
{
	uint8_t x,y;
	for(y = 0; y < 8; y ++)
	{
		for (x = 0; x < 21; x ++)
		{
			screen[x][y] = screen[x][y+1];
		}			
	}
	for (x= 1; x < 21; x++)
	{
		screen[x][8] = 32; // 32 je mezera
	}
	screen[0][8] = 62;   // znak >
	line_pointer = 1;
	render_screen();
}

void cont_line()
{
	uint8_t x,y;
	for(y = 0; y < 8; y ++)
	{
		for (x = 0; x < 21; x ++)
		{
			screen[x][y] = screen[x][y+1];
		}			
	}
	for (x= 0; x < 21; x++)
	{
		screen[x][8] = 32; // mezera
	}
	line_pointer = 0;
	render_screen();
}

void monitor(uint8_t ch)
{
	uint8_t buff[21] = {48,48,48,32,32,32,48,88,48,48,32,48,66,48,48,48,48,48,48,48,48};
	
	char transbuff[10];
	
	uint8_t j = 0;
	uint8_t k;
	
	// dec
	utoa(ch, &transbuff[0], 10);
	j = 0;
	while (transbuff[j] != 0) j++;
	for (k = 0; k < j; k++)
	{
		buff[k+3-j] = transbuff[k];
	}
	
	// hex
	utoa(ch, &transbuff[0], 16);
	j = 0;
	while (transbuff[j] != 0) j++;
	
	for (k = 0; k < j; k++)
	{
		buff[k+10-j] = transbuff[k];
	}
	
	// bin
	utoa(ch, &transbuff[0], 2);
	j = 0;
	while (transbuff[j] != 0) j++;
	
	for (k = 0; k < j; k++)
	{
		buff[k+21-j] = transbuff[k];
	}
	
	// printable character
	if (ch > 127) ch = 127; // out of ascii
	buff[4] = ch;
	
	uint8_t i;
	for (i=0; i < 21; i ++)
	{
		screen[i][9] = buff[i];
	}
	
	render_line(9);
}

void new_char(uint8_t ch)
{
	if (ch == 10)
	{
		new_line();
	}
	else
	{	
		if (ch > 127) ch = 0; // out of ascii
		
		screen[line_pointer][8] = ch;
		line_pointer ++;
		render_line(8);	
	}	
		
	if (line_pointer > 20)
	{
		cont_line();
	}
}





int main(void)
{
    DDRB =  0b11111111; //
	PORTB = 0b00000000; // 
	
	DDRD =  0b11111011;
	PORTD = 0b00000100;    //pullup

	EEDR = 0x00; // clear flags
	MODE = 0x00; // default mode
	
	UCSRB |= (1 << RXEN);	// usart receive on
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // usart init
	UBRRH = (BAUD_PRESCALE >> 8);
	UBRRL = BAUD_PRESCALE;
	UCSRB |= (1 << RXCIE); // interrupt receive

	TCCR0 = (1<<CS02) | (1<<CS00); // prescaler 256
	TIMSK |= (1<<TOIE0);  // timer 0 overflow interrupt enable
	
	//MCUCR &= ~(1 << ISC01) & ~(1 << ISC00) ;  // interrupt for low level on INT0 pin
	//GICR |= (1 << INT0);
	
	init_ST7920();
	init_screen();
	clear_gLCD();
	
	new_line();
	
	sei();
	
	
    for (;;)
	{   	
	
		if (PIND & (1<<PD2))
		{
			if(buff_pointer > 0) // v bufferu neco je
			{	
				uint8_t r = serial_buffer[0]; // nacteme to
			
				uint8_t l;
				for (l = 0; l < buff_pointer; l ++)
				{
					serial_buffer[l] = serial_buffer[l+1];  // posuneme buffer doleva
				}
				if (buff_pointer > 0) buff_pointer --;
			
				new_char(r)	;
				monitor(r);
			}		
		}
	}		
}

ISR(USART_RXC_vect, ISR_BLOCK)
{
	uint8_t rcv = UDR;
	
	TCNT0 = 0;
	PORTD |= (RCV_LED);
	
	serial_buffer[buff_pointer] = rcv;
	buff_pointer ++;
	
	if (buff_pointer > 48)
	{
		PORTD |= WARN_LED;
	}
			
	if (buff_pointer > 64)
	{
		PORTD |= LOST_LED;
		buff_pointer = 64;
	}
}

ISR(TIMER0_OVF_vect)
{
	PORTD &= ~(RCV_LED);
	PORTD &= ~(LOST_LED);
	PORTD &= ~(WARN_LED);
}

/*
ISR(INT0_vect, ISR_BLOCK)
{	
	//
}
*/