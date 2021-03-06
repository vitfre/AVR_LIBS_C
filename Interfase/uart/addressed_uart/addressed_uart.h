﻿/*
 * addressed_uart.h
 *
 * Created: 06.02.2015 17:48:25
 *  Author: Администратор
 */ 


#ifndef ADDRESSED_UART_H_
#define ADDRESSED_UART_H_
//---------------------------------------------------------------------------------------
#define F_CPU 16000000UL // or whatever may be your frequency
//---------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
//---------------------------------------------------------------------------------------
#include "comands.h"
//---------------------------------------------------------------------------------------
#define UART_BAUD_RATE	9600UL
#define UART_BUFSIZE	32UL
#define UART_BUFEND		(UART_BUFSIZE-1)
//---------------------------------------------------------------------------------------
#define _START_			0xF1
#define _STOP_			0xF2
#define _RESTART_		0xF3
//---------------------------------------------------------------------------------------
#define _FOUND_			0x01
#define _NOT_FOUND_		0x00
//---------------------------------------------------------------------------------------
#define _ADDRES_		24
//---------------------------------------------------------------------------------------
#define _READY_			0x00
#define _FOUND_START_	0x01
#define _FOUND_ADDRES_	0x02
#define _FOUND_RESTART_	0x03
#define _FOUND_STOP_	0x04
#define _WAIT_TX_		0x05
#define _WAIT_RX_		0x06
#define _WAIT_STOP_		0x07
#define _WAIT_RESTART_	0x08
#define _READY_TX_		0x09
//---------------------------------------------------------------------------------------
#define _FOUND_TX_		0x11
#define _FOUND_RX_		0x12
#define _FOUND_COMM_	0x21
//---------------------------------------------------------------------------------------
uint8_t  receive_state;
//---------------------------------------------------------------------------------------
//uint8_t packet_received;

uint8_t begin_u;
uint8_t length_u;
//---------------------------------------------------------------------------------------
uint8_t uart_rxrd, uart_rxwr;
uint8_t uart_rx[UART_BUFSIZE];
uint8_t uart_txrd, uart_txwr;
uint8_t uart_tx[UART_BUFSIZE];
//****************************************************************************************
uint8_t packet_is_received();
//****************************************************************************************
uint8_t receive_state_();
//****************************************************************************************
uint8_t uart_rx_count();
//****************************************************************************************
uint8_t uart_read();
//****************************************************************************************
void uart_write(uint8_t byte);
//****************************************************************************************
void uart_init();
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************
#endif /* ADDRESSED_UART_H_ */