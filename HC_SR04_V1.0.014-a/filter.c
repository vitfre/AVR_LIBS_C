﻿/*
 * filter.c
 *
 * Created: 09.11.2013 8:36:38
 *  Author: Администратор
 */ 

#include "filter.h"

//***************************************************************************************
unsigned int samples[4] = {0};
//***************************************************************************************
unsigned int filter(unsigned int del) {
	signed char cnt = 0;
	unsigned long int sum = 0;
	//=================================================================
	for (cnt = 1; cnt <= 3; cnt++) 
	{
		samples[cnt - 1] = samples[cnt];
	};
	//=================================================================
	samples[3] = del;
	for (cnt = 0; cnt <= 3; cnt++) 
	{
		sum = sum + samples[cnt];
	};
	//=================================================================
	del = sum >> 2;
	return del;
};