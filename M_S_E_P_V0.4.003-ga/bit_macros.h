/*
 * bit_macros.h
 *
 * Created: 30.11.2013 18:52:01
 *  Author: Администратор
 */ 


#ifndef BIT_MACROS_H_
#define BIT_MACROS_H_
//---------------------------------------------------------------------------------------
#define ClrBit(x,y)				(x&=~(1<<y))
#define SetBit(x,y)				(x|=(1<<y))
#define BitVal(x,y)				(((x)>>(y))&1)
#define BitFlp(x,y)				((x)^(1<<(y)))
//---------------------------------------------------------------------------------------
#endif /* BIT_MACROS_H_ */