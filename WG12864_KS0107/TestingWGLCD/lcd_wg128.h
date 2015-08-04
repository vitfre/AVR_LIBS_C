#ifndef WG128_H
#define WG128_H

#include <avr/io.h>
#include <avrlibtypes.h>
#include <avrlibdefs.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

// Display

// ��������� ������ ��� �������
// ���� ������
#define LCD_DATA_PORT 	PORTA
#define LCD_DATA_DDR 	DDRA
#define LCD_DATA_PIN	PINA

// ���� ����������
#define LCD_CMD_PORT 	PORTB
#define LCD_CMD_DDR 	DDRB

// ���� ����� ����������.
#define DI 	0			// ������/�������
#define RW 	1			// ������/������
#define E 	2			// �����
#define CS1 3			// ���������� 1
#define CS2 4			// ���������� 2
#define RST 5			// ����� ������
#define DBG	6			// ���������� �����. ����� �������������� ��� ���������� ����������



// ���� ���������� �����. 
#define DEBUG_UP		do{LCD_CMD_PORT |= 1<<DBG;}while(0)
#define DEBUG_DN		do{LCD_CMD_PORT &= ~(1<<DBG);}while(0)


// ������������� �����.
#define LCD_PORT_INIT	do																				\
						{																				\
						LCD_DATA_DDR	 =0xFF;															\
						LCD_DATA_PORT 	 =0;															\
						LCD_CMD_DDR 	|= (1<<DI)|(1<<RW)|(1<<E)|(1<<CS1)|(1<<CS2)|(1<<RST)|(1<<DBG);	\
						LCD_CMD_PORT 	|= (1<<RST)|(1<<CS1)|(1<<CS2)|(0<<E); 							\
						}while(0)

// ����� �������
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

// ���� ������
#define LCD_SET_DAT	do							\
					{							\
					LCD_CMD_PORT |=1<<DI;		\
					}while(0)


// ���� �������
#define LCD_SET_CMD	do							\
					{							\
					LCD_CMD_PORT &=~(1<<DI);	\
					}while(0)


// ���� �� ������
#define LCD_SET_R	do							\
					{							\
					LCD_DATA_DDR = 0;			\
					LCD_DATA_PORT = 0xFF;		\
					LCD_CMD_PORT |= 1<<RW; 		\
					}while(0)

// ���� �� ������
#define LCD_SET_W	do							\
					{							\
					LCD_DATA_DDR = 0xFF;		\
					LCD_DATA_PORT = 0;			\
					LCD_CMD_PORT &=~(1<<RW);	\
					}while(0)

// �������� �����
#define LCD_DN_E	do							\
					{							\
					LCD_CMD_PORT &=~(1<<E);		\
					}while(0)

// �������� �������
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

// ������� �����
#define LCD_UP_E	do							\
					{							\
					LCD_CMD_PORT |=1<<E;		\
					}while(0)



// ���������� ������ � ����
#define LCD_DATA_INS(x)		do					\
							{					\
							LCD_DATA_PORT = x;	\
							}while(0)



// ������� ���������� ��������� � ���������� �������� ���������� ������������
// ��� ���������� CS ������� ����� �������� �� �������. 
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


// ������� ��������. ������������ ��� �������������� ����������� ������. ��������������
// ��� 8��� � ���� ���� �� �������.
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

// ����� ��������, ������������ ������ �� ��������������. �������������� ��� 8��� � ������
// ����� �� �������. 
#define NOPSm		do							\
					{							\
					asm volatile("nop");		\
					asm volatile("nop");		\
					}while(0)

//��������� �����������. �������� �������� ���������, �������� 1<<CS1
#define ON_CS(x)	do							\
					{							\
					LCD_CMD_PORT |= x;			\
					}while(0)					\

//������������ �����������. �������� �������� ���������, �������� 1<<CS1
#define OFF_CS(x)	do							\
					{							\
					LCD_CMD_PORT &=~(x);		\
					}while(0)					\
// ��� ��������� CS �������� ON ������ �� OFF, � OFF �� ON

// ������ ��������� �������� ������. ����� �������� � ���� �������� � ����������. 
// �������� (�) �� 0 �� 7, ���������� �� 0 �� 63
#define SET_ADDR(x, y, CSC) do							\
							{							\
							LCD_WR_COM(0x40+y,CSC);		\
							LCD_WR_COM(0xB8+x,CSC);		\
							}while(0)		


// ���������
// ������������� �������
extern void LCD_INIT(void);

// ������� ������. ���� �������� �����, �� ����� �������. 
extern void LCD_FILL(u08 byte);

// ������ ������� � ������. ������� ������� cmd � ���������� CSC
// ���������� �������� ��������� �.�. ���� 1<<CS1 ����� � ����� � ���, ���� 
// ����� �������.
extern void LCD_WR_COM(u08 cmd,u08 CSC);
extern void LCD_WR_DATA(u08 cmd, u08 CSC);

// ������ ����� � �������� ������ �� ����������� CSC
extern u08 LCD_RD_DATA(u08 CSC);

//===================================================
// ������� ��������� �������. 
// ������ 
#define INVERT 	1	// �������� ��������
#define CLEAR  	0	// ����� ��������
#define SET		2	// ��������� ��������

// � - ���������� � �� 0 �� 63. �� ������ �� ���������
// � - ���������� � �� 0 �� 127
// mode - ����� ��������.
extern void PIXEL(u08 x,u08 y,u08 mode);

//===================================================
// ������� ��������� �����. 
// �� �����
// � - �������� �� � �� 0 �� 7, 
// � - ���������� �� Y �� 0 �� 127
// len - ������ ����� � ������. 
// addr - ����� �� ����� ��� ����� ����. ����� ���� � ������ ��� ������, ��
// ����� ���� ���������� ������� ������� �����. ���� ��������� ��������. �� �������� ����� ��������
// ���� �� ������ - ������ ������������ ������!
extern u08 BLOCK(u08 x,u08 y, u08 len, u16 addr);
//===================================================
// ������� ��������� ������. ���������� ����� � ������� �������. ���� ������ ����������
// �� ����� �������� �� ������ ������� ASCII. ���� ������ case ������������ ������� �� 
// �������� ��������. 
// �� ����� 
// � - �������� �� 0 �� 7
// � - ���������� �� � �� 0 �� 127
// *string - ��������� �� ������
extern void LCD_putc(u08 x,u08 y,u08 *string);

#endif
