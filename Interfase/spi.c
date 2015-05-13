/*
 * spi.c
 *
 * Created: 04.12.2013 21:27:06
 *  Author: Администратор
 */ 

#include "spi.h"

void spi_init(void)
{
	unsigned char x;
	// Port B initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=Out Func0=Out
	// State7=T State6=T State5=T State4=T State3=T State2=0 State1=0 State0=0
	DDRX_SPI|=0x07;

	// SPI initialization
	// SPI Type: Master
	// SPI Clock Rate: 125,000 kHz
	// SPI Clock Phase: Cycle Start
	// SPI Clock Polarity: Low
	// SPI Data Order: MSB First
	SPCR=0xD3;
	SPSR=0x00;

	// Clear the SPI interrupt flag
	x=SPSR;
	x=SPDR;
	x=x;
};
//****************************************************************************************
ISR(SPI_STC_vect)
{
	uint8_t byte;
	uint8_t rd = (spi_rxwr+1) & SPI_BUFEND;
	byte = SPDR;
	if(rd != spi_rxrd)
	{
		spi_rx[spi_rxwr] = byte;
		spi_rxwr = rd;
	};

	spi_txrd = (spi_txrd+1) & SPI_BUFEND;
	if (spi_txrd != spi_txwr)
	{
		SPDR = spi_tx[spi_txrd];
	} 
	else
	{
		
	};
};
//****************************************************************************************
uint8_t spi_rx_count()
{
	return (spi_rxwr-spi_rxrd) & SPI_BUFEND;
};
//****************************************************************************************
uint8_t spi_read()
{
	uint8_t rd = spi_rxrd;
	uint8_t byte;
	if(rd != spi_rxwr)
	{
		byte = spi_rx[rd];
		spi_rxrd = (rd+1) & SPI_BUFEND;
		return byte;
	};
	return 0;
};
//****************************************************************************************
void spi_write(uint8_t byte)
{
	spi_tx[spi_txwr] = byte;
	if (spi_txwr != spi_txrd)
	{
		
	} 
	else
	{
		SPDR=spi_tx[spi_txwr];
	};
	spi_txwr = (spi_txwr+1) & SPI_BUFEND;
};

