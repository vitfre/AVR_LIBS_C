
;CodeVisionAVR C Compiler V2.05.0 Professional
;(C) Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATtiny25
;Program type             : Application
;Clock frequency          : 8,000000 MHz
;Memory model             : Tiny
;Optimize for             : Size
;(s)printf features       : int, width
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 32 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : Yes
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;global 'const' stored in FLASH: No
;Enhanced core instructions    : On
;Smart register allocation     : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATtiny25
	#pragma AVRPART MEMORY PROG_FLASH 2048
	#pragma AVRPART MEMORY EEPROM 128
	#pragma AVRPART MEMORY INT_SRAM SIZE 223
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

	.LISTMAC
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E

	.EQU WDTCR=0x21
	.EQU MCUSR=0x34
	.EQU MCUCR=0x35
	.EQU SPL=0x3D
	.EQU SREG=0x3F
	.EQU GPIOR0=0x11
	.EQU GPIOR1=0x12
	.EQU GPIOR2=0x13

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0060
	.EQU __SRAM_END=0x00DF
	.EQU __DSTACK_SIZE=0x0020
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	RCALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __GETD1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X+
	LD   R22,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	SUBI R26,-@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	SUBI R26,-@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	SUBI R26,-@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	SUBI R26,-@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	SUBI R26,-@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	SUBI R26,-@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOV  R26,R@0
	SUBI R26,-@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOV  R26,R@0
	SUBI R26,-@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOV  R26,R@0
	SUBI R26,-@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOV  R26,R@0
	SUBI R26,-@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOV  R26,R@0
	SUBI R26,-@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOV  R26,R@0
	SUBI R26,-@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	SUBI R26,-@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	SUBI R26,-@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	SUBI R26,-@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	SUBI R26,-@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	SUBI R26,-@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	SUBI R26,-@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _PWM0=R2
	.DEF _PWM=R4
	.DEF _fl=R6
	.DEF _fl1=R8
	.DEF _temp=R10

;GPIOR0-GPIOR2 INITIALIZATION VALUES
	.EQU __GPIOR0_INIT=0x00
	.EQU __GPIOR1_INIT=0x00
	.EQU __GPIOR2_INIT=0x00

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	RJMP __RESET
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00

_0x29:
	.DB  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
	.DB  0x0,0x0

__GLOBAL_INI_TBL:
	.DW  0x0A
	.DW  0x02
	.DW  _0x29*2

_0xFFFFFFFF:
	.DW  0

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	WDR
	IN   R26,MCUSR
	CBR  R26,8
	OUT  MCUSR,R26
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,__CLEAR_SRAM_SIZE
	LDI  R26,__SRAM_START
__CLEAR_SRAM:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;GPIOR0-GPIOR2 INITIALIZATION
	LDI  R30,__GPIOR0_INIT
	OUT  GPIOR0,R30
	;__GPIOR1_INIT = __GPIOR0_INIT
	OUT  GPIOR1,R30
	;__GPIOR2_INIT = __GPIOR0_INIT
	OUT  GPIOR2,R30

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)

	RJMP _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x80

	.CSEG
;#include <tiny25.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x18
	.EQU __sm_adc_noise_red=0x08
	.EQU __sm_powerdown=0x10
	.EQU __sm_standby=0x18
	.SET power_ctrl_reg=mcucr
	#endif
;
;#include <delay.h>
;
;
;//#define                                sel PINB.0
;#define                                pls PINB.3
;#define                                mns PINB.4
;
;#define speaker                  PORTB.0  //17 Пискун
;
;#define ADC_VREF_TYPE 0x00
;
;// Declare your global variables here
;
;
;unsigned int PWM0=0,PWM=0,fl=0,fl1=0,temp=0;
;eeprom unsigned char PWM_eeprom=100;
;
;
;
;// Read the AD conversion result
;unsigned int read_adc(unsigned char adc_input)
; 0000 0018 {

	.CSEG
; 0000 0019 ADMUX=(adc_input & 0x0f) | ADC_VREF_TYPE;
;	adc_input -> Y+0
; 0000 001A // Delay needed for the stabilization of the ADC input voltage
; 0000 001B delay_us(10);
; 0000 001C // Start the AD conversion
; 0000 001D ADCSRA|=0x40;
; 0000 001E // Wait for the AD conversion to complete
; 0000 001F while ((ADCSRA & 0x10)==0);
; 0000 0020 ADCSRA|=0x10;
; 0000 0021 return ADCW;
; 0000 0022 }
;
;
;//**********************************************************************
;void beep (unsigned char squeak)  //Робимо пік
; 0000 0027 {
_beep:
; 0000 0028 for (;squeak>0; squeak--)
;	squeak -> Y+0
_0x7:
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRLO _0x8
; 0000 0029 {
; 0000 002A for (temp=50; temp; temp--)
	LDI  R30,LOW(50)
	LDI  R31,HIGH(50)
	MOVW R10,R30
_0xA:
	MOV  R0,R10
	OR   R0,R11
	BREQ _0xB
; 0000 002B {
; 0000 002C speaker=!speaker;
	SBIS 0x18,0
	RJMP _0xC
	CBI  0x18,0
	RJMP _0xD
_0xC:
	SBI  0x18,0
_0xD:
; 0000 002D delay_us(300);
	__DELAY_USW 600
; 0000 002E }
	MOVW R30,R10
	SBIW R30,1
	MOVW R10,R30
	RJMP _0xA
_0xB:
; 0000 002F //delay_us(900);
; 0000 0030 }
	LD   R30,Y
	SUBI R30,LOW(1)
	ST   Y,R30
	RJMP _0x7
_0x8:
; 0000 0031 }
	ADIW R28,1
	RET
;
;
;
;
;void main(void)
; 0000 0037 {
_main:
; 0000 0038 // Declare your local variables here
; 0000 0039 
; 0000 003A // Crystal Oscillator division factor: 1
; 0000 003B #pragma optsize-
; 0000 003C CLKPR=0x80;
	LDI  R30,LOW(128)
	OUT  0x26,R30
; 0000 003D CLKPR=0x00;
	LDI  R30,LOW(0)
	OUT  0x26,R30
; 0000 003E #ifdef _OPTIMIZE_SIZE_
; 0000 003F #pragma optsize+
; 0000 0040 #endif
; 0000 0041 
; 0000 0042 // Input/Output Ports initialization
; 0000 0043 // Port B initialization
; 0000 0044 // Func5=In Func4=In Func3=In Func2=In Func1=Out Func0=Out
; 0000 0045 // State5=T State4=P State3=P State2=P State1=0 State0=0
; 0000 0046 PORTB=0x1C;
	LDI  R30,LOW(28)
	OUT  0x18,R30
; 0000 0047 DDRB=0x03;
	LDI  R30,LOW(3)
	OUT  0x17,R30
; 0000 0048 
; 0000 0049 // Timer/Counter 0 initialization
; 0000 004A // Clock source: System Clock
; 0000 004B // Clock value: 125,000 kHz
; 0000 004C // Mode: Fast PWM top=0xFF
; 0000 004D // OC0A output: Disconnected
; 0000 004E // OC0B output: Non-Inverted PWM
; 0000 004F TCCR0A=0x23;
	LDI  R30,LOW(35)
	OUT  0x2A,R30
; 0000 0050 TCCR0B=0x03;
	LDI  R30,LOW(3)
	OUT  0x33,R30
; 0000 0051 TCNT0=0x00;
	LDI  R30,LOW(0)
	OUT  0x32,R30
; 0000 0052 OCR0A=0x00;
	OUT  0x29,R30
; 0000 0053 OCR0B=0x00;
	OUT  0x28,R30
; 0000 0054 
; 0000 0055 // Timer/Counter 1 initialization
; 0000 0056 // Clock source: System Clock
; 0000 0057 // Clock value: Timer1 Stopped
; 0000 0058 // Mode: Normal top=0xFF
; 0000 0059 // OC1A output: Disconnected
; 0000 005A // OC1B output: Disconnected
; 0000 005B // Timer1 Overflow Interrupt: Off
; 0000 005C // Compare A Match Interrupt: Off
; 0000 005D // Compare B Match Interrupt: Off
; 0000 005E PLLCSR=0x00;
	OUT  0x27,R30
; 0000 005F 
; 0000 0060 TCCR1=0x00;
	OUT  0x30,R30
; 0000 0061 GTCCR=0x00;
	OUT  0x2C,R30
; 0000 0062 TCNT1=0x00;
	OUT  0x2F,R30
; 0000 0063 OCR1A=0x00;
	OUT  0x2E,R30
; 0000 0064 OCR1B=0x00;
	OUT  0x2B,R30
; 0000 0065 OCR1C=0x00;
	OUT  0x2D,R30
; 0000 0066 
; 0000 0067 // External Interrupt(s) initialization
; 0000 0068 // INT0: Off
; 0000 0069 // Interrupt on any change on pins PCINT0-5: Off
; 0000 006A GIMSK=0x00;
	OUT  0x3B,R30
; 0000 006B MCUCR=0x00;
	OUT  0x35,R30
; 0000 006C 
; 0000 006D // Timer(s)/Counter(s) Interrupt(s) initialization
; 0000 006E TIMSK=0x00;
	OUT  0x39,R30
; 0000 006F 
; 0000 0070 // Universal Serial Interface initialization
; 0000 0071 // Mode: Disabled
; 0000 0072 // Clock source: Register & Counter=no clk.
; 0000 0073 // USI Counter Overflow Interrupt: Off
; 0000 0074 USICR=0x00;
	OUT  0xD,R30
; 0000 0075 
; 0000 0076 // Analog Comparator initialization
; 0000 0077 // Analog Comparator: Off
; 0000 0078 ACSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x8,R30
; 0000 0079 ADCSRB=0x00;
	LDI  R30,LOW(0)
	OUT  0x3,R30
; 0000 007A DIDR0=0x00;
	OUT  0x14,R30
; 0000 007B 
; 0000 007C 
; 0000 007D // ADC initialization
; 0000 007E // ADC Clock frequency: 125,000 kHz
; 0000 007F // ADC Voltage Reference: AVCC pin
; 0000 0080 // ADC Bipolar Input Mode: Off
; 0000 0081 // ADC Reverse Input Polarity: Off
; 0000 0082 // ADC Auto Trigger Source: ADC Stopped
; 0000 0083 // Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
; 0000 0084 DIDR0&=0x03;
	IN   R30,0x14
	ANDI R30,LOW(0x3)
	OUT  0x14,R30
; 0000 0085 DIDR0|=0x00;
	IN   R30,0x14
	OUT  0x14,R30
; 0000 0086 ADMUX=ADC_VREF_TYPE & 0xff;
	LDI  R30,LOW(0)
	OUT  0x7,R30
; 0000 0087 ADCSRA=0x86;
	LDI  R30,LOW(134)
	OUT  0x6,R30
; 0000 0088 ADCSRB&=0x5F;
	IN   R30,0x3
	ANDI R30,LOW(0x5F)
	OUT  0x3,R30
; 0000 0089 
; 0000 008A 
; 0000 008B beep (15);
	RCALL SUBOPT_0x0
; 0000 008C PWM=PWM_eeprom;
	LDI  R26,LOW(_PWM_eeprom)
	LDI  R27,HIGH(_PWM_eeprom)
	RCALL __EEPROMRDB
	MOV  R4,R30
	CLR  R5
; 0000 008D 
; 0000 008E while (PWM0<PWM)
_0xE:
	__CPWRR 2,3,4,5
	BRSH _0x10
; 0000 008F {
; 0000 0090     PWM0++;
	RCALL SUBOPT_0x1
; 0000 0091     OCR0B=PWM0;
	OUT  0x28,R2
; 0000 0092     delay_ms(10);
	RCALL SUBOPT_0x2
; 0000 0093 }
	RJMP _0xE
_0x10:
; 0000 0094 beep (4);
	RCALL SUBOPT_0x3
; 0000 0095 
; 0000 0096 while (1)
_0x11:
; 0000 0097       {
; 0000 0098 //Опитування кнопок + і - і s
; 0000 0099 while (pls==0)
_0x14:
	SBIC 0x16,3
	RJMP _0x16
; 0000 009A {
; 0000 009B     PWM0++;  //якщо кнопка нажата то +1
	RCALL SUBOPT_0x1
; 0000 009C     if (pls==0&mns==0)
	RCALL SUBOPT_0x4
	BREQ _0x17
; 0000 009D     {
; 0000 009E         beep (15);
	RCALL SUBOPT_0x0
; 0000 009F         PWM=254;
	RCALL SUBOPT_0x5
	MOVW R4,R30
; 0000 00A0         while (PWM0<PWM)
_0x18:
	__CPWRR 2,3,4,5
	BRSH _0x1A
; 0000 00A1         {
; 0000 00A2             PWM0++;
	RCALL SUBOPT_0x1
; 0000 00A3             if (PWM0>254) {PWM0=254;}
	RCALL SUBOPT_0x5
	CP   R30,R2
	CPC  R31,R3
	BRSH _0x1B
	RCALL SUBOPT_0x5
	MOVW R2,R30
; 0000 00A4             OCR0B=PWM0;
_0x1B:
	RCALL SUBOPT_0x6
; 0000 00A5             delay_ms(40);
; 0000 00A6         }
	RJMP _0x18
_0x1A:
; 0000 00A7         beep (4);
	RCALL SUBOPT_0x3
; 0000 00A8     };
_0x17:
; 0000 00A9     if (PWM0>=254)
	RCALL SUBOPT_0x5
	CP   R2,R30
	CPC  R3,R31
	BRLO _0x1C
; 0000 00AA     {
; 0000 00AB         PWM0=254;
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x7
; 0000 00AC         beep (1);
; 0000 00AD         delay_ms(200);
; 0000 00AE     };
_0x1C:
; 0000 00AF     delay_ms(10);
	RCALL SUBOPT_0x2
; 0000 00B0     OCR0B=PWM0;
	OUT  0x28,R2
; 0000 00B1 };
	RJMP _0x14
_0x16:
; 0000 00B2 
; 0000 00B3 while (mns==0)
_0x1D:
	SBIC 0x16,4
	RJMP _0x1F
; 0000 00B4 {
; 0000 00B5 PWM0--;   //якщо кнопка нажата то -1
	MOVW R30,R2
	SBIW R30,1
	MOVW R2,R30
; 0000 00B6 if (pls==0&mns==0)
	RCALL SUBOPT_0x4
	BREQ _0x20
; 0000 00B7 {
; 0000 00B8     beep (15);
	RCALL SUBOPT_0x0
; 0000 00B9     PWM=1;
	RCALL SUBOPT_0x8
	MOVW R4,R30
; 0000 00BA     while (PWM0>PWM)
_0x21:
	__CPWRR 4,5,2,3
	BRSH _0x23
; 0000 00BB     {
; 0000 00BC         PWM0--;
	MOVW R30,R2
	SBIW R30,1
	MOVW R2,R30
; 0000 00BD         if (PWM<1)
	RCALL SUBOPT_0x8
	CP   R4,R30
	CPC  R5,R31
	BRSH _0x24
; 0000 00BE         {
; 0000 00BF             PWM=1;
	RCALL SUBOPT_0x8
	MOVW R4,R30
; 0000 00C0         }
; 0000 00C1         OCR0B=PWM0;
_0x24:
	RCALL SUBOPT_0x6
; 0000 00C2         delay_ms(40);
; 0000 00C3     }
	RJMP _0x21
_0x23:
; 0000 00C4     beep (4);
	RCALL SUBOPT_0x3
; 0000 00C5 }
; 0000 00C6 
; 0000 00C7 
; 0000 00C8 if (PWM0<1)
_0x20:
	RCALL SUBOPT_0x8
	CP   R2,R30
	CPC  R3,R31
	BRSH _0x25
; 0000 00C9 {
; 0000 00CA     PWM0=1;
	RCALL SUBOPT_0x8
	RCALL SUBOPT_0x7
; 0000 00CB     beep (1);
; 0000 00CC     delay_ms(200);
; 0000 00CD }
; 0000 00CE delay_ms(10);
_0x25:
	RCALL SUBOPT_0x2
; 0000 00CF OCR0B=PWM0;
	OUT  0x28,R2
; 0000 00D0 }
	RJMP _0x1D
_0x1F:
; 0000 00D1 
; 0000 00D2 
; 0000 00D3 if (PWM0==PWM_eeprom) {;}
	LDI  R26,LOW(_PWM_eeprom)
	LDI  R27,HIGH(_PWM_eeprom)
	RCALL __EEPROMRDB
	MOVW R26,R2
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	BREQ _0x27
; 0000 00D4 else  {PWM_eeprom=PWM0;}
	MOV  R30,R2
	LDI  R26,LOW(_PWM_eeprom)
	LDI  R27,HIGH(_PWM_eeprom)
	RCALL __EEPROMWRB
_0x27:
; 0000 00D5 
; 0000 00D6 }}
	RJMP _0x11
_0x28:
	RJMP _0x28

	.ESEG
_PWM_eeprom:
	.DB  0x64

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x0:
	LDI  R30,LOW(15)
	ST   -Y,R30
	RJMP _beep

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x1:
	MOVW R30,R2
	ADIW R30,1
	MOVW R2,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x2:
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	ST   -Y,R31
	ST   -Y,R30
	RJMP _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x3:
	LDI  R30,LOW(4)
	ST   -Y,R30
	RJMP _beep

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x4:
	LDI  R26,0
	SBIC 0x16,3
	LDI  R26,1
	LDI  R30,LOW(0)
	RCALL __EQB12
	MOV  R0,R30
	LDI  R26,0
	SBIC 0x16,4
	LDI  R26,1
	LDI  R30,LOW(0)
	RCALL __EQB12
	AND  R30,R0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x5:
	LDI  R30,LOW(254)
	LDI  R31,HIGH(254)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x6:
	OUT  0x28,R2
	LDI  R30,LOW(40)
	LDI  R31,HIGH(40)
	ST   -Y,R31
	ST   -Y,R30
	RJMP _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x7:
	MOVW R2,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	RCALL _beep
	LDI  R30,LOW(200)
	LDI  R31,HIGH(200)
	ST   -Y,R31
	ST   -Y,R30
	RJMP _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x8:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	RET


	.CSEG
_delay_ms:
	ld   r30,y+
	ld   r31,y+
	adiw r30,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0x7D0
	wdr
	sbiw r30,1
	brne __delay_ms0
__delay_ms1:
	ret

__EQB12:
	CP   R30,R26
	LDI  R30,1
	BREQ __EQB12T
	CLR  R30
__EQB12T:
	RET

__EEPROMRDB:
	SBIC EECR,EEWE
	RJMP __EEPROMRDB
	PUSH R31
	IN   R31,SREG
	CLI
	OUT  EEARL,R26
	SBI  EECR,EERE
	IN   R30,EEDR
	OUT  SREG,R31
	POP  R31
	RET

__EEPROMWRB:
	SBIS EECR,EEWE
	RJMP __EEPROMWRB1
	WDR
	RJMP __EEPROMWRB
__EEPROMWRB1:
	IN   R25,SREG
	CLI
	OUT  EEARL,R26
	SBI  EECR,EERE
	IN   R24,EEDR
	CP   R30,R24
	BREQ __EEPROMWRB0
	OUT  EEDR,R30
	SBI  EECR,EEMWE
	SBI  EECR,EEWE
__EEPROMWRB0:
	OUT  SREG,R25
	RET

;END OF CODE MARKER
__END_OF_CODE:
