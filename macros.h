////////////////////////////////////////////////////////////////////////////////////////////////////
//	����������������.
//
//  (C) ������� ��������, ����� ����������. 2001 - 2007 �.
//	������: 2.7.1 �� 18.12.2007
////////////////////////////////////////////////////////////////////////////////////////////////////


/*

����� �������� �������� ��� ������ � ������� � �� ������.

�� ��������� � 2007 ����, ��� ��� � ������� �� GCC-AVR, � �������� ���� ������ �� C# ��� ��.

����������� ����������, ����������� �����, �� ���� ��� ������.

������ ������:
#include &amp;amp;amp;quot;macros.h&amp;amp;amp;quot;
 
#define Example    B, 5, Low, On
 
main(void)
{
  // ��������� ������ 5 ����� � �� �����;
  Pin_Out(Example);
 
  while(1)
  {
    // ��������� ��������� ������
    Pin_Change(Example);
  }
}
	
*/	
	
#include &amp;amp;amp;quot;macros.h&amp;amp;amp;quot;
 
#define Example    B, 5, Low, On
 
main(void)
{
  // ��������� ������ 5 ����� � �� �����;
  Pin_Out(Example);
 
  while(1)
  {
    // ��������� ��������� ������
    Pin_Change(Example);
  }
}




// ��������� ���������� (������� ���������� ����������� � ��.)
#ifndef __MCU_Clock
  #warning �������� ������� ���������� (__MCU_Clock) �� �������! ������������ �������� ��-���������: 9 216 000 ��
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

// �����
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

// ��������
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

// �������
#define TC16_Compare(Div, Value)      (unsigned  int)((1 / (float)(Div)) * (Value))
#define TC16_Overflow(Div, Value)     (unsigned  int)(0x10000 - TC16_Compare(Div, Value))
#define TC8_Compare(Div, Value)       (unsigned char)TC16_Compare(Div, Value)
#define TC8_Overflow(Div, Value)      (unsigned char)(0x00100 - TC8_Compare(Div, Value))

// ������� ��������
#define Bit(Idx)                  		(1 << (Idx))
#define _Bit(Idx)                 		Bit(Idx)

// ��������� ��������� ������ ���� � ���������� ��� ��������
//
//  �������:
//      Set_Bit(TCCR1A, WGM10);       - ��������� ���� WGM10 � �������� TCCR1A
//      Clear_Bit(TCCR1B, CS10);      - ����� ���� CS10 � �������� TCCR1B
//      Get_Bit(TCCR1A, WGM10);       - ��������� �������� ���� WGM10 � �������� TCCR1A
//      Invert_Bit(TCCR1B, CS10);     - �������������� ���� CS10 � �������� TCCR1B
//      Is_Bit_Set(TCCR1A, WGM10);    - ���������� ������ ���� ��� WGM10 � �������� TCCR1A ����������
//      Is_Bit_Clear(TCCR1B, CS10);   - ���������� ������ ���� ��� CS10 � �������� TCCR1B �������
//
#define Set_Bit(Var, BitIdx)          ((Var) |=  _Bit(BitIdx))
#define Clear_Bit(Var, BitIdx)        ((Var) &= ~_Bit(BitIdx))
#define Get_Bit(Var, BitIdx)          ((Var)  &  _Bit(BitIdx))
#define Invert_Bit(Var, BitIdx)       ((Var) ^=  _Bit(BitIdx))
#define Toggle_Bit(Var, BitIdx)       Invert_Bit(Var, BitIdx)
#define Is_Bit_Set(Var, BitIdx)       (Get_Bit(Var, BitIdx) == _Bit(BitIdx))
#define Is_Bit_Clear(Var, BitIdx)     (Get_Bit(Var, BitIdx) == 0x00)

// ��������� ��������� ���������� ��� �������� �� ��������� �����
//
//  �������:
//      Set_Bits(TCCR1A, _Bit(WGM10) | _Bit(COM1A1));      - ��������� ����� WGM10 � COM1A1 � �������� TCCR1A
//      Set_Bits(TCCR1A, ~_Bit(WGM10) | _Bit(COM1A1));     - ����� ���� WGM10 � ��������� ���� COM1A1 � �������� TCCR1A
//      Clear_Bits(TCCR1A, ~_Bit(WGM10) | _Bit(COM1A1));   - ��������� ���� WGM10 � ����� ���� COM1A1 � �������� TCCR1A
//
#define Set_Bits(Var, Mask)           ((Var) |=  (Mask))
#define Clear_Bits(Var, Mask)         ((Var) &= ~(Mask))
#define Invert_Bits(Var, Mask)        ((Var) ^=  (Mask))
#define Toggle_Bits(Var, Mask)        Invert_Bits(Var, (Mask))

// ��������� ��������� ������ ���� � ���������� ��� �������� ����������� ����������� ������ (��������)
//
//  �������:
//      volatile Byte System_Status = 0x00;       - ���������� ����������, ��� ����� �������� �����
//
//      #define Error_Flag    System_Status, 0    - ���������� ����� � ���������� System_Status � ���� 0
//      #define Ready_Flag    System_Status, 1    - ���������� ����� � ���������� System_Status � ���� 1
//      #define Delay_Flag    System_Status, 3    - ���������� ����� � ���������� System_Status � ���� 3
//      #define RX_Flag       System_Status, 6    - ���������� ����� � ���������� System_Status � ���� 6
//      #define TX_Flag       System_Status, 8    - ���������� ����� � ���������� System_Status � ���� 8
//
//      Set_Flag(Ready_Flag);         - ��������� ���� 1 � ���������� System_Status
//      Set_Flag(RX_Flag);            - ��������� ���� 6 � ���������� System_Status
//      Clear_Flag(TX_Flag);          - ����� ���� 8 � ���������� System_Status
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

// �������� ����� UART
#define Baud_Rate(BaudRate)       (unsigned  int)((unsigned long)(__MCU_Clock) / (((BaudRate) * 0x10L) - 1))
#define Baud_RateL(BaudRate)      (unsigned char)((unsigned long)(__MCU_Clock) / (((BaudRate) * 0x10L) - 1))
#define Baud_RateH(BaudRate)      (unsigned char)((unsigned long)(__MCU_Clock) / (((BaudRate) * 0x10L) - 1) >> 8)

// �������������� �����

#ifndef bool
  #include <stdbool.h>
#endif

// ������� ���
typedef bool                  Boolean;

// ����� ���, 8 ���
typedef unsigned char				  Byte;
typedef unsigned char				  u08;
typedef signed char				    ShortInt;
typedef signed char				    s08;
typedef signed char				    SByte;

// ����� ���, 16 ���
typedef unsigned int				  Word;
typedef unsigned int				  u16;
typedef unsigned int          UInt16;
typedef signed int            SmallInt;
typedef signed int            s16;
typedef signed int            SInt16;

// ����� ���, 32 ����
typedef unsigned long				  DWord;
typedef unsigned long				  Cardinal;
typedef unsigned long				  u32;
typedef unsigned long         UInt32;
typedef signed long				    Integer;
typedef signed long				    s32;
typedef signed long           SInt32;

// ����� ���, 64 ����
typedef unsigned long long    DDWord;
typedef unsigned long	long	  u64;
typedef unsigned long long    UInt64;
typedef signed long long	    Int64;
typedef signed long	long	    s64;
typedef signed long	long	    SInt64;

// ������������ ���, 32 ����
typedef float                 Float;

//������������ ���, 32 ���� (���� ��������� � ����������, �� 64 ����)
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
//  �������� � �������.
//
//  ���������:
//      #define <���>   <Letter>, <Idx>, [<Level>], [<PullUp>]
//
//      <Letter>  - ����� ����� (A, B, C, � �. �.).
//      <Idx>     - ����� ������ ����� (0-7).
//      <Level>   - �������� �������. (High ��� Low, ���� �� ������, �� High);
//                    High - ���������� �������;
//                    Low  - ���������� ����.
//      <PullUp>  - ������� ������������� ��������. (On ��� Off, ���� �� ������, �� Off);
//                    On  - ������������ ������� ������������� ��������;
//                    Off - �������� �������������� ��������� ���.
//
//  �������� �����������:
//      Pin_HiZ (Set_Pin_HiZ)             - ���� PullUp = Off, �� ������ ����� ��������������� � "������" (Hi-Z) ���������;
//      Pin_PullUp_IO (Set_Pin_PullUp_IO) - ��������� ������ ����� ��� ������ � ������� "�������������" ����������;
//      Pin_In (Set_Pin_In)               - ��������� ������ ����� ��� ����;
//      Pin_Out (Set_Pin_Out)             - ��������� ������ ����� ��� �����;
//      Pin_On (Set_Pin_On)               - ��������� ��������� ����������� ������ �� ������ �����;
//      Pin_High (Set_Pin_High)           - ��������� "����������" ����������� ������ �� ������ �����;
//      Pin_Off (Set_Pin_Off)             - ��������� ����������� ����������� ������ �� ������ �����;
//      Pin_Low (Set_Pin_Low)             - ��������� "��������" ����������� ������ �� ������ �����;
//      Pin_Signal (Get_Pin_Signal)       - �������� ����������� ������ �� ������ �����;
//      Pin_Change (Change_Pin_Signal)    - ��������� ����������� ������ �� ������ ����� �� ���������������;
//      Pin_Invert (Invert_Pin_Signal)    - ���������� Pin_Change;
//      Pin_Out_On (Set_Pin_Out_On)       - ��������� ������ ����� ��� ����� � ��������� ��������� ����������� ������;
//      Pin_Out_Off (Set_Pin_Out_Off)     - ��������� ������ ����� ��� ����� � ��������� ����������� ����������� ������;
//      Pin_Out_High (Set_Pin_Out_High)   - ��������� ������ ����� ��� ����� � ��������� ���������� ����������� ������ �� ������ �����;
//      Pin_Out_Low (Set_Pin_Out_Low)     - ��������� ������ ����� ��� ����� � ��������� �������� ����������� ������ �� ������ �����.
//
//  ����������:
//      1. ���� � ����������� ������ ����� �������, ��� ������������ ������� �������������
//         �������� (PullUp = On), �� ������������ Pin_In � Pin_Out ��������� ��������,
//         ����������� ������������ Pin_PullUp_IO.
//      2. � ������� ������� ������������, ����������� ����������� ��������, �� ��������� ���
//         ��� ���������� �� ����� ����������� define, � ����� ��� � �������.
//      3. � ����������� define ��������� ��� ��������� ����� �� ���������, �� ������� �������� ������.
//
//  �������:
//      #define Example1    B, 5, Low,
//      #define Example2    B, 5,,
//      #define Example3    B, 5, Low, On
//      #define Example4    B, 5,, Off
//
//      Pin_Out(Example1);	- ��������� ������ 5 ����� � �� �����;
//      Pin_On(Example1);		- ��������� ���������� ��������� ������ (Low) �� ������ 5 ����� B.
//
//  ������ � ���������� 2:
//      ������������ Set_Pin_On(B, 5, Low, On) �������� ����������� ��������, ��� � ������� Pin_On(Example3)
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// �������� �������� � �������
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

// ��������� ������ ����� � ������ ��������� (Hi-Z)
#define Set_Pin_HiZ(Ltr, Idx, Level, PullUp)         { Clr_DDR_Bit(Ltr, Idx); Clr_PORT_Bit(Ltr, Idx); }
#define Pin_HiZ(PortDef)                             Set_Pin_HiZ(PortDef)

// ��������� ������ ����� ��� ������ � ������� ������������� ����������
#define SetPinPullUp(Ltr, Idx)
#define SetPinPullUpOff(Ltr, Idx)
#define SetPinPullUpOn(Ltr, Idx)                     Clr_PORT_Bit(Ltr, Idx)
#define Set_Pin_PullUp_IO(Ltr, Idx, Level, PullUp)   { Clr_DDR_Bit(Ltr, Idx); SetPinPullUp##PullUp(Ltr, Idx); }
#define Pin_PullUp_IO(PortDef)                       Set_Pin_PullUp_IO(PortDef)

// ��������� ������ ����� ��� ����
#define Set_Pin_In(Ltr, Idx, Level, PullUp)          Set_Pin_PullUp_IO(Ltr, Idx, Level, PullUp)
#define Pin_In(PortDef)                              Set_Pin_In(PortDef)

// ��������� ������ ����� ��� �����
#define SetPinOut(Ltr, Idx)                          Set_DDR_Bit(Ltr, Idx)
#define SetPinOutOff(Ltr, Idx)                       Set_DDR_Bit(Ltr, Idx)
#define SetPinOutOn(Ltr, Idx)                        Clr_DDR_Bit(Ltr, Idx)
#define Set_Pin_Out(Ltr, Idx, Level, PullUp)         { SetPinOut##PullUp(Ltr, Idx); SetPinPullUp##PullUp(Ltr, Idx); }
#define Pin_Out(PortDef)	                           Set_Pin_Out(PortDef)

// ��������� ��������� ����������� ������ �� ������ �����
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

// ��������� ������ ����� ��� ����� � ��������� ��������� ����������� ������
#define Set_Pin_Out_On(Ltr, Idx, Level, PullUp)      { Set_Pin_Out(Ltr, Idx, Level, PullUp); Set_Pin_On(Ltr, Idx, Level, PullUp); }
#define Pin_Out_On(PortDef)                          Set_Pin_Out_On(PortDef)

// ��������� ���������� ����������� ������ �� ������ �����
#define Set_Pin_High(Ltr, Idx, Level, PullUp)        Set_Pin_On(Ltr, Idx, High, PullUp)
#define Pin_High(PortDef)                            Set_Pin_High(PortDef)

// ��������� ������ ����� ��� ����� � ��������� ���������� ����������� ������ �� ������ �����
#define Set_Pin_Out_High(Ltr, Idx, Level, PullUp)    { Set_Pin_Out(Ltr, Idx, Level, PullUp); Set_Pin_High(Ltr, Idx, Level, PullUp); }
#define Pin_Out_High(PortDef)                        Set_Pin_Out_High(PortDef)

// ��������� ����������� ����������� ������ �� ������ �����
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

// ��������� ����������� ����������� ������ �� ������ �����
#define Set_Pin_Out_Off(Ltr, Idx, Level, PullUp)     { Set_Pin_Out(Ltr, Idx, Level, PullUp); Set_Pin_Off(Ltr, Idx, Level, PullUp); }
#define Pin_Out_Off(PortDef)                         Set_Pin_Out_Off(PortDef)

// ��������� �������� ����������� ������ �� ������ �����
#define Set_Pin_Low(Ltr, Idx, Level, PullUp)         Set_Pin_Off(Ltr, Idx, High, PullUp)
#define Pin_Low(PortDef)                             Set_Pin_Low(PortDef)

// ��������� ������ ����� ��� ����� � ��������� �������� ����������� ������ �� ������ �����
#define Set_Pin_Out_Low(Ltr, Idx, Level, PullUp)     { Set_Pin_Out(Ltr, Idx, Level, PullUp); Set_Pin_Low(Ltr, Idx, Level, PullUp); }
#define Pin_Out_Low(PortDef)                         Set_Pin_Out_Low(PortDef)

// �������� ����������� ������ �� ������ �����
#define GetPinSignal(Ltr, Idx)	                     Get_PIN_State(Ltr, Idx)
#define GetPinSignalLow(Ltr, Idx)	                   (!Get_PIN_State(Ltr, Idx))
#define GetPinSignalHigh(Ltr, Idx)	                 Get_PIN_State(Ltr, Idx)
#define Get_Pin_Signal(Ltr, Idx, Level, PullUp)      GetPinSignal##Level(Ltr, Idx)
#define Get_Pin_State(Ltr, Idx, Level, PullUp)       GetPinSignal##Level(Ltr, Idx)
#define Pin_Signal(PortDef)                          Get_Pin_Signal(PortDef)
#define Pin_State(PortDef)                           Get_Pin_Signal(PortDef)
#define Test_Pin(PortDef)                            Get_Pin_Signal(PortDef)

// ��������� ����������� ������ �� ������ ����� �� ���������������
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

// ���������������� ���� � ���� ����������
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
