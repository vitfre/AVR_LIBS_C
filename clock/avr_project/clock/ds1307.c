#include <avr/interrupt.h>
#include "ds1307.h"

#ifndef F_CPU
   #define F_CPU  8000000UL
#endif

/*������ ������ TWI ������*/
#define TWI_BUFFER_SIZE  8      

/****************************************************************************
  ��������� ���� TWI ������ 
****************************************************************************/

/*����� ��������� ���� */                    
#define TWI_START                  0x08  // ��������� START ������������ 
#define TWI_REP_START              0x10  // ��������� ��������� START ������������ 
#define TWI_ARB_LOST               0x38  // ��� ������� ��������� 

/*��������� ���� �������� �����������*/                
#define TWI_MTX_ADR_ACK            0x18  // ��� ������� ����� SLA+W � �������� �������������
#define TWI_MTX_ADR_NACK           0x20  // ��� ������� ���� SLA+W � �� �������� �������������
#define TWI_MTX_DATA_ACK           0x28  // ��� ������� ���� ������ � �������� �������������  
#define TWI_MTX_DATA_NACK          0x30  // ��� ������� ���� ������ � �� �������� �������������

/*��������� ���� �������� ���������*/ 
#define TWI_MRX_ADR_ACK            0x40  // ��� ������� ����� SLA+R � �������� ������������ 
#define TWI_MRX_ADR_NACK           0x48  // ��� ������� ����� SLA+R � �� �������� ������������� 
#define TWI_MRX_DATA_ACK           0x50  // ���� ������ ������ � �������� �������������  
#define TWI_MRX_DATA_NACK          0x58  // ��� ������ ���� ������ ��� �������������  

/*������ ��������� ����*/
#define TWI_NO_STATE               0xF8  // �������������� ���������; TWINT = �0�
#define TWI_BUS_ERROR              0x00  // ������ �� ���� ��-�� ����������� ��������� ����� ��� ����

/*���������������� ����*/
#define TWI_SUCCESS                0xff

/****************************************************************************
  ����������� ��������
****************************************************************************/
#define TWI_READ_BIT     0       // ������� R/W ���� � �������� ������
#define TWI_ADR_BITS     1       // ������� ������ � �������� ������
#define TRUE             1
#define FALSE            0
#define TWSR_MASK     0xfc  

#define DS1307_ADR 104

volatile static uint8_t twiBuf[TWI_BUFFER_SIZE];
volatile static uint8_t twiState = TWI_NO_STATE;      
volatile static uint8_t twiMsgSize;       

/*������������ ��� ��������� �������� ������ twi ������*/
static uint8_t pre[4] = {2, 8, 32, 128};

static uint8_t TWI_MasterInit(uint16_t fr)
{
  uint8_t i;
  uint16_t twbrValue;
  
  for(i = 0; i<4; i++){
    twbrValue = ((((F_CPU)/1000UL)/fr)-16)/pre[i];
    if ((twbrValue > 0)&& (twbrValue < 256)){
       TWBR = (uint8_t)twbrValue;
       TWSR = i;
       TWDR = 0xFF;
       TWCR = (1<<TWEN);
       return TWI_SUCCESS;
    }
  }
  return 0;  
}    
static uint8_t TWI_TransceiverBusy(void)
{
  return (TWCR & (1<<TWIE));                 
}

extern void init_rtc()
{
	TWI_MasterInit(100);
}

extern uint8_t UpdateTime(dstime_t* current_time)
{
	uint8_t result=0;
		
    /*������������� ��������� DS1307 
    �� ������� �����*/
    twiBuf[0] = (DS1307_ADR<<1)|0; //�������� �����
    twiBuf[1] = 0;                 //����� ��������   
	twiMsgSize = 2;
    
	while(TWI_TransceiverBusy());   //����, ����� TWI ������ �����������
	twiState = TWI_NO_STATE ;
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA); //��������� ���������� � ��������� ��������� �����
    while(TWI_TransceiverBusy());
  
    /*��������� ����� � DS1307*/
    twiBuf[0] = (DS1307_ADR<<1)|1;
 	twiMsgSize = 5;
 	while(TWI_TransceiverBusy());   //����, ����� TWI ������ �����������
	twiState = TWI_NO_STATE ;
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA); //��������� ���������� � ��������� ��������� �����
  
    while(TWI_TransceiverBusy());  
	
	if(twiState == TWI_SUCCESS)
	{    
		//if ((current_time->min != twiBuf[2]) && (current_time->hour != twiBuf[3]))
		//{
			//result = 1;
		//}
		//
		current_time->min  = twiBuf[2];
		current_time->hour = twiBuf[3];
		current_time->day  = twiBuf[4];
	}
	
	return result;
}

ISR(TWI_vect)
{
  static uint8_t ptr;
  uint8_t stat = TWSR & TWSR_MASK;
  
  switch (stat){
    
    case TWI_START:                   // ��������� START ������������ 
    case TWI_REP_START:               // ��������� ��������� START ������������        
       ptr = 0;      

    case TWI_MTX_ADR_ACK:             // ��� ������� ����� SLA+W � �������� �������������
    case TWI_MTX_DATA_ACK:            // ��� ������� ���� ������ � �������� �������������  
       if (ptr < twiMsgSize){
          TWDR = twiBuf[ptr];                    //��������� � ������� ������ ��������� ����
          TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT); //���������� ���� TWINT    
          ptr++;
       }
       else{
          twiState = TWI_SUCCESS;  
          TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO)|(0<<TWIE); //��������� ��������� ����, ���������� ����, ��������� ����������
       }
       break;
     
    case TWI_MRX_DATA_ACK:          //���� ������ ������ � �������� �������������  
       twiBuf[ptr] = TWDR;
       ptr++;
    
    case TWI_MRX_ADR_ACK:           //��� ������� ����� SLA+R � �������� ������������  
      if (ptr < (twiMsgSize-1)){
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);  //���� ��� �� ������������� �������� ����, ��������� �������������                             
      }
      else {
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT);            //���� ������� ������������� ����, ������������� �� ���������
      }    
      break; 
      
    case TWI_MRX_DATA_NACK:       //��� ������ ���� ������ ��� �������������      
      twiBuf[ptr] = TWDR;
      twiState = TWI_SUCCESS;  
      TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO); //��������� ��������� ����
      break; 
     
    case TWI_ARB_LOST:          //��� ������� ��������� 
      TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA); // ���������� ���� TWINT, ��������� ��������� �����
      break;
      
    case TWI_MTX_ADR_NACK:      // ��� ������� ���� SLA+W � �� �������� �������������
    case TWI_MRX_ADR_NACK:      // ��� ������� ����� SLA+R � �� �������� �������������    
    case TWI_MTX_DATA_NACK:     // ��� ������� ���� ������ � �� �������� �������������
    case TWI_BUS_ERROR:         // ������ �� ���� ��-�� ����������� ��������� ����� ��� ����
    default:     
      twiState = stat;                                                                                    
      TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC); //��������� ����������                              
  }
}
