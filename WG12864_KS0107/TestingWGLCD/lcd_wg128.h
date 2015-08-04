#ifndef WG128_H
#define WG128_H

#include <avr/io.h>
#include <avrlibtypes.h>
#include <avrlibdefs.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

// Display

// Настройка портов для дисплея
// Порт данных
#define LCD_DATA_PORT 	PORTA
#define LCD_DATA_DDR 	DDRA
#define LCD_DATA_PIN	PINA

// Порт управления
#define LCD_CMD_PORT 	PORTB
#define LCD_CMD_DDR 	DDRB

// Биты порта управления.
#define DI 	0			// Данные/команда
#define RW 	1			// Чтение/запись
#define E 	2			// Строб
#define CS1 3			// Контроллер 1
#define CS2 4			// Контроллер 2
#define RST 5			// Сброс адреса
#define DBG	6			// Отладочный вывод. Может использоваться для управления подсветкой



// Дрыг отладочной ногой. 
#define DEBUG_UP		do{LCD_CMD_PORT |= 1<<DBG;}while(0)
#define DEBUG_DN		do{LCD_CMD_PORT &= ~(1<<DBG);}while(0)


// Инициализация порта.
#define LCD_PORT_INIT	do																				\
						{																				\
						LCD_DATA_DDR	 =0xFF;															\
						LCD_DATA_PORT 	 =0;															\
						LCD_CMD_DDR 	|= (1<<DI)|(1<<RW)|(1<<E)|(1<<CS1)|(1<<CS2)|(1<<RST)|(1<<DBG);	\
						LCD_CMD_PORT 	|= (1<<RST)|(1<<CS1)|(1<<CS2)|(0<<E); 							\
						}while(0)

// Сброс дисплея
#define LCD_RST		do							\
					{							\
					LCD_CMD_PORT &=~(1<<RST); 	\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					LCD_CMD_PORT |= 1<<RST;		\
					}while(0)					

// Шлем данные
#define LCD_SET_DAT	do							\
					{							\
					LCD_CMD_PORT |=1<<DI;		\
					}while(0)


// Шлем команду
#define LCD_SET_CMD	do							\
					{							\
					LCD_CMD_PORT &=~(1<<DI);	\
					}while(0)


// Порт на чтение
#define LCD_SET_R	do							\
					{							\
					LCD_DATA_DDR = 0;			\
					LCD_DATA_PORT = 0xFF;		\
					LCD_CMD_PORT |= 1<<RW; 		\
					}while(0)

// Порт на запись
#define LCD_SET_W	do							\
					{							\
					LCD_DATA_DDR = 0xFF;		\
					LCD_DATA_PORT = 0;			\
					LCD_CMD_PORT &=~(1<<RW);	\
					}while(0)

// Опустить строб
#define LCD_DN_E	do							\
					{							\
					LCD_CMD_PORT &=~(1<<E);		\
					}while(0)

// Дрыгнуть стробом
#define LCD_PUL_E	do							\
					{							\
					LCD_CMD_PORT |=1<<E;		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					LCD_CMD_PORT &=~(1<<E);		\
					}while(0)

// Поднять строб
#define LCD_UP_E	do							\
					{							\
					LCD_CMD_PORT |=1<<E;		\
					}while(0)



// Выставляем данные в порт
#define LCD_DATA_INS(x)		do					\
							{					\
							LCD_DATA_PORT = x;	\
							}while(0)



// Макросы одиночного включения и выключения сигналов разрешения контроллеров
// Для инверсного CS сигнала нужно поменять их местами. 
#define LCD_ON_CS1	do							\
					{							\
					LCD_CMD_PORT |=1<<CS1;		\
					}while(0)

#define LCD_ON_CS2	do							\
					{							\
					LCD_CMD_PORT |=1<<CS2;		\
					}while(0)


#define LCD_OFF_CS1	do							\
					{							\
					LCD_CMD_PORT &=~(1<<CS1);	\
					}while(0)

#define LCD_OFF_CS2	do							\
					{							\
					LCD_CMD_PORT &=~(1<<CS2);	\
					}while(0)


// большая задержка. Регулируется под быстродействие конкретного случая. Оптимизирована
// под 8МГЦ и один такт на команду.
#define NOPS		do							\
					{							\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					asm volatile("nop");		\
					}while(0)

// Малая задержка, регулируется исходя из быстродействия. Оптимизировано для 8МГЦ и одного
// такта на команду. 
#define NOPSm		do							\
					{							\
					asm volatile("nop");		\
					asm volatile("nop");		\
					}while(0)

//Активация контроллера. Параметр задается битмаской, например 1<<CS1
#define ON_CS(x)	do							\
					{							\
					LCD_CMD_PORT |= x;			\
					}while(0)					\

//Дезактивация контроллера. Параметр задается битмаской, например 1<<CS1
#define OFF_CS(x)	do							\
					{							\
					LCD_CMD_PORT &=~(x);		\
					}while(0)					\
// Для инверсных CS сигналов ON меняем на OFF, а OFF на ON

// Макрос установки текущего адреса. Адрес задается в виде страницы и координаты. 
// Страница (х) от 0 до 7, координата от 0 до 63
#define SET_ADDR(x, y, CSC) do							\
							{							\
							LCD_WR_COM(0x40+y,CSC);		\
							LCD_WR_COM(0xB8+x,CSC);		\
							}while(0)		


// Прототипы
// Инициализация дисплея
extern void LCD_INIT(void);

// Заливка памяти. Если заливать нулем, то будет очистка. 
extern void LCD_FILL(u08 byte);

// Запись команды и данных. Пишется команда cmd в контроллер CSC
// Контроллер задается битмаской т.е. вида 1<<CS1 можно и сразу в оба, если 
// маски склеить.
extern void LCD_WR_COM(u08 cmd,u08 CSC);
extern void LCD_WR_DATA(u08 cmd, u08 CSC);

// Чтение байта с текущего адреса из контроллера CSC
extern u08 LCD_RD_DATA(u08 CSC);

//===================================================
// Функция рисования пикселя. 
// Режимы 
#define INVERT 	1	// Инверсия текущего
#define CLEAR  	0	// Сброс текущего
#define SET		2	// Установка текущего

// х - координата Х от 0 до 63. Не путать со страницей
// у - координата У от 0 до 127
// mode - режим закраски.
extern void PIXEL(u08 x,u08 y,u08 mode);

//===================================================
// Функйия рисования блока. 
// На входе
// х - страница по Х от 0 до 7, 
// у - координата по Y от 0 до 127
// len - длинна блока в байтах. 
// addr - адрес во флеше где лежит блок. Может быть и просто где угодно, но
// тогда надо подправить функцию выборки байта. Либо поставить селектор. На кортексе такой проблемы
// быть не должно - единое пространство форева!
extern u08 BLOCK(u08 x,u08 y, u08 len, u16 addr);
//===================================================
// Функция рисования строки. Использует блоки и кодовую таблицу. Пока только английский
// Но можно допилить до полной видимой ASCII. Надо только case смонтировать который бы 
// смещения раздавал. 
// На входе 
// х - страница от 0 до 7
// у - координата по У от 0 до 127
// *string - указатель на строку
extern void LCD_putc(u08 x,u08 y,u08 *string);

#endif
