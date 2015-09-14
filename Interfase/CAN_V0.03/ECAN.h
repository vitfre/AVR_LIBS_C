/*==============================================================================
 *            ---2014-2015---
 *         Catcatcat electronics
 * �������� ������
 * ���������� ��� ������ � ����� CAN ����� ��� PIC-������������ 18 �����
 *            2014 - ����
 * ������
 * V-0.01 �������� ������ ����������� ���������
 * V-0.02 �������� ������� ������������� CAN - ������� ����� ��������
 * ��������� �������� � ���������� 83,33 ������ 83.
 * ��������  �������� @ 64MHz
 *     33 333 - 33
 *     47 619 - 47
 *     50 000 - 50
 *     83 333 - 83
 *     95 238 - 95
 *    100 000 - 100
 *    125 000 - 125
 *    200 000 - 200
 *    250 000 - 250
 *    400 000 - 400
 *    500 000 - 500
 *   1000 000 - 1000
 *--------------------------------
 * V-0.03 - ���������� ������ ������, �������� ������ � ����� ������
 *==============================================================================
 */


//------------------------------------------------------------------------------
/*���������� ����������*/

/* ������ ��� �������� ��������� �� ������� ���
 * 0 - ����������� ������������� h
 * 1 - ����������� ������������� l
 * 2 - ����������� ������������� h (���� ������������ ��������������)
 * 3 - ����������� ������������� l
 * 4 - ������������ ������ (���� �������)
 * 5 - ������ 0
 * 6 - ������ 1
 * 7 - ������ 2
 * 8 - ������ 3
 * 9 - ������ 4
 * 10 -������ 5
 * 11 -������ 6
 * 12 -������ 7
 * 13 -������ 8
 *
 * ��� ���������������� �������� �������� ��� PIC18 � ������ ����� ��������� ������ CAN
 * // ECAN Mux bit
 * #pragma config CANMX = PORTB	 	// PORTB	ECAN TX and RX pins are located on RB2 and RB3, respectively
 *                                      // PORTC	ECAN TX and RX pins are located on RC6 and RC7, respectively
 *  TRISB = 0b00001100;	//
 *                ||
 *                ++---------- ��� CAN ������
 */

/*���������� ������� ����� ������������ � �������� ���������*/
unsigned char RXBUFER[14];  // ����� ������ ������
/*����� �������� ������*/
unsigned char TXBUID[5];    // ����� ID - ��� �������� �������������� ���������
unsigned char TXBUFER[9];   // ������ - ��� �������� ������

// �������� ������ ����������
struct 
{
    unsigned PSo:       1;  // ���� �������� ���������
    unsigned IndCAN:    1;  // ���� ��������� ���������� ���� CAN
	/* ... */
}FlagCAN;

unsigned char Speed;        // ��� ������� ���������� �������� ��������

//------------------------------------------------------------------------------
#ifndef _ECAN_H
#define _ECAN_H


//------------------------------------------------------------------------------
/*�����������*/
//------------------------------------------------------------------------------
#define TRUE    1
#define FALSE   0


//------------------------------------------------------------------------------
/*�������� ���������� �������*/
//------------------------------------------------------------------------------
/*��������� ������ CAN*/
//void InitECAN(void);                // ������������� ����������� CAN
void Init_CAN(unsigned int freq, unsigned int filter1, unsigned int filter2, unsigned int filter3, unsigned int filter4, unsigned int filter5, unsigned int filter6);

void autospeed (void);  /*������������� �� �������� ��������*/

/*����� ���������*/
void ECAN_Receive(void);            // ����� ������ (�������� � ������� ����������)

/*�������� �����������  ��������� �� ������� const - ������������� ���������*/
void CANS_Transmit_TXB0(const char *str);

/* ������� ���������*/
/* ���������
 * ID ������������ ��������� */
void CAN_EID(int EID, int SID);

/* ���������
 * ID ������������ ��������� */
void CAN_SID(int SID);

/* ���������
 * RTR - ���� �������������� �������
 * COU - ���������� ���� �������� */
void CAN_RTR_COU(char RTR, char COU);

/* ����������� ������� �������� ��������� �� ������� TXBUID � TXBUFER*/
/* �������� ���������
 * TXB - ����� ������ ��������  0-2
 * PRI - ����� ���������� ��������  0-3 */
void CAN_Transmit(char TXB, char PRI);



//------------------------------------------------------------------------------
#endif