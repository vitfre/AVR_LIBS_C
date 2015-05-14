#include <avr/interrupt.h>
#include "ds1307.h"

#ifndef F_CPU
   #define F_CPU  8000000UL
#endif

/*размер буфера TWI модуля*/
#define TWI_BUFFER_SIZE  8      

/****************************************************************************
  Статусные коды TWI модуля 
****************************************************************************/

/*Общие статусные коды */                    
#define TWI_START                  0x08  // состояние START сформировано 
#define TWI_REP_START              0x10  // состояние повторный START сформировано 
#define TWI_ARB_LOST               0x38  // был потерян приоритет 

/*Статусные коды ведущего передатчика*/                
#define TWI_MTX_ADR_ACK            0x18  // был передан пакет SLA+W и получено подтверждение
#define TWI_MTX_ADR_NACK           0x20  // был передан пает SLA+W и не получено подтверждение
#define TWI_MTX_DATA_ACK           0x28  // был передан байт данных и получено подтверждение  
#define TWI_MTX_DATA_NACK          0x30  // был передан байт данных и не получено подтверждение

/*Статусные коды ведущего приемника*/ 
#define TWI_MRX_ADR_ACK            0x40  // был передан пакет SLA+R и получено подтвеждение 
#define TWI_MRX_ADR_NACK           0x48  // был передан пакет SLA+R и не получено подтверждение 
#define TWI_MRX_DATA_ACK           0x50  // байт данных принят и передано подтверждение  
#define TWI_MRX_DATA_NACK          0x58  // был принят байт данных без подтверждения  

/*Другие статусные коды*/
#define TWI_NO_STATE               0xF8  // неопределенное состояние; TWINT = “0”
#define TWI_BUS_ERROR              0x00  // ошибка на шине из-за некоректных состояний СТАРТ или СТОП

/*Пользовательские коды*/
#define TWI_SUCCESS                0xff

/****************************************************************************
  Определения констант
****************************************************************************/
#define TWI_READ_BIT     0       // позиция R/W бита в адресном пакете
#define TWI_ADR_BITS     1       // позиция адреса в адресном пакете
#define TRUE             1
#define FALSE            0
#define TWSR_MASK     0xfc  

#define DS1307_ADR 104

volatile static uint8_t twiBuf[TWI_BUFFER_SIZE];
volatile static uint8_t twiState = TWI_NO_STATE;      
volatile static uint8_t twiMsgSize;       

/*предделители для установки скорости обмена twi модуля*/
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
		
    /*устанавливаем указатель DS1307 
    на нулевой адрес*/
    twiBuf[0] = (DS1307_ADR<<1)|0; //адресный пакет
    twiBuf[1] = 0;                 //адрес регистра   
	twiMsgSize = 2;
    
	while(TWI_TransceiverBusy());   //ждем, когда TWI модуль освободится
	twiState = TWI_NO_STATE ;
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA); //разрешаем прерывание и формируем состояние старт
    while(TWI_TransceiverBusy());
  
    /*считываем время с DS1307*/
    twiBuf[0] = (DS1307_ADR<<1)|1;
 	twiMsgSize = 5;
 	while(TWI_TransceiverBusy());   //ждем, когда TWI модуль освободится
	twiState = TWI_NO_STATE ;
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA); //разрешаем прерывание и формируем состояние старт
  
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
    
    case TWI_START:                   // состояние START сформировано 
    case TWI_REP_START:               // состояние повторный START сформировано        
       ptr = 0;      

    case TWI_MTX_ADR_ACK:             // был передан пакет SLA+W и получено подтверждение
    case TWI_MTX_DATA_ACK:            // был передан байт данных и получено подтверждение  
       if (ptr < twiMsgSize){
          TWDR = twiBuf[ptr];                    //загружаем в регистр данных следующий байт
          TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT); //сбрасываем флаг TWINT    
          ptr++;
       }
       else{
          twiState = TWI_SUCCESS;  
          TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO)|(0<<TWIE); //формируем состояние СТОП, сбрасываем флаг, запрещаем прерывания
       }
       break;
     
    case TWI_MRX_DATA_ACK:          //байт данных принят и передано подтверждение  
       twiBuf[ptr] = TWDR;
       ptr++;
    
    case TWI_MRX_ADR_ACK:           //был передан пакет SLA+R и получено подтвеждение  
      if (ptr < (twiMsgSize-1)){
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);  //если это не предпоследний принятый байт, формируем подтверждение                             
      }
      else {
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT);            //если приняли предпоследний байт, подтверждение не формируем
      }    
      break; 
      
    case TWI_MRX_DATA_NACK:       //был принят байт данных без подтверждения      
      twiBuf[ptr] = TWDR;
      twiState = TWI_SUCCESS;  
      TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO); //формируем состояние стоп
      break; 
     
    case TWI_ARB_LOST:          //был потерян приоритет 
      TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA); // сбрасываем флаг TWINT, формируем повторный СТАРТ
      break;
      
    case TWI_MTX_ADR_NACK:      // был передан пает SLA+W и не получено подтверждение
    case TWI_MRX_ADR_NACK:      // был передан пакет SLA+R и не получено подтверждение    
    case TWI_MTX_DATA_NACK:     // был передан байт данных и не получено подтверждение
    case TWI_BUS_ERROR:         // ошибка на шине из-за некоректных состояний СТАРТ или СТОП
    default:     
      twiState = stat;                                                                                    
      TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC); //запретить прерывание                              
  }
}
