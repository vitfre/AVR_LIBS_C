//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: Mega
//
//  Compiler....: 
//
//  Description.: ”правление 7-ми сегментным индикатором по SPI
//
//  Data........: 2.10.12
//
//***************************************************************************
#include "compilers.h"
#include "spi.h"
#include "indicator.h"

uint8_t i = 0;
uint8_t count = 0;

int main( void )
{
  IND_Init();
  SPI_Init();

   while(1){
     if (i == 63) {
       i = 0;
        count++;
        IND_Output(count, 0);       
     }
      IND_Update();
      __delay_cycles(10000);
      i++;
   }
  return 0;
}
