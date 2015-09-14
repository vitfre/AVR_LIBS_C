//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: AVR
//
//  Compiler....: �����
//
//  Description.: ������� 7-�� ����������� 4-�� ���������� ���������� 
//
//  Data........: 15.04.10  ������
//  Data........: 8.03.13  ������� ��������� SPI � ������������ ���������     
//
//***************************************************************************
#include "indicator.h"

#ifndef IND_SPI_CONNECTION
   #if (IND_AMOUNT_NUM > 8)
    #error "��� ������ ������������ IND_AMOUNT_NUM ������ ���� ������ 8"
  #endif 
#endif 

#ifndef IND_SHIFT_REG_DIG2
  #if (IND_AMOUNT_NUM > 8)
    #error "��� ������ ������������ IND_AMOUNT_NUM ������ ���� ������ 8"
  #endif 

  #if ((IND_SHIFT_REG_SEG > 1)||(IND_SHIFT_REG_DIG1 > 1))
    #error "��� ������ ������������ IND_SHIFT_REG_SEG � IND_SHIFT_REG_DIG1 ������ ���� ������ 2"
  #endif 
#endif

#ifdef IND_SPI_CONNECTION
 #ifdef IND_SHIFT_REG_DIG2
   #define IND_AMOUNT_REG 3
 #else 
   #define IND_AMOUNT_REG 2
 #endif
   uint8_t spi_buf[IND_AMOUNT_REG];
#endif

#define IND_LIGHT_OUT 0xff

#if (IND_ACTIVE_LEVEL_SEG == 0)
   #define IND_OutSeg(port, value) port &= ~(value)
   #define IND_LightOutSeg(port, value) port |= (value)
#else 
   #define IND_OutSeg(port, value) port |= (value)
   #define IND_LightOutSeg(port, value) port &= ~(value)
#endif

#if (IND_ACTIVE_LEVEL_DIG == 0)
   #define IND_OutDig(port, value) port &= ~(value)
   #define IND_LightOutDig(port, value) port |= (value)
#else 
   #define IND_OutDig(port, value) port |= (value)
   #define IND_LightOutDig(port, value) port &= ~(value)
#endif

#define IND_EMPTY_NUM 10

/*����� 7-�� ���������� ����������*/
uint8_t ind_buf[IND_AMOUNT_NUM];

/*������� �������������*/
__flash uint8_t number[] = 
{
  (1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)|(0<<IND_G), //0
  (0<<IND_A)|(1<<IND_B)|(1<<IND_C)|(0<<IND_D)|(0<<IND_E)|(0<<IND_F)|(0<<IND_G), //1
  (1<<IND_A)|(1<<IND_B)|(0<<IND_C)|(1<<IND_D)|(1<<IND_E)|(0<<IND_F)|(1<<IND_G), //2
  (1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(0<<IND_E)|(0<<IND_F)|(1<<IND_G), //3   
  (0<<IND_A)|(1<<IND_B)|(1<<IND_C)|(0<<IND_D)|(0<<IND_E)|(1<<IND_F)|(1<<IND_G), //4
  (1<<IND_A)|(0<<IND_B)|(1<<IND_C)|(1<<IND_D)|(0<<IND_E)|(1<<IND_F)|(1<<IND_G), //5 
  (1<<IND_A)|(0<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)|(1<<IND_G), //6
  (1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(0<<IND_D)|(0<<IND_E)|(0<<IND_F)|(0<<IND_G), //7   
  (1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)|(1<<IND_G), //8
  (1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(0<<IND_E)|(1<<IND_F)|(1<<IND_G), //9    
  (0<<IND_A)|(0<<IND_B)|(0<<IND_C)|(0<<IND_D)|(0<<IND_E)|(0<<IND_F)|(0<<IND_G)  //IND_EMPTY_NUM   
};

__flash uint8_t digit[] = 
{
   IND_NUM1,  
   IND_NUM2,
   IND_NUM3,   
   IND_NUM4,  
   IND_NUM5,  
   IND_NUM6,
   IND_NUM7,   
   IND_NUM8  
#ifdef IND_SHIFT_REG_DIG2   
   ,IND_NUM9,  
   IND_NUM10,
   IND_NUM11,   
   IND_NUM12,  
   IND_NUM13,  
   IND_NUM14,
   IND_NUM15,   
   IND_NUM16   
#endif 
};


#ifdef IND_SCAN_SEGMENT
__flash uint8_t mask[] = 
{
   (1<<IND_A),  
   (1<<IND_B),
   (1<<IND_C),   
   (1<<IND_D),  
   (1<<IND_E),  
   (1<<IND_F),
   (1<<IND_G),   
   (1<<IND_COM)
};
#endif 

/******************************************************************************/
void IND_Init(void)
{
  uint8_t i;
#ifndef IND_SPI_CONNECTION
  IND_PORT_SEG = (1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)|(1<<IND_G)|(1<<IND_COM);
  IND_DDR_SEG = (1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)|(1<<IND_G)|(1<<IND_COM);
  
  for(i = 0; i < IND_AMOUNT_NUM; i++){
     IND_PORT_DIG |= (1<<read_byte_flash(digit[i]));
     IND_DDR_DIG |= (1<<read_byte_flash(digit[i]));
  }
#else 
    SPI_Init();  
#endif   

  for(i = 0; i < IND_AMOUNT_NUM; i++) {
     ind_buf[i] = 0;
  }
}

/******************************************************************************/
void IND_Output(uint16_t value, uint8_t comma)
{
  uint8_t tmp;    
  uint8_t i;
  
  
  for(i = 0; i < IND_AMOUNT_NUM; i++){
    tmp = value % 10;
    if ((i == 0)||(value > 0)||(i <= comma)){ 
       ind_buf[i] = read_byte_flash(number[tmp]);
    }
    else{
       ind_buf[i] = read_byte_flash(number[IND_EMPTY_NUM]);  
    }
    value = value/10;
  }
  
  if ((comma <= IND_AMOUNT_NUM) && (comma != 0)) {
    ind_buf[comma-1] |= 1<<IND_COM;
  }
}

/******************************************************************************/
void IND_OutputFormat(uint16_t value, uint8_t comma, uint8_t position, uint8_t amount)
{
  uint8_t tmp;
  uint8_t i;
  
  if ((position <= IND_AMOUNT_NUM)&&(position != 0)){
     for(i = 0; i < amount; i++){
        tmp = value % 10;
        if ((i == 0)||(value > 0)||(position <= comma)){ 
           ind_buf[position-1] = read_byte_flash(number[tmp]);
        }
        else{
           ind_buf[position-1] = read_byte_flash(number[IND_EMPTY_NUM]);
        }      
        value = value/10;
        position++;
        if (position > IND_AMOUNT_NUM){
           break;
        } 
     }
  }
     
  if ((comma <= IND_AMOUNT_NUM) && (comma != 0)) {
    ind_buf[comma-1] |= 1<<IND_COM;
  }
}

/******************************************************************************/
void IND_Update(void)
{
  static uint8_t count = 0;     
  uint8_t i;
  
#ifdef IND_SPI_CONNECTION    
  /*________________ ����������� �� SPI____________________________*/

 #ifdef IND_SHIFT_REG_DIG2  
  uint16_t tmp;
 #else 
  uint8_t tmp;
 #endif
  
  /*����� ���������*/
  IND_LightOutSeg(spi_buf[IND_SHIFT_REG_SEG],IND_LIGHT_OUT);
  
  tmp = 0;
  for (i = 0; i < IND_AMOUNT_NUM; i++){
     tmp |= (1<<read_byte_flash(digit[i]));  
  }
  IND_LightOutDig(spi_buf[IND_SHIFT_REG_DIG1],((uint8_t)tmp));
  
 #ifdef IND_SHIFT_REG_DIG2  
  IND_LightOutDig(spi_buf[IND_SHIFT_REG_DIG2],(tmp>>8));
 #endif  
  
  SPI_WriteArray(IND_AMOUNT_REG, spi_buf); 

 #ifdef IND_SCAN_SEGMENT
 /*___________���������� ������������ ���������___________*/ 
  
  tmp = 0;
  for (i = 0; i < IND_AMOUNT_NUM; i++){
    if (ind_buf[i]&read_byte_flash(mask[count])) {
       tmp |= (1<<read_byte_flash(digit[i]));
    }
  }
  IND_OutDig(spi_buf[IND_SHIFT_REG_DIG1], ((uint8_t)tmp));  
  
 #ifdef IND_SHIFT_REG_DIG2  
  IND_OutDig(spi_buf[IND_SHIFT_REG_DIG2],(tmp>>8));
 #endif      
  
  IND_OutSeg(spi_buf[IND_SHIFT_REG_SEG], read_byte_flash(mask[count])); 

  count++;
  count &= 0x07;
  
 #else
  /*___________���������� ����������� ���������___________*/ 
  
  tmp = 1<<read_byte_flash(digit[count]);
  IND_OutDig(spi_buf[IND_SHIFT_REG_DIG1], ((uint8_t)tmp));  
  
 #ifdef IND_SHIFT_REG_DIG2  
  IND_OutDig(spi_buf[IND_SHIFT_REG_DIG2],(tmp>>8));
 #endif    
  
  IND_OutSeg(spi_buf[IND_SHIFT_REG_SEG], ind_buf[count]);
  
  count++;
  if (count >= IND_AMOUNT_NUM){
     count = 0;  
  }
 #endif
  
  /*����� �� ���������*/
  SPI_WriteArray(IND_AMOUNT_REG, spi_buf); 
  
#else
 /*____________________������� �����������____________________________*/ 
 
 uint8_t tmp; 
  
 /*����� ��� ����������*/
 IND_LightOutSeg(IND_PORT_SEG, IND_LIGHT_OUT);
 
 tmp = 0;
 for (i = 0; i < IND_AMOUNT_NUM; i++){
    tmp |= (1<<read_byte_flash(digit[i]));  
 }
 IND_LightOutDig(IND_PORT_DIG, tmp);
   
 #ifdef IND_SCAN_SEGMENT
 /*____________���������� ������������ ���������___________*/ 
 
  tmp = 0;
   
  for (i = 0; i < IND_AMOUNT_NUM; i++){
    if (ind_buf[i]&read_byte_flash(mask[count])) {
       tmp |= (1<<read_byte_flash(digit[i]));
    }
  }
  
  IND_OutDig(IND_PORT_DIG, tmp);
  IND_OutSeg(IND_PORT_SEG, read_byte_flash(mask[count]));
  
  count++;
  count &= 0x07;
  
 #else
 /*____________��������� ����������� ���������____________*/  
  
  IND_OutDig(IND_PORT_DIG, (1<<read_byte_flash(digit[count])));
  IND_OutSeg(IND_PORT_SEG, ind_buf[count]);
  
  count++;
  if (count >= IND_AMOUNT_NUM){
     count = 0;  
  }
 #endif
#endif 
}
