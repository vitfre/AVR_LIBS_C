/*
 * spi.h
 *
 * Created: 04.12.2013 21:27:18
 *  Author: Администратор
 */ 


#ifndef SPI_H_
#define SPI_H_
//---------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit_macros.h"
//---------------------------------------------------------------------------------------
#define MASK_SPI_IN 				(1<<_SS_)|(1<<_SCK_)|(1<<_SI_)
#define MASK_SPI_OUT 				(0<<_SO_)
//---------------------------------------------------------------------------------------
//порт, к которому подключены ряды клавиатуры
#define PORT_SPI				PORTB
#define PIN_SPI					PINB
#define DDRX_SPI				DDRB
//---------------------------------------------------------------------------------------
//Входы
#define _SS_ 					2UL
#define _SCK_ 					5UL
#define _SI_					3UL
//Выходы
#define _SO_ 					4UL
//---------------------------------------------------------------------------------------
#define SPI_BUFSIZE				16UL
#define SPI_BUFEND				(SPI_BUFSIZE-1)
//---------------------------------------------------------------------------------------
uint8_t spi_rxrd, spi_rxwr;
uint8_t spi_rx[SPI_BUFSIZE];
uint8_t spi_txrd, spi_txwr;
uint8_t spi_tx[SPI_BUFSIZE];
//---------------------------------------------------------------------------------------
void spi_init(void);
void spi_write(uint8_t byte);
#endif /* SPI_H_ */