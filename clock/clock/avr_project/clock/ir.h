/*
 * ir.h
 *
 * Created: 11.10.2014 18:24:05
 *  Author: Evgeny
 */ 


#ifndef IR_H_
#define IR_H_

#define CMD_B1	0x8030
#define CMD_B2	0x80B0
#define CMD_B3	0x8070
#define CMD_B4	0x8000
#define CMD_B5	0x80F0
#define CMD_ON	0x8058
#define CMD_OFF	0x80A0

extern void SendCommand(uint16_t cmd);
extern void init_ir();

#endif /* IR_H_ */