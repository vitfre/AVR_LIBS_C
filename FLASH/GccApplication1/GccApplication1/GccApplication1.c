/*
 * GccApplication1.c
 *
 * Created: 19.11.2013 17:24:46
 *  Author: Администратор
 */ 

#define F_CPU 8000000L
#define XTAL 8000000L
#define baudrate 9600L
#define bauddivider (XTAL/(16*baudrate)-1)
#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)


#include <avr/io.h>
#include <avr/pgmspace.h>

//Прототипы функций
void SendStr_P(char const *string);
void SendByte(char byte);
//___________________________

//char const StringP[] PROGMEM = " Hello_IN_FLASH ";
char const StringP[] PROGMEM = {0x10,0x25,0x52};

int main(void)
{
	
	char const *z;
	
	// Инициализация периферии
	UBRRL = LO(bauddivider);
	UBRRH = HI(bauddivider);
	UCSRA = 0;
	UCSRB = 1<<RXEN|1<<TXEN|0<<RXCIE|0<<TXCIE;
	UCSRC = 1<<URSEL|1<<UCSZ0|1<<UCSZ1;
	
	
	// Главный код

	//z=StringP;		// И он ничем не отличается от указателя на RAM
	
	//SendStr_P(z);					// Печатаем по указателю из флеша
	SendStr_P(StringP);				// Печатаем по прямому адресу строки.
	//SendStr_P(PSTR("INLINE_IN_FLASH"));	// Инлайновая флеш строка
	
	
	while(1)
	{

	};
}

// Отправка строки из флеша
void SendStr_P(char const *string)
{
	while (pgm_read_byte(string)!='\0')
	{
		SendByte(pgm_read_byte(string));
		string++;
	}
}

// Отправка одного символа
void SendByte(char byte)
{
	while(!(UCSRA & (1<<UDRE)));
	UDR=byte;
}