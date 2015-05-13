/*
 * menu.c
 *
 * Created: 08.11.2013 12:39:55
 *  Author: Администратор
 */ 

#include "menu.h"

//****************************************************************************************
void Main_menu (unsigned char Com,unsigned int data)
{
	unsigned char redirect=0;
	unsigned int data1=0;
	double ddd = data;
	//clear_lcd();						// Чистим дисплей перед виводом
	set_lcd_position(1,3);				// Переводимо курсор  
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
		detect_string(data1);			// Виводим на дисплей
		send_lcd_string(",");
		detect_string(ddd);				// Виводим на дисплей
		//==========================
		set_lcd_position(2,14);           // Переводимо курсор
		send_lcd_string("cm");          // Виводим строку на дисплей
		//==========================
		break;
		default :;
	};   //switch (redirect)
	
};   //Main_menu
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************