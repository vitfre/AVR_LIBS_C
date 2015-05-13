/*
 * eeprom.h
 *
 * Created: 04.12.2013 11:32:25
 *  Author: Evgeny
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);

void SaveAlarmParam(uint8_t Num);
void ReadAlarmParam(uint8_t Num);

#endif /* EEPROM_H_ */