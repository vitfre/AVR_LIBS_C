//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: AVR
//
//  Compiler....: �����
//
//  Description.: ������� 7-�� ����������� 4-�� ���������� ���������� 
//
//  Data........: 15.04.10  ������
//  Data........: 8.03.13  ������� ��������� SPI � ������������ ���������     
//
//***************************************************************************
#ifndef INDICATOR_H
#define INDICATOR_H

#include "compilers.h"
#include "spi.h"

/********************** ��������� ***************************/

/*���������� ���� ����������*/
#define IND_AMOUNT_NUM 16

/*����������������, ���� �� ������������ ����������� �� SPI*/
#define IND_SPI_CONNECTION

/*����������������, ���� ������������ ����������� ���������*/
#define IND_SCAN_SEGMENT

/*�������� ������� �������� 0 ��� 1*/
#define IND_ACTIVE_LEVEL_SEG   0

/*�������� ������� ������� 0 ��� 1*/
#define IND_ACTIVE_LEVEL_DIG   1

/*���� ���������� �������� ����������*/
#define IND_PORT_SEG PORTC
#define IND_DDR_SEG DDRC

/*���� ��������� ������� ����������*/
#define IND_PORT_DIG PORTD
#define IND_DDR_DIG DDRD

/*� ������ �������� ���������� �������� � �������
���� ������������ ��� ��������,
������ ������� ����� ����������������*/
#define IND_SHIFT_REG_SEG  2
#define IND_SHIFT_REG_DIG1 1
#define IND_SHIFT_REG_DIG2 0

/*� ����� ������� ���������� �������*/
#define IND_NUM1 0
#define IND_NUM2 1
#define IND_NUM3 2
#define IND_NUM4 3
#define IND_NUM5 4
#define IND_NUM6 5
#define IND_NUM7 6
#define IND_NUM8 7

/*���� ������������ ��� ��������� ��������
����� ���������� ������ ������ �������� */
#define IND_NUM9  8
#define IND_NUM10 9
#define IND_NUM11 10
#define IND_NUM12 11
#define IND_NUM13 12
#define IND_NUM14 13
#define IND_NUM15 14
#define IND_NUM16 15

/*� ����� ������� ���������� ��������*/  
#define IND_A    0
#define IND_B    1
#define IND_C    2
#define IND_D    3
#define IND_E    4
#define IND_F    5
#define IND_G    6
#define IND_COM  7

/***********************************************************************************
*   Function name :   IND_Init
*   Returns :         ���       
*   Parameters :      ���    
*   Purpose :         ������������� ������ �����-������, ������� ������ ����������       
************************************************************************************/
void IND_Init(void);

/***********************************************************************************
*   Function name :   IND_Output
*   Returns :         ���       
*   Parameters :      value - ����� ��� ��������������, comma - ������� ����� �� ����������    
*   Purpose :         ����������� 16-�� ��������� �����, ���������� ��� � ����� ����������       
***********************************************************************************/
void IND_Output(uint16_t value, uint8_t comma);

/***********************************************************************************
*   Function name :   IND_OutputFormat
*   Returns :         ���       
*   Parameters :      value - ����� ��� ��������������, 
*                     comma - ������� ����� �� ���������� 
*                     position - �� ����� ������� �������� �����
*                     amount - ������� ���� �������� 
*   Purpose :         ����������� 16-�� ��������� �����, ���������� ��� � ����� ����������       
***********************************************************************************/
void IND_OutputFormat(uint16_t value, uint8_t comma, uint8_t position, uint8_t amount);

/***********************************************************************************
*   Function name :   IND_Update
*   Returns :         ���       
*   Parameters :      ���   
*   Purpose :         ������� �� ��������� ����� �� ������.
*                     ��� ������� ����� �������� �� ���������� �������
***********************************************************************************/
void IND_Update(void);

#endif //INDICATOR_H