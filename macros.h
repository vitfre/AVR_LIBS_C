////////////////////////////////////////////////////////////////////////////////////////////////////
//	Макроопределения.
//
//  (C) Евгений Трифонов, Денис Железняков. 2001 - 2007 г.
//	Версия: 2.7.1 от 18.12.2007
////////////////////////////////////////////////////////////////////////////////////////////////////


/*

Набор полезных макросов для работы с портами и не только.

Не правились с 2007 года, так как я перешел на GCC-AVR, а напарник стал писать на C# для ПК.

Комментарии нормальные, разобраться можно, но если что пишите.

Пример работы:
#include &amp;amp;amp;quot;macros.h&amp;amp;amp;quot;
 
#define Example    B, 5, Low, On
 
main(void)
{
  // установка вывода 5 порта В на выход;
  Pin_Out(Example);
 
  while(1)
  {
    // изменение состояния вывода
    Pin_Change(Example);
  }
}
	
*/	
	
#include &amp;amp;amp;quot;macros.h&amp;amp;amp;quot;
 
#define Example    B, 5, Low, On
 
main(void)
{
  // установка вывода 5 порта В на выход;
  Pin_Out(Example);
 
  while(1)
  {
    // изменение состояния вывода
    Pin_Change(Example);
  }
}




// Параметры генератора (частота генератора указывается в Гц.)
#ifndef __MCU_Clock
  #warning Тактовая частота процессора (__MCU_Clock) не указана! Используется значение по-умолчанию: 9 216 000 Гц
  #define __MCU_Clock   9216000
#endif

//
#ifndef __Macros_Included__
	#define __Macros_Included__

//
#ifndef ENABLE_BIT_DEFINITIONS
	#define ENABLE_BIT_DEFINITIONS
#endif

//
#include <ioavr.h>
#include <ina90.h>

// Циклы
#define Hz(Value)       ((1 / (float)(Value)) * __MCU_Clock)
#define kHz(Value)      Hz((Value) * 0x000003E8)
#define MHz(Value)      Hz((Value) * 0x000F4240)
#define _ns             * ((float)(__MCU_Clock) / 0x3B9ACA00)
#define _us             * ((float)(__MCU_Clock) / 0x000F4240)
#define _mks            _us
#define _ms             * ((float)(__MCU_Clock) / 0x000003E8)
#define _s              * ((float)(__MCU_Clock))
#define _cycle
#define _cycles

// Задержка
#define Delay(Value)            __delay_cycles(Value)
#define Delay_ns(Value) 	      Delay((Value)_ns)
#define Delay_us(Value) 	      Delay((Value)_us)
#define Delay_mks(Value)        Delay((Value)_mks)
#define Delay_ms(Value) 	      Delay((Value)_ms)
#define Delay_s(Value)  	      Delay((Value)_s)
#define Delay_Hz(Value)         Delay(Hz(Value))
#define Delay_kHz(Value) 	      Delay(kHz(Value))
#define Delay_MHz(Value)  	    Delay(MHz(Value))
#define Delay_Cycle(Value)      Delay(Value)
#define Delay_Cycles(Value)	    Delay(Value)

// Таймеры
#define TC16_Compare(Div, Value)      (unsigned  int)((1 / (float)(Div)) * (Value))
#define TC16_Overflow(Div, Value)     (unsigned  int)(0x10000 - TC16_Compare(Div, Value))
#define TC8_Compare(Div, Value)       (unsigned char)TC16_Compare(Div, Value)
#define TC8_Overflow(Div, Value)      (unsigned char)(0x00100 - TC8_Compare(Div, Value))

// Битовые операции
#define Bit(Idx)                  		(1 << (Idx))
#define _Bit(Idx)                 		Bit(Idx)

// Изменение состояния обного бита в переменной или регистре
//
//  Примеры:
//      Set_Bit(TCCR1A, WGM10);       - установка бита WGM10 в регистре TCCR1A
//      Clear_Bit(TCCR1B, CS10);      - сброс бита CS10 в регистре TCCR1B
//      Get_Bit(TCCR1A, WGM10);       - получение значения бита WGM10 в регистре TCCR1A
//      Invert_Bit(TCCR1B, CS10);     - инвертирование бита CS10 в регистре TCCR1B
//      Is_Bit_Set(TCCR1A, WGM10);    - возвращает истину если бит WGM10 в регистре TCCR1A установлен
//      Is_Bit_Clear(TCCR1B, CS10);   - возвращает истину если бит CS10 в регистре TCCR1B сброшен
//
#define Set_Bit(Var, BitIdx)          ((Var) |=  _Bit(BitIdx))
#define Clear_Bit(Var, BitIdx)        ((Var) &= ~_Bit(BitIdx))
#define Get_Bit(Var, BitIdx)          ((Var)  &  _Bit(BitIdx))
#define Invert_Bit(Var, BitIdx)       ((Var) ^=  _Bit(BitIdx))
#define Toggle_Bit(Var, BitIdx)       Invert_Bit(Var, BitIdx)
#define Is_Bit_Set(Var, BitIdx)       (Get_Bit(Var, BitIdx) == _Bit(BitIdx))
#define Is_Bit_Clear(Var, BitIdx)     (Get_Bit(Var, BitIdx) == 0x00)

// Изменение состояния переменной или регистра по указанной маске
//
//  Примеры:
//      Set_Bits(TCCR1A, _Bit(WGM10) | _Bit(COM1A1));      - установка битов WGM10 и COM1A1 в регистре TCCR1A
//      Set_Bits(TCCR1A, ~_Bit(WGM10) | _Bit(COM1A1));     - сброс бита WGM10 и установка бита COM1A1 в регистре TCCR1A
//      Clear_Bits(TCCR1A, ~_Bit(WGM10) | _Bit(COM1A1));   - установка бита WGM10 и сброс бита COM1A1 в регистре TCCR1A
//
#define Set_Bits(Var, Mask)           ((Var) |=  (Mask))
#define Clear_Bits(Var, Mask)         ((Var) &= ~(Mask))
#define Invert_Bits(Var, Mask)        ((Var) ^=  (Mask))
#define Toggle_Bits(Var, Mask)        Invert_Bits(Var, (Mask))

// Изменение состояния обного бита в переменной или регистре посредством определённых флагов (шаблонов)
//
//  Примеры:
//      volatile Byte System_Status = 0x00;       - объявление переменной, где будут меняться флаги
//
//      #define Error_Flag    System_Status, 0    - объявление флага в переменной System_Status в бите 0
//      #define Ready_Flag    System_Status, 1    - объявление флага в переменной System_Status в бите 1
//      #define Delay_Flag    System_Status, 3    - объявление флага в переменной System_Status в бите 3
//      #define RX_Flag       System_Status, 6    - объявление флага в переменной System_Status в бите 6
//      #define TX_Flag       System_Status, 8    - объявление флага в переменной System_Status в бите 8
//
//      Set_Flag(Ready_Flag);         - установка бита 1 в переменной System_Status
//      Set_Flag(RX_Flag);            - установка бита 6 в переменной System_Status
//      Clear_Flag(TX_Flag);          - сброс бита 8 в переменной System_Status
//
#define Set_Flag(FlagDef)           Set_Bit(FlagDef)
#define Clear_Flag(FlagDef)         Clear_Bit(FlagDef)
#define Get_Flag(FlagDef)           Get_Bit(FlagDef)
#define Invert_Flag(FlagDef)        Invert_Bit(FlagDef)
#define Toggle_Flag(FlagDef)        Toggle_Bit(FlagDef)
#define Is_Flag_Set(FlagDef)        Is_Bit_Set(FlagDef)
#define Is_Flag_Clear(FlagDef)      Is_Bit_Clear(FlagDef)

//
#define BIN(Value)    (((0x##Value##L >> 0x0015) & 0x8000) + ((0x##Value##L >> 0x0012) & 0x4000) +\
                       ((0x##Value##L >> 0x000F) & 0x2000) + ((0x##Value##L >> 0x000C) & 0x1000) +\
                       ((0x##Value##L >> 0x0009) & 0x0800) + ((0x##Value##L >> 0x0006) & 0x0400) +\
                       ((0x##Value##L >> 0x0003) & 0x0200) + ((0x##Value##L  & 0x0100)) +\
                       ((0x##Value##L >> 0x0015) & 0x0080) + ((0x##Value##L >> 0x0012) & 0x0040) +\
                       ((0x##Value##L >> 0x000F) & 0x0020) + ((0x##Value##L >> 0x000C) & 0x0010) +\
                       ((0x##Value##L >> 0x0009) & 0x0008) + ((0x##Value##L >> 0x0006) & 0x0004) +\
                       ((0x##Value##L >> 0x0003) & 0x0002) + ((0x##Value##L  & 0x0001)))

// Скорость порта UART
#define Baud_Rate(BaudRate)       (unsigned  int)((unsigned long)(__MCU_Clock) / (((BaudRate) * 0x10L) - 1))
#define Baud_RateL(BaudRate)      (unsigned char)((unsigned long)(__MCU_Clock) / (((BaudRate) * 0x10L) - 1))
#define Baud_RateH(BaudRate)      (unsigned char)((unsigned long)(__MCU_Clock) / (((BaudRate) * 0x10L) - 1) >> 8)

// Переименование типов

#ifndef bool
  #include <stdbool.h>
#endif

// Булевый тип
typedef bool                  Boolean;

// Целый тип, 8 бит
typedef unsigned char				  Byte;
typedef unsigned char				  u08;
typedef signed char				    ShortInt;
typedef signed char				    s08;
typedef signed char				    SByte;

// Целый тип, 16 бит
typedef unsigned int				  Word;
typedef unsigned int				  u16;
typedef unsigned int          UInt16;
typedef signed int            SmallInt;
typedef signed int            s16;
typedef signed int            SInt16;

// Целый тип, 32 бита
typedef unsigned long				  DWord;
typedef unsigned long				  Cardinal;
typedef unsigned long				  u32;
typedef unsigned long         UInt32;
typedef signed long				    Integer;
typedef signed long				    s32;
typedef signed long           SInt32;

// Целый тип, 64 бита
typedef unsigned long long    DDWord;
typedef unsigned long	long	  u64;
typedef unsigned long long    UInt64;
typedef signed long long	    Int64;
typedef signed long	long	    s64;
typedef signed long	long	    SInt64;

// Вещественный тип, 32 бита
typedef float                 Float;

//Вещественный тип, 32 бита (если разрешено в настройках, то 64 бита)
typedef double                Double;
typedef long double           Extended;

//
typedef union
{
	unsigned char Byte;
	struct
	{
		unsigned char
			Bit0: 1,
			Bit1: 1,
			Bit2: 1,
			Bit3: 1,
			Bit4: 1,
			Bit5: 1,
			Bit6: 1,
			Bit7: 1;
	};
} Byte_Bit;

//
typedef union
{
	unsigned int Word;
	struct
	{
		unsigned char Byte0;
		unsigned char Byte1;
	};
	struct
	{
		unsigned int
			Bit0:  1,
			Bit1:  1,
			Bit2:  1,
			Bit3:  1,
			Bit4:  1,
			Bit5:  1,
			Bit6:  1,
			Bit7:  1,
			Bit8:  1,
			Bit9:  1,
			Bit10: 1,
			Bit11: 1,
			Bit12: 1,
			Bit13: 1,
			Bit14: 1,
			Bit15: 1;
	};
} Word_Byte_Bit;

//
typedef union
{
  unsigned int Word;
  struct
  {
		unsigned char Byte0;
		unsigned char Byte1;
	};
} Word_Byte;

//
typedef union
{
  unsigned long DWord;
  struct
  {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
	};
} DWord_Byte;

//
typedef union
{
  unsigned long DWord;
  struct
  {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
	};
  struct
  {
    unsigned long
			Bit0:  1,
			Bit1:  1,
			Bit2:  1,
			Bit3:  1,
			Bit4:  1,
			Bit5:  1,
			Bit6:  1,
			Bit7:  1,
			Bit8:  1,
			Bit9:  1,
			Bit10: 1,
			Bit11: 1,
			Bit12: 1,
			Bit13: 1,
			Bit14: 1,
			Bit15: 1,
			Bit16: 1,
			Bit17: 1,
			Bit18: 1,
			Bit19: 1,
			Bit20: 1,
			Bit21: 1,
			Bit22: 1,
			Bit23: 1,
			Bit24: 1,
			Bit25: 1,
			Bit26: 1,
			Bit27: 1,
			Bit28: 1,
			Bit29: 1,
			Bit30: 1,
			Bit31: 1;
	};
} DWord_Byte_Bit;

//
typedef union
{
  unsigned long long DDWord;
  struct
  {
		unsigned int Word0;
		unsigned int Word1;
		unsigned int Word2;
		unsigned int Word3;
	};
  struct
  {
    unsigned char Byte0;
    unsigned char Byte1;
    unsigned char Byte2;
    unsigned char Byte3;
    unsigned char Byte4;
    unsigned char Byte5;
		unsigned char Byte6;
		unsigned char Byte7;
	};
} DDWord_Byte;

//
typedef union
{
  unsigned long long DDWord;
  struct
  {
		unsigned int Word0;
		unsigned int Word1;
		unsigned int Word2;
		unsigned int Word3;
	};
  struct
  {
    unsigned char Byte0;
    unsigned char Byte1;
    unsigned char Byte2;
    unsigned char Byte3;
    unsigned char Byte4;
    unsigned char Byte5;
		unsigned char Byte6;
		unsigned char Byte7;
	};
  struct
  {
    unsigned long
			Bit0:  1,
			Bit1:  1,
			Bit2:  1,
			Bit3:  1,
			Bit4:  1,
			Bit5:  1,
			Bit6:  1,
			Bit7:  1,
			Bit8:  1,
			Bit9:  1,
			Bit10: 1,
			Bit11: 1,
			Bit12: 1,
			Bit13: 1,
			Bit14: 1,
			Bit15: 1,
			Bit16: 1,
			Bit17: 1,
			Bit18: 1,
			Bit19: 1,
			Bit20: 1,
			Bit21: 1,
			Bit22: 1,
			Bit23: 1,
			Bit24: 1,
			Bit25: 1,
			Bit26: 1,
			Bit27: 1,
			Bit28: 1,
			Bit29: 1,
			Bit30: 1,
			Bit31: 1,
			Bit32: 1,
			Bit33: 1,
			Bit34: 1,
			Bit35: 1,
			Bit36: 1,
			Bit37: 1,
			Bit38: 1,
			Bit39: 1,
			Bit40: 1,
			Bit41: 1,
			Bit42: 1,
			Bit43: 1,
			Bit44: 1,
			Bit45: 1,
			Bit46: 1,
			Bit47: 1,
			Bit48: 1,
			Bit49: 1,
			Bit50: 1,
			Bit51: 1,
			Bit52: 1,
			Bit53: 1,
			Bit54: 1,
			Bit55: 1,
			Bit56: 1,
			Bit57: 1,
			Bit58: 1,
			Bit59: 1,
			Bit60: 1,
			Bit61: 1,
			Bit62: 1,
			Bit63: 1;
	};
} DDWord_Byte_Bit;


////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Операции с портами.
//
//  Синтаксис:
//      #define <имя>   <Letter>, <Idx>, [<Level>], [<PullUp>]
//
//      <Letter>  - Буква порта (A, B, C, и т. д.).
//      <Idx>     - Номер вывода порта (0-7).
//      <Level>   - Активный уровень. (High или Low, если не указан, то High);
//                    High - логическая единица;
//                    Low  - логический ноль.
//      <PullUp>  - Внешний подтягивающий резистор. (On или Off, если не указан, то Off);
//                    On  - используется внешний подтягивающий резистор;
//                    Off - внешнего подтягивающего резастора нет.
//
//  Описание макрокоманд:
//      Pin_HiZ (Set_Pin_HiZ)             - если PullUp = Off, то вывода порта устанавливается в "третье" (Hi-Z) состояние;
//      Pin_PullUp_IO (Set_Pin_PullUp_IO) - установка вывода порта для работы с внешним "подтягивающим" резистором;
//      Pin_In (Set_Pin_In)               - установка вывода порта как вход;
//      Pin_Out (Set_Pin_Out)             - установка вывода порта как выход;
//      Pin_On (Set_Pin_On)               - установка активного логического уровня на выводе порта;
//      Pin_High (Set_Pin_High)           - установка "единичного" логического уровня на выводе порта;
//      Pin_Off (Set_Pin_Off)             - установка неактивного логического уровня на выводе порта;
//      Pin_Low (Set_Pin_Low)             - установка "нулевого" логического уровня на выводе порта;
//      Pin_Signal (Get_Pin_Signal)       - проверка логического уровня на выводе порта;
//      Pin_Change (Change_Pin_Signal)    - изменение логического уровня на выводе порта на противоположное;
//      Pin_Invert (Invert_Pin_Signal)    - аналогично Pin_Change;
//      Pin_Out_On (Set_Pin_Out_On)       - установка вывода порта как выход и установка активного логического уровня;
//      Pin_Out_Off (Set_Pin_Out_Off)     - установка вывода порта как выход и установка неактивного логического уровня;
//      Pin_Out_High (Set_Pin_Out_High)   - установка вывода порта как выход и установка единичного логического уровня на выводе порта;
//      Pin_Out_Low (Set_Pin_Out_Low)     - установка вывода порта как выход и установка нулевого логического уровня на выводе порта.
//
//  Примечания:
//      1. Если в определении вывода порта указано, что используется внешний подтягивающий
//         резистор (PullUp = On), то макрокоманды Pin_In и Pin_Out выполняют действия,
//         аналогичные макрокоманде Pin_PullUp_IO.
//      2. В скобках указаны макрокоманды, выполняющие аналогичные действия, но параметры для
//         них передаются не через определение define, а также как в функцию.
//      3. В определении define последние два параметра можно не указывать, но ЗАПЯТЫЕ ОПУСКАТЬ НЕЛЬЗЯ.
//
//  Примеры:
//      #define Example1    B, 5, Low,
//      #define Example2    B, 5,,
//      #define Example3    B, 5, Low, On
//      #define Example4    B, 5,, Off
//
//      Pin_Out(Example1);	- установка вывода 5 порта В на выход;
//      Pin_On(Example1);		- установка указанного активного уровня (Low) на выводе 5 порта B.
//
//  Пример к примечанию 2:
//      Макрокоманда Set_Pin_On(B, 5, Low, On) выполнит аналогичные действия, что и команда Pin_On(Example3)
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// Основные операции с портами
#define Set_PORT_Bit(Ltr, Idx)        (PORT##Ltr |=  _Bit(Idx))
#define Clr_PORT_Bit(Ltr, Idx)        (PORT##Ltr &= ~_Bit(Idx))
#define Invert_PORT_Bit(Ltr, Idx)     (PORT##Ltr ^=  _Bit(Idx))
#define Xor_PORT_Bit(Ltr, Idx)        Invert_PORT_Bit(Ltr, Idx)
#define Toggle_PORT_Bit(Ltr, Idx)     Invert_PORT_Bit(Ltr, Idx)
#define Set_DDR_Bit(Ltr, Idx)         (DDR##Ltr |=  _Bit(Idx))
#define Clr_DDR_Bit(Ltr, Idx)         (DDR##Ltr &= ~_Bit(Idx))
#define Invert_DDR_Bit(Ltr, Idx)      (DDR##Ltr ^=  _Bit(Idx))
#define Xor_DDR_Bit(Ltr, Idx)         Invert_DDR_Bit(Ltr, Idx)
#define Toggle_DDR_Bit(Ltr, Idx)      Invert_DDR_Bit(Ltr, Idx)
#define Get_PIN_State(Ltr, Idx)       (PIN##Ltr  &  _Bit(Idx))

// Установка вывода порта в третье состояние (Hi-Z)
#define Set_Pin_HiZ(Ltr, Idx, Level, PullUp)         { Clr_DDR_Bit(Ltr, Idx); Clr_PORT_Bit(Ltr, Idx); }
#define Pin_HiZ(PortDef)                             Set_Pin_HiZ(PortDef)

// Установка вывода порта для работы с внешним подтягивающим резистором
#define SetPinPullUp(Ltr, Idx)
#define SetPinPullUpOff(Ltr, Idx)
#define SetPinPullUpOn(Ltr, Idx)                     Clr_PORT_Bit(Ltr, Idx)
#define Set_Pin_PullUp_IO(Ltr, Idx, Level, PullUp)   { Clr_DDR_Bit(Ltr, Idx); SetPinPullUp##PullUp(Ltr, Idx); }
#define Pin_PullUp_IO(PortDef)                       Set_Pin_PullUp_IO(PortDef)

// Установка вывода порта как вход
#define Set_Pin_In(Ltr, Idx, Level, PullUp)          Set_Pin_PullUp_IO(Ltr, Idx, Level, PullUp)
#define Pin_In(PortDef)                              Set_Pin_In(PortDef)

// Установка вывода порта как выход
#define SetPinOut(Ltr, Idx)                          Set_DDR_Bit(Ltr, Idx)
#define SetPinOutOff(Ltr, Idx)                       Set_DDR_Bit(Ltr, Idx)
#define SetPinOutOn(Ltr, Idx)                        Clr_DDR_Bit(Ltr, Idx)
#define Set_Pin_Out(Ltr, Idx, Level, PullUp)         { SetPinOut##PullUp(Ltr, Idx); SetPinPullUp##PullUp(Ltr, Idx); }
#define Pin_Out(PortDef)	                           Set_Pin_Out(PortDef)

// Установка активного логического уровня на выводе порта
#define SetPin(Ltr, Idx)                             Set_PORT_Bit(Ltr, Idx)
#define SetPinHighOn(Ltr, Idx)	                     Clr_DDR_Bit(Ltr, Idx)
#define SetPinHighOff(Ltr, Idx)	                     Set_PORT_Bit(Ltr, Idx)
#define SetPinLowOn(Ltr, Idx)	                       Set_DDR_Bit(Ltr, Idx)
#define SetPinLowOff(Ltr, Idx)	                     Clr_PORT_Bit(Ltr, Idx)
#define SetPinHigh(Ltr, Idx)	                       Set_PORT_Bit(Ltr, Idx)
#define SetPinLow(Ltr, Idx)	                         Clr_PORT_Bit(Ltr, Idx)
#define SetPinOn(Ltr, Idx)                           Clr_DDR_Bit(Ltr, Idx)
#define SetPinOff(Ltr, Idx)                          Set_PORT_Bit(Ltr, Idx)
#define Set_Pin_On(Ltr, Idx, Level, PullUp)	         SetPin##Level##PullUp(Ltr, Idx)
#define Pin_On(PortDef)                              Set_Pin_On(PortDef)

// Установка вывода порта как выход и установка активного логического уровня
#define Set_Pin_Out_On(Ltr, Idx, Level, PullUp)      { Set_Pin_Out(Ltr, Idx, Level, PullUp); Set_Pin_On(Ltr, Idx, Level, PullUp); }
#define Pin_Out_On(PortDef)                          Set_Pin_Out_On(PortDef)

// Установка единичного логического уровня на выводе порта
#define Set_Pin_High(Ltr, Idx, Level, PullUp)        Set_Pin_On(Ltr, Idx, High, PullUp)
#define Pin_High(PortDef)                            Set_Pin_High(PortDef)

// Установка вывода порта как выход и установка единичного логического уровня на выводе порта
#define Set_Pin_Out_High(Ltr, Idx, Level, PullUp)    { Set_Pin_Out(Ltr, Idx, Level, PullUp); Set_Pin_High(Ltr, Idx, Level, PullUp); }
#define Pin_Out_High(PortDef)                        Set_Pin_Out_High(PortDef)

// Установка неактивного логического уровня на выводе порта
#define ClrPin(Ltr, Idx)                             Clr_PORT_Bit(Ltr, Idx)
#define ClrPinLow(Ltr, Idx)	                         Set_PORT_Bit(Ltr, Idx)
#define ClrPinHigh(Ltr, Idx)	                       Clr_PORT_Bit(Ltr, Idx)
#define ClrPinOff(Ltr, Idx)	                         Clr_PORT_Bit(Ltr, Idx)
#define ClrPinLowOff(Ltr, Idx)	                     Set_PORT_Bit(Ltr, Idx)
#define ClrPinHighOff(Ltr, Idx)	                     Clr_PORT_Bit(Ltr, Idx)
#define ClrPinOn(Ltr, Idx)	                         Set_DDR_Bit(Ltr, Idx)
#define ClrPinLowOn(Ltr, Idx)	                       Clr_DDR_Bit(Ltr, Idx)
#define ClrPinHighOn(Ltr, Idx)	                     Set_DDR_Bit(Ltr, Idx)
#define Set_Pin_Off(Ltr, Idx, Level, PullUp)	       ClrPin##Level##PullUp(Ltr, Idx)
#define Pin_Off(PortDef)	                           Set_Pin_Off(PortDef)

// Установка неактивного логического уровня на выводе порта
#define Set_Pin_Out_Off(Ltr, Idx, Level, PullUp)     { Set_Pin_Out(Ltr, Idx, Level, PullUp); Set_Pin_Off(Ltr, Idx, Level, PullUp); }
#define Pin_Out_Off(PortDef)                         Set_Pin_Out_Off(PortDef)

// Установка нулевого логического уровня на выводе порта
#define Set_Pin_Low(Ltr, Idx, Level, PullUp)         Set_Pin_Off(Ltr, Idx, High, PullUp)
#define Pin_Low(PortDef)                             Set_Pin_Low(PortDef)

// Установка вывода порта как выход и установка нулевого логического уровня на выводе порта
#define Set_Pin_Out_Low(Ltr, Idx, Level, PullUp)     { Set_Pin_Out(Ltr, Idx, Level, PullUp); Set_Pin_Low(Ltr, Idx, Level, PullUp); }
#define Pin_Out_Low(PortDef)                         Set_Pin_Out_Low(PortDef)

// Проверка логического уровня на выводе порта
#define GetPinSignal(Ltr, Idx)	                     Get_PIN_State(Ltr, Idx)
#define GetPinSignalLow(Ltr, Idx)	                   (!Get_PIN_State(Ltr, Idx))
#define GetPinSignalHigh(Ltr, Idx)	                 Get_PIN_State(Ltr, Idx)
#define Get_Pin_Signal(Ltr, Idx, Level, PullUp)      GetPinSignal##Level(Ltr, Idx)
#define Get_Pin_State(Ltr, Idx, Level, PullUp)       GetPinSignal##Level(Ltr, Idx)
#define Pin_Signal(PortDef)                          Get_Pin_Signal(PortDef)
#define Pin_State(PortDef)                           Get_Pin_Signal(PortDef)
#define Test_Pin(PortDef)                            Get_Pin_Signal(PortDef)

// Изменение логического уровня на выводе порта на противоположное
#define Invert(Ltr, Idx)                             Invert_PORT_Bit(Ltr, Idx)
#define InvertOff(Ltr, Idx)                          Invert_PORT_Bit(Ltr, Idx)
#define InvertOn(Ltr, Idx)                           Invert_DDR_Bit(Ltr, Idx)
#define Change_Pin_Signal(Ltr, Idx, Level, PullUp)	 Invert##PullUp(Ltr, Idx)
#define Invert_Pin_Signal(Ltr, Idx, Level, PullUp)	 Change_Pin_Signal(Ltr, Idx, Level, PullUp)
#define Toggle_Pin_Signal(Ltr, Idx, Level, PullUp)	 Change_Pin_Signal(Ltr, Idx, Level, PullUp)
#define Pin_Change(PortDef)                          Invert_Pin_Signal(PortDef)
#define Pin_Invert(PortDef)                          Invert_Pin_Signal(PortDef)
#define Pin_Toggle(PortDef)                          Invert_Pin_Signal(PortDef)
#define Change_Pin(PortDef)                          Invert_Pin_Signal(PortDef)
#define Invert_Pin(PortDef)                          Invert_Pin_Signal(PortDef)
#define Toggle_Pin(PortDef)                          Invert_Pin_Signal(PortDef)

////////////////////////////////////////////////////////////////////////////////////////////////////

//
#define BYTES(Value)    ((Byte *) & (Value))
#define WORDS(Value)    ((Word *) & (Value))

// Конфигурационные биты и биты блокировки
#define _DataToZByte(Addr, Value)     __AddrToZByteToSPMCR_LPM((void __flash *)(Addr), (Value))
#define _DataToR0Byte(Addr, Value)    __DataToR0ByteToSPMCR_SPM((Addr), (Value))
#define Get_Lock_Bits()               _DataToZByte(0x0001, 0x09)
#define Set_Lock_Bits(Value)          _DataToR0Byte(Value, 0x09)
#define Get_Low_Fuses()               _DataToZByte(0x0000, 0x09)
#define Get_High_Fuses()              _DataToZByte(0x0003, 0x09)
#define Get_Extended_Fuses()          _DataToZByte(0x0002, 0x09)
#define Enable_RWW_Section()          _DataToR0Byte(0x00, 0x11)

//
#define INLINE                              _Pragma("inline=forced")
#define DisableUndefinedBehaviorWarning			_Pragma(diag_suppress=Pa082)
#define EnableUndefinedBehaviorWarning			_Pragma(diag_default=Pa082)

//
#define Array_Length(Value)     (sizeof(Value) / sizeof(Value[0]))

#endif	// __Macros_Included__
