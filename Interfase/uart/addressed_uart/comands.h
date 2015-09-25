/*
 * comands.h
 *
 * Created: 10.02.2015 12:30:11
 *  Author: Администратор
 */ 


#ifndef COMANDS_H_
#define COMANDS_H_
//---------------------------------------------------------------------------------------
#define _ON_LAMP_		0x31
#define _OF_LAMP_		0x32
//---------------------------------------------------------------------------------------
#define work			1UL
#define idle			0UL
unsigned char system_state;
//---------------------------------------------------------------------------------------
#define _COMMAND_LON_	(PORTC|=(1<<0));	system_state=work;
#define _COMMAND_LOF_	(PORTC&=~(1<<0));	system_state=idle;
//---------------------------------------------------------------------------------------

#endif /* COMANDS_H_ */