/*
 * menu.c
 *
 * Created: 08.11.2013 12:39:55
 *  Author: �������������
 */ 

#include "menu.h"

//****************************************************************************************
void Main_menu (unsigned char Com,unsigned int data)
{
	unsigned char redirect=0;
	unsigned int data1=0;
	double ddd = data;
	//clear_lcd();						// ������ ������� ����� �������
	set_lcd_position(1,3);				// ���������� ������  
	switch (Com) 
	{
		case 0x00 : send_lcd_string("  Distanse  ");redirect=0x00;
		break;
		default:;
	}; //switch (Com)
	
	switch (redirect)
	{
		case 0x00 :
		//==========================
		set_lcd_position(2,4);
		ddd = (ddd*10000)/58;
		data1=ddd/0x2710;
		detect_string(data1);			// ������� �� �������
		send_lcd_string(",");
		detect_string(ddd);				// ������� �� �������
		//==========================
		set_lcd_position(2,14);           // ���������� ������
		send_lcd_string("cm");          // ������� ������ �� �������
		//==========================
		break;
		default :;
	};   //switch (redirect)
	
};   //Main_menu
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************