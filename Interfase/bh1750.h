/*
bh1750 lib 0x01

copyright (c) Davide Gironi, 2013

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#ifndef BH1750_H_
#define BH1750_H_
//---------------------------------------------------------------------------------------
/* define CPU frequency in Mhz here if not defined in Makefile */
#define F_CPU 16000000UL // or whatever may be your frequency
//---------------------------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//---------------------------------------------------------------------------------------
#include "i2c/i2c_1.h"
#include "i2c/i2c_2.h"
#include "i2c/i2c_3.h"
#include "i2c/i2c_4.h"
#include "i2c/i2c_5.h"
#include "i2c/i2c_6.h"
#include "i2c/i2c_7.h"
#include "i2c/i2c_8.h"
#include "i2c/i2c_9.h"
//---------------------------------------------------------------------------------------
#define BH1750_ADDR 0x46 //device address		//#define BH1750_ADDR (0x23<<1) //device address
//---------------------------------------------------------------------------------------
#define POWER_DOWN 0x00
#define POWER_UP 0x01
#define RESET 0x07
//resolution modes
#define BH1750_MODEH 0x10 //continuously h-resolution mode, 1lx resolution, 120ms
#define BH1750_MODEH2 0x11 //continuously h-resolution mode, 0.5lx resolution, 120ms
#define BH1750_MODEL 0x13 //continuously l-resolution mode, 4x resolution, 16ms
//one_time resolution modes
#define ONE_TIME_BH1750_MODEH 0x20
#define ONE_TIME_BH1750_MODEH2 0x21
#define ONE_TIME_BH1750_MODEL 0x23
//define active resolution mode
#define BH1750_MODE BH1750_MODEH
//****************************************************************************************
//functions
/*
 * init bh1750
 */
void bh1750_init();
//****************************************************************************************
/*
 * read lux value
 */
unsigned int bh1750_getlux(unsigned long int *data);
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************

#endif
