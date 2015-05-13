/*
 * buart.h
 *
 * Created: 03.11.2013 21:32:40
 *  Author: User
 */ 

#ifndef BUART_H_
#define BUART_H_
//---------------------------------------------------------------------------------------
#define F_CPU 16000000UL // or whatever may be your frequency
//---------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
//---------------------------------------------------------------------------------------
#define UART_RATE		9600
#define UART_BUFSIZE	64
#define UART_BUFEND		(UART_BUFSIZE-1)
//---------------------------------------------------------------------------------------
uint8_t uart_rxrd, uart_rxwr;
uint8_t uart_rx[UART_BUFSIZE];
uint8_t uart_txrd, uart_txwr;
uint8_t uart_tx[UART_BUFSIZE];
//---------------------------------------------------------------------------------------
void uart_init();
uint8_t uart_rx_count();
uint8_t uart_read();
void uart_write(uint8_t byte);
//---------------------------------------------------------------------------------------
//****************************************************************************************
//Send 14 bit data into USART
//****************************************************************************************
void  send_usart_14bit(unsigned int temp_usart_int);
#endif /* BUART_H_ */
