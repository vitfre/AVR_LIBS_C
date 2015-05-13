/*
 * buart_0.c
 *
 * Created: 18.10.2013 12:43:18
 *  Author: �������������
 */

#include "buart_0.h"

//****************************************************************************************
ISR(USART0_RX_vect)
{
	uint8_t byte;
	uint8_t wr = (uart_0_rxwr+1) & UART_BUFEND_0_;
	byte = UDR0;
	if(wr != uart_0_rxrd)
	{
		uart_0_rx[uart_0_rxwr] = byte;
		uart_0_rxwr = wr;
	};
};
//****************************************************************************************
ISR(USART0_UDRE_vect)
{
	uint8_t rd = uart_0_txrd;
	if(rd != uart_0_txwr)
	{
		UDR0 = uart_0_tx[rd];
		uart_0_txrd = (rd+1) & UART_BUFEND_0_;
		return;
	};
	UCSR0B &= ~(1<<UDRIE);
};
//****************************************************************************************
uint8_t uart_0_rx_count()
{
	return (uart_0_rxwr-uart_0_rxrd) & UART_BUFEND_0_;
};
//****************************************************************************************
uint8_t uart_0_read()
{
	uint8_t rd = uart_0_rxrd;
	uint8_t byte;
	if(rd != uart_0_rxwr)
	{
		byte = uart_0_rx[rd];
		uart_0_rxrd = (rd+1) & UART_BUFEND_0_;
		return byte;
	};
	return 0;
};
//****************************************************************************************
void uart_0_write(uint8_t byte)
{
	uint8_t wr = (uart_0_txwr+1) & UART_BUFEND_0_;
	if(wr != uart_0_txrd)
	{
		uart_0_tx[uart_0_txwr] = byte;
		uart_0_txwr = wr;
		UCSR0B |= (1<<UDRIE);
	};
};
//****************************************************************************************
void uart_0_init()
{
	//UBRR0L = (F_CPU+UART_RATE/8)/(UART_RATE/16)-1;
	//UBRR0H = ((F_CPU+UART_RATE/8)/(UART_RATE/16)-1)>>8;
	UBRR0L = F_CPU/UART_BAUD_RATE_0_/16-1;
	UBRR0H = (F_CPU/UART_BAUD_RATE_0_/16-1)>>8;
	UCSR0B = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);
	//UCSR0C = (1<<UPM0)|(1<<UPM1);
};
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************