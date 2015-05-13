
;CodeVisionAVR C Compiler V2.05.3 Standard
;(C) Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATtiny13A
;Program type             : Application
;Clock frequency          : 1.000000 MHz
;Memory model             : Tiny
;Optimize for             : Size
;(s)printf features       : int, width
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 16 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : Yes
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;Global 'const' stored in FLASH     : No
;Enhanced function parameter passing: Yes
;Smart register allocation          : Off
;Automatic register allocation      : On

	#pragma AVRPART ADMIN PART_NAME ATtiny13A
	#pragma AVRPART MEMORY PROG_FLASH 1024
	#pragma AVRPART MEMORY EEPROM 64
	#pragma AVRPART MEMORY INT_SRAM SIZE 159
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
	.EQU __SRAM_END=0x009F
	.EQU __DSTACK_SIZE=0x0010
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

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
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
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOV  R30,R0
	MOV  R31,R1
	.ENDM

	.MACRO __GETB2SX
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOV  R26,R0
	MOV  R27,R1
	.ENDM

	.MACRO __GETBRSX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOV  R26,R28
	MOV  R27,R29
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOV  R30,R28
	MOV  R31,R29
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOV  R26,R28
	MOV  R27,R29
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
	MOV  R26,R28
	MOV  R27,R29
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
	MOV  R26,R28
	MOV  R27,R29
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
	.DEF _volt=R4
	.DEF _i=R6

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

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
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

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)

	RJMP _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x70

	.CSEG
;#include <tiny13a.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x18
	.EQU __sm_adc_noise_red=0x08
	.EQU __sm_powerdown=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;#include <ht1621.h>

	.CSEG
_data:
	ST   -Y,R26
;	b -> Y+0
	RCALL SUBOPT_0x0
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x3
	CBI  0x18,1
	SBI  0x18,2
	RJMP _0x6F
_0x3:
	CBI  0x18,1
	CBI  0x18,2
_0x6F:
	SBI  0x18,1
	RCALL SUBOPT_0x0
	RJMP _0x2000001
_cs:
	ST   -Y,R26
;	b -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x11
	SBI  0x18,0
	RJMP _0x14
_0x11:
	CBI  0x18,0
_0x14:
	RJMP _0x2000001
_out_cmd:
	RCALL SUBOPT_0x1
;	cmd -> Y+1
;	maska -> R16
	RCALL SUBOPT_0x2
_0x18:
	RCALL SUBOPT_0x3
	LDD  R26,Y+1
	AND  R30,R26
	BREQ _0x1C
	SBI  0x18,2
	RJMP _0x1F
_0x1C:
	CBI  0x18,2
_0x1F:
	RCALL SUBOPT_0x4
	CPI  R16,0
	BRNE _0x18
	CBI  0x18,1
	CBI  0x18,2
	SBI  0x18,1
	LDI  R26,LOW(1)
	RCALL _cs
	LDD  R16,Y+0
	ADIW R28,2
	RET
_out_data:
	RCALL SUBOPT_0x1
;	temp1 -> Y+4
;	temp2 -> Y+3
;	temp3 -> Y+2
;	addr -> Y+1
;	maska -> R16
	RCALL SUBOPT_0x5
	LDD  R26,Y+1
	CPI  R26,LOW(0x1)
	BRNE _0x2A
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	LDI  R26,LOW(0)
	RJMP _0x70
_0x2A:
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x5
	LDI  R26,LOW(1)
_0x70:
	RCALL _data
	RCALL SUBOPT_0x5
_0x2D:
	RCALL SUBOPT_0x3
	LDD  R26,Y+4
	AND  R30,R26
	BREQ _0x31
	SBI  0x18,2
	RJMP _0x34
_0x31:
	CBI  0x18,2
_0x34:
	RCALL SUBOPT_0x4
	CPI  R16,0
	BRNE _0x2D
	LDI  R16,LOW(128)
_0x3A:
	RCALL SUBOPT_0x3
	LDD  R26,Y+3
	AND  R30,R26
	BREQ _0x3E
	SBI  0x18,2
	RJMP _0x41
_0x3E:
	CBI  0x18,2
_0x41:
	RCALL SUBOPT_0x4
	CPI  R16,0
	BRNE _0x3A
	LDI  R16,LOW(128)
_0x47:
	RCALL SUBOPT_0x3
	LDD  R26,Y+2
	AND  R30,R26
	BREQ _0x4B
	SBI  0x18,2
	RJMP _0x4E
_0x4B:
	CBI  0x18,2
_0x4E:
	RCALL SUBOPT_0x4
	CPI  R16,0
	BRNE _0x47
	LDD  R26,Y+1
	CPI  R26,LOW(0x1)
	BRNE _0x53
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x2
	LDI  R26,LOW(0)
	RJMP _0x71
_0x53:
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x5
	LDI  R26,LOW(1)
_0x71:
	RCALL _data
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x2
	LDI  R26,LOW(1)
	RCALL _cs
	LDD  R16,Y+0
	ADIW R28,5
	RET
;#define ADC_VREF_TYPE 0x00
;
;unsigned int volt_[6], volt;
;uchar i;
;
;// Read the AD conversion result
;unsigned int read_adc(unsigned char adc_input)
; 0000 000A {
_read_adc:
; 0000 000B ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
	ST   -Y,R26
;	adc_input -> Y+0
	LD   R30,Y
	OUT  0x7,R30
; 0000 000C // Delay needed for the stabilization of the ADC input voltage
; 0000 000D delay_us(10);
	RCALL SUBOPT_0x0
; 0000 000E // Start the AD conversion
; 0000 000F ADCSRA|=0x40;
	SBI  0x6,6
; 0000 0010 // Wait for the AD conversion to complete
; 0000 0011 while ((ADCSRA & 0x10)==0);
_0x55:
	SBIS 0x6,4
	RJMP _0x55
; 0000 0012 ADCSRA|=0x10;
	SBI  0x6,4
; 0000 0013 return ADCW;
	IN   R30,0x4
	IN   R31,0x4+1
_0x2000001:
	ADIW R28,1
	RET
; 0000 0014 }
;
;
;void main(void)
; 0000 0018 {
_main:
; 0000 0019 #pragma optsize-
; 0000 001A CLKPR=0x80;
	LDI  R30,LOW(128)
	OUT  0x26,R30
; 0000 001B CLKPR=0x00;
	LDI  R30,LOW(0)
	OUT  0x26,R30
; 0000 001C #ifdef _OPTIMIZE_SIZE_
; 0000 001D #pragma optsize+
; 0000 001E #endif
; 0000 001F 
; 0000 0020 PORTB=0x00;
	OUT  0x18,R30
; 0000 0021 DDRB.0=1;
	SBI  0x17,0
; 0000 0022 DDRB.1=1;
	SBI  0x17,1
; 0000 0023 DDRB.2=1;
	SBI  0x17,2
; 0000 0024 
; 0000 0025 
; 0000 0026 // ADC initialization
; 0000 0027 // ADC Clock frequency: 125.000 kHz
; 0000 0028 // ADC Bandgap Voltage Reference: Off
; 0000 0029 // ADC Auto Trigger Source: Free Running
; 0000 002A // Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
; 0000 002B DIDR0&=0x03;
	IN   R30,0x14
	ANDI R30,LOW(0x3)
	OUT  0x14,R30
; 0000 002C DIDR0|=0x00;
	IN   R30,0x14
	OUT  0x14,R30
; 0000 002D ADMUX=ADC_VREF_TYPE & 0xff;
	LDI  R30,LOW(0)
	OUT  0x7,R30
; 0000 002E ADCSRA=0xA3;
	LDI  R30,LOW(163)
	OUT  0x6,R30
; 0000 002F ADCSRB&=0xF8;
	IN   R30,0x3
	ANDI R30,LOW(0xF8)
	OUT  0x3,R30
; 0000 0030 
; 0000 0031 
; 0000 0032 cs(1); //pottiagivaem na plius
	LDI  R26,LOW(1)
	RCALL _cs
; 0000 0033 
; 0000 0034  out_cmd(0b00101000); //BIAS & COM
	LDI  R26,LOW(40)
	RCALL _out_cmd
; 0000 0035  out_cmd(0b00000001); //SYSTEM ENABLE
	LDI  R26,LOW(1)
	RCALL _out_cmd
; 0000 0036  out_cmd(0b00000011);    //TURN ON LCD
	LDI  R26,LOW(3)
	RCALL _out_cmd
; 0000 0037  //out_cmd(0b10100000);
; 0000 0038 /*==================================================================================*/
; 0000 0039 /*
; 0000 003A //Send Data
; 0000 003B cs(0);  //set CS LOW
; 0000 003C data(1);data(0);data(1);   //send command WRITE = "101"
; 0000 003D data(0);data(0);data(0);data(0);data(0);data(1);     data(1);data(1);data(1);data(1);  delay_ms(200);
; 0000 003E cs(1); //set CS HIGH
; 0000 003F */
; 0000 0040 
; 0000 0041 
; 0000 0042 while (1)
_0x5E:
; 0000 0043       {
; 0000 0044 
; 0000 0045 read_adc(2);
	LDI  R26,LOW(2)
	RCALL SUBOPT_0x6
; 0000 0046 //vmesto plavaiushei tochki uvelichivaiu vse na 10, toest vmnesto 5.0 pishu 50
; 0000 0047        volt = ADCW;//  >>2;
; 0000 0048        volt_[0] = (volt*5)/1023;
	LDI  R26,LOW(5)
	LDI  R27,HIGH(5)
	RCALL SUBOPT_0x7
	STS  _volt_,R30
	STS  _volt_+1,R31
; 0000 0049        volt_[1] = ((volt*50)/1023) %10;
	RCALL SUBOPT_0x8
	LDI  R26,LOW(50)
	LDI  R27,HIGH(50)
	RCALL SUBOPT_0x7
	RCALL SUBOPT_0x9
	__PUTW1MN _volt_,2
; 0000 004A        volt_[2] = ((volt*500)/1023) %10;
	RCALL SUBOPT_0x8
	LDI  R26,LOW(500)
	LDI  R27,HIGH(500)
	RCALL SUBOPT_0x7
	RCALL SUBOPT_0x9
	__PUTW1MN _volt_,4
; 0000 004B 
; 0000 004C read_adc(3);
	LDI  R26,LOW(3)
	RCALL SUBOPT_0x6
; 0000 004D        volt = ADCW;//  >>2;
; 0000 004E        volt_[3] = ((volt*500)/1023)*16/10;
	LDI  R26,LOW(500)
	LDI  R27,HIGH(500)
	RCALL SUBOPT_0x7
	RCALL SUBOPT_0xA
	__PUTW1MN _volt_,6
; 0000 004F        volt_[4] = (((volt*5000)/1023) %10)*16/10;
	RCALL SUBOPT_0x8
	LDI  R26,LOW(5000)
	LDI  R27,HIGH(5000)
	RCALL SUBOPT_0x7
	RCALL SUBOPT_0x9
	RCALL SUBOPT_0xA
	__PUTW1MN _volt_,8
; 0000 0050        volt_[5] = (((volt*50000)/1023) %10)*16/10;
	RCALL SUBOPT_0x8
	LDI  R26,LOW(50000)
	LDI  R27,HIGH(50000)
	RCALL SUBOPT_0x7
	RCALL SUBOPT_0x9
	RCALL SUBOPT_0xA
	__PUTW1MN _volt_,10
; 0000 0051 
; 0000 0052 
; 0000 0053 /*
; 0000 0054 if (volt_[3] > 3) {
; 0000 0055 
; 0000 0056      out_cmd(0b00001001);  //TONE ON
; 0000 0057      delay_ms(1000);
; 0000 0058      out_cmd(0b00001000);} //TONE OFF
; 0000 0059      delay_ms(10000);
; 0000 005A */
; 0000 005B 
; 0000 005C i=0;
	CLR  R6
; 0000 005D while (i != 6) {
_0x61:
	LDI  R30,LOW(6)
	CP   R30,R6
	BRNE PC+2
	RJMP _0x63
; 0000 005E        if (volt_[i] == 6) {volt_[i] = 0b11111010;}
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x6)
	LDI  R26,HIGH(0x6)
	CPC  R31,R26
	BRNE _0x64
	RCALL SUBOPT_0xB
	LDI  R30,LOW(250)
	LDI  R31,HIGH(250)
	RCALL SUBOPT_0xC
; 0000 005F        if (volt_[i] == 1) {volt_[i] = 0b00000110;}
_0x64:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x65
	RCALL SUBOPT_0xB
	LDI  R30,LOW(6)
	LDI  R31,HIGH(6)
	RCALL SUBOPT_0xC
; 0000 0060        if (volt_[i] == 2) {volt_[i] = 0b01111100;}
_0x65:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x66
	RCALL SUBOPT_0xB
	LDI  R30,LOW(124)
	LDI  R31,HIGH(124)
	RCALL SUBOPT_0xC
; 0000 0061        if (volt_[i] == 3) {volt_[i] = 0b01011110;}
_0x66:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x3)
	LDI  R26,HIGH(0x3)
	CPC  R31,R26
	BRNE _0x67
	RCALL SUBOPT_0xB
	LDI  R30,LOW(94)
	LDI  R31,HIGH(94)
	RCALL SUBOPT_0xC
; 0000 0062        if (volt_[i] == 4) {volt_[i] = 0b11000110;}
_0x67:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x4)
	LDI  R26,HIGH(0x4)
	CPC  R31,R26
	BRNE _0x68
	RCALL SUBOPT_0xB
	LDI  R30,LOW(198)
	LDI  R31,HIGH(198)
	RCALL SUBOPT_0xC
; 0000 0063        if (volt_[i] == 5) {volt_[i] = 0b11011010;}
_0x68:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x5)
	LDI  R26,HIGH(0x5)
	CPC  R31,R26
	BRNE _0x69
	RCALL SUBOPT_0xB
	LDI  R30,LOW(218)
	LDI  R31,HIGH(218)
	RCALL SUBOPT_0xC
; 0000 0064        if (volt_[i] == 7) {volt_[i] = 0b00001110;}
_0x69:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x7)
	LDI  R26,HIGH(0x7)
	CPC  R31,R26
	BRNE _0x6A
	RCALL SUBOPT_0xB
	LDI  R30,LOW(14)
	LDI  R31,HIGH(14)
	RCALL SUBOPT_0xC
; 0000 0065        if (volt_[i] == 8) {volt_[i] = 0b11111110;}
_0x6A:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x8)
	LDI  R26,HIGH(0x8)
	CPC  R31,R26
	BRNE _0x6B
	RCALL SUBOPT_0xB
	LDI  R30,LOW(254)
	LDI  R31,HIGH(254)
	RCALL SUBOPT_0xC
; 0000 0066        if (volt_[i] == 9) {volt_[i] = 0b11011110;}
_0x6B:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	CPI  R30,LOW(0x9)
	LDI  R26,HIGH(0x9)
	CPC  R31,R26
	BRNE _0x6C
	RCALL SUBOPT_0xB
	LDI  R30,LOW(222)
	LDI  R31,HIGH(222)
	RCALL SUBOPT_0xC
; 0000 0067        if (volt_[i] == 0) {volt_[i] = 0b10111110;}
_0x6C:
	RCALL SUBOPT_0xB
	RCALL __GETW1P
	SBIW R30,0
	BRNE _0x6D
	RCALL SUBOPT_0xB
	LDI  R30,LOW(190)
	LDI  R31,HIGH(190)
	RCALL SUBOPT_0xC
; 0000 0068 i++;
_0x6D:
	INC  R6
; 0000 0069 
; 0000 006A     };
	RJMP _0x61
_0x63:
; 0000 006B 
; 0000 006C 
; 0000 006D 
; 0000 006E       delay_ms(500);
	LDI  R26,LOW(500)
	LDI  R27,HIGH(500)
	RCALL _delay_ms
; 0000 006F        out_data(volt_[0], volt_[1] | 1, volt_[2], 1);
	LDS  R30,_volt_
	ST   -Y,R30
	__GETB1MN _volt_,2
	ORI  R30,1
	ST   -Y,R30
	__GETB1MN _volt_,4
	ST   -Y,R30
	LDI  R26,LOW(1)
	RCALL _out_data
; 0000 0070        out_data(volt_[3] | 1, volt_[4], volt_[5], 2);
	__GETB1MN _volt_,6
	ORI  R30,1
	ST   -Y,R30
	__GETB1MN _volt_,8
	ST   -Y,R30
	__GETB1MN _volt_,10
	ST   -Y,R30
	LDI  R26,LOW(2)
	RCALL _out_data
; 0000 0071        }
	RJMP _0x5E
; 0000 0072 }
_0x6E:
	RJMP _0x6E
;
;
;/*
;data(0);data(0);data(0);data(0);     data(0);data(1);data(1);data(0);       //1   00000110     invert - 01100000
;data(0);data(1);data(1);data(1);     data(1);data(1);data(0);data(0);       //2   01111100              00111110
;data(0);data(1);data(0);data(1);     data(1);data(1);data(1);data(0);       //3   01011110              01111010
;data(1);data(1);data(0);data(0);     data(0);data(1);data(1);data(0);       //4   11000110              01100011
;data(1);data(1);data(0);data(1);     data(1);data(0);data(1);data(0);       //5   11011010              01011011
;data(1);data(1);data(1);data(1);     data(1);data(0);data(1);data(0);       //6   11111010              01011111
;data(0);data(0);data(0);data(0);     data(1);data(1);data(1);data(0);       //7   00001110              01110000
;data(1);data(1);data(1);data(1);     data(1);data(1);data(1);data(0);       //8   11111110              01111111
;data(1);data(1);data(0);data(1);     data(1);data(1);data(1);data(0);       //9   11011110              01111011
;data(1);data(0);data(1);data(1);     data(1);data(1);data(1);data(0);       //0   10111110

	.DSEG
_volt_:
	.BYTE 0xC

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x0:
	__DELAY_USB 3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x1:
	ST   -Y,R26
	ST   -Y,R16
	LDI  R16,128
	LDI  R26,LOW(0)
	RCALL _cs
	LDI  R26,LOW(1)
	RCALL _data
	LDI  R26,LOW(0)
	RJMP _data

;OPTIMIZER ADDED SUBROUTINE, CALLED 19 TIMES, CODE SIZE REDUCTION:16 WORDS
SUBOPT_0x2:
	LDI  R26,LOW(0)
	RJMP _data

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3:
	CBI  0x18,1
	MOV  R30,R16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x4:
	LSR  R16
	SBI  0x18,1
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x5:
	LDI  R26,LOW(1)
	RJMP _data

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x6:
	RCALL _read_adc
	__INWR 4,5,4
	__GETW1R 4,5
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:23 WORDS
SUBOPT_0x7:
	RCALL __MULW12U
	MOV  R26,R30
	MOV  R27,R31
	LDI  R30,LOW(1023)
	LDI  R31,HIGH(1023)
	RCALL __DIVW21U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x8:
	__GETW1R 4,5
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x9:
	MOV  R26,R30
	MOV  R27,R31
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RCALL __MODW21U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0xA:
	RCALL __LSLW4
	MOV  R26,R30
	MOV  R27,R31
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RCALL __DIVW21U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 20 TIMES, CODE SIZE REDUCTION:55 WORDS
SUBOPT_0xB:
	MOV  R30,R6
	LDI  R26,LOW(_volt_)
	LSL  R30
	ADD  R26,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xC:
	ST   X+,R30
	ST   X,R31
	RET


	.CSEG
_delay_ms:
	adiw r26,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0xFA
	wdr
	sbiw r26,1
	brne __delay_ms0
__delay_ms1:
	ret

__LSLW4:
	LSL  R30
	ROL  R31
__LSLW3:
	LSL  R30
	ROL  R31
__LSLW2:
	LSL  R30
	ROL  R31
	LSL  R30
	ROL  R31
	RET

__MULW12U:
	MOV  R0,R26
	MOV  R1,R27
	LDI  R24,17
	CLR  R26
	SUB  R27,R27
	RJMP __MULW12U1
__MULW12U3:
	BRCC __MULW12U2
	ADD  R26,R0
	ADC  R27,R1
__MULW12U2:
	LSR  R27
	ROR  R26
__MULW12U1:
	ROR  R31
	ROR  R30
	DEC  R24
	BRNE __MULW12U3
	RET

__DIVW21U:
	CLR  R0
	CLR  R1
	LDI  R25,16
__DIVW21U1:
	LSL  R26
	ROL  R27
	ROL  R0
	ROL  R1
	SUB  R0,R30
	SBC  R1,R31
	BRCC __DIVW21U2
	ADD  R0,R30
	ADC  R1,R31
	RJMP __DIVW21U3
__DIVW21U2:
	SBR  R26,1
__DIVW21U3:
	DEC  R25
	BRNE __DIVW21U1
	MOV  R30,R26
	MOV  R31,R27
	MOV  R26,R0
	MOV  R27,R1
	RET

__MODW21U:
	RCALL __DIVW21U
	MOV  R30,R26
	MOV  R31,R27
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	DEC  R26
	RET

;END OF CODE MARKER
__END_OF_CODE:
