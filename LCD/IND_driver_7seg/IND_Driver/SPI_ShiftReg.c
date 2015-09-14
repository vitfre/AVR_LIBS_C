//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: Mega
//
//  Compiler....: GCC
//
//  Description.: Управление 7-ми сегментным индикатором по SPI
//
//  Data........: 10.03.13
//
//***************************************************************************

#define F_CPU 80000000UL

#include "compilers.h"
#include "spi.h"
#include "indicator.h"

uint8_t i = 0;
uint16_t count = 0;

int main( void )
{
	IND_Init();
	
	i = 100;
	count = 4328;

	while(1){
		if (i == 100) {
			i = 0;
			IND_OutputFormat(count, 0, 10, 4);
			IND_OutputFormat(45, 0, 6, 3);
			count++;
		}
		IND_Update();
		__delay_cycles(10000);
		i++;
	}
	return 0;
}