// PD7=LCD_D7 PD6=LCD_D6 PD5=LCD_D5 PD4=LCD_D4
// PD3=LCD_E
// PD2=LCD_RS

//PB0=DS18b20_data

#define F_CPU 4000000UL

#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>

#define RS    2
#define E     3
#define TIME 10
#define PORTB_MASK 0x01

unsigned int  temperature=0;
unsigned char temperature_sign=0;


void pause (unsigned int a)               //????????
{ unsigned int i;

  for (i=a;i>0;i--)
   ;
}

void lcd_com (unsigned char lcd)          //??????? ???
{ unsigned char temp;

  temp=(lcd&~(1<<RS))|(1<<E);
  PORTD=temp;
   asm("nop");
  PORTD=temp&~(1<<E);

  temp=((lcd*16)&~(1<<RS))|(1<<E);
  PORTD=temp;
   asm("nop");
  PORTD=temp&~(1<<E);

  pause (10*TIME);
}

void lcd_dat (unsigned char lcd)          //????? ??????? ?? ??? ? ??????? ???????
{ unsigned char temp;

  temp=(lcd|(1<<RS))|(1<<E);
  PORTD=temp;
   asm("nop");
  PORTD=temp&~(1<<E);

  temp=((lcd*16)|(1<<RS))|(1<<E);
  PORTD=temp;
   asm("nop");
  PORTD=temp&~(1<<E);

  pause(TIME);
}

void lcd_init (void)     //????????????? ???
{
 lcd_com(0x0c);
 pause(100*TIME);
 lcd_com(0x2c);
 pause(100*TIME);
 lcd_com(0x01);
 pause (100*TIME);
}

unsigned char present_ds18b20(void)
{ unsigned char res;

  DDRB|= PORTB_MASK;          //???? ????????????? ??? ?????
  _delay_loop_2(475);         //???? 480???

  DDRB&=~PORTB_MASK;          //???? ????????????? ??? ????
  _delay_loop_2(65);          //???? 70???

  if ((PINB&PORTB_MASK) == 0x00) res=1;  //???? ?? ???? ???????? ?????? ???????
                            else res=0;  //?? ?????????? 1, else 0
 _delay_loop_2(405);          //???? 410???
 return res;
}

void send_ds18b20(unsigned char command)
{ unsigned char i, data;

 data=command;

 for(i=0;i<8;i++)
 {
  if ((data&0x01)==0x01) {                    	//???????? ???. 1
                          DDRB|=PORTB_MASK;
		  				  _delay_loop_2(2);		//6
						  DDRB&=~PORTB_MASK;
						  _delay_loop_2(60);	//64
                         }
                    else {                   	//???????? ???. 0
				          DDRB|=PORTB_MASK; 
                          _delay_loop_2(55);    //60
				          DDRB&=~PORTB_MASK;
						  _delay_loop_2(2);		//10
                         }
  data=data>>1;
}
}

void receive_ds18b20(void)
{ unsigned char i;

 temperature_sign=0;
 for(i=0;i<16;i++)
 {
  DDRB|=PORTB_MASK;
  _delay_loop_2(2);       		//???????? 6???
  DDRB&=~PORTB_MASK;
  _delay_loop_1(6);           	//???????? 9???

  if ((PINB & PORTB_MASK)==0x00) temperature&=~_BV(i);   			//?????? ???. 0
                           else {
                                 temperature|=_BV(i);	           	//?????? ???. 1
                                 if (i==12) temperature_sign=1;		//???? 12-? ?????? 1, ?? ??????????? ? ???????
                                }
  _delay_loop_2(50);      											//???????? 55???
 }
}
           
void show_temp (void)
{ unsigned int temp_des=0, temp_int=0;
  unsigned char h,d,o;

 lcd_init();
 lcd_dat('T');
 lcd_dat('e');
 lcd_dat('m');
 lcd_dat('p');
 lcd_dat('.');
 lcd_dat('=');

 if (temperature_sign==1) {
                           temperature=65536-temperature;
					       lcd_dat('-');
                          }
			          else lcd_dat(' ');			    

 temp_des=temperature&0b00001111;
 temp_int=temperature>>4;

 h=temp_int / 100;
 d=(temp_int-h*100)/10;
 o=temp_int-h*100-d*10;

 if (h!=0) lcd_dat (0x30+h);
 if (d!=0) lcd_dat (0x30+d);
 lcd_dat (0x30+o);
 lcd_dat ('.');

 if(temp_des==0) lcd_dat(0x30);
	else if(temp_des==1 || temp_des==2)	 lcd_dat(0x31);
 	else if(temp_des==3)				 lcd_dat(0x32);
 	else if(temp_des==4 || temp_des==5)	 lcd_dat(0x33);
 	else if(temp_des==6 || temp_des==7)	 lcd_dat(0x34);
 	else if(temp_des==8)				 lcd_dat(0x35);
 	else if(temp_des==9 || temp_des==10) lcd_dat(0x36);
 	else if(temp_des==11)				 lcd_dat(0x37);
 	else if(temp_des==12 || temp_des==13)lcd_dat(0x38);
 	else if(temp_des==14 || temp_des==15)lcd_dat(0x39);
 lcd_dat(0xdf);
 lcd_dat('C');
}

void no_sensor (void)
{ unsigned char i;
 lcd_init();
 lcd_dat ('N'); lcd_dat ('o'); lcd_dat (' '); lcd_dat ('s');
 lcd_dat ('e'); lcd_dat ('n'); lcd_dat ('s'); lcd_dat ('o');
 lcd_dat ('r');

 for (i=0;i<9;i++)
  pause (32000);
}

int main(void)
{ unsigned char i;

DDRD=0xfc;             //????????????? ????? ??? ???

PORTB&=~PORTB_MASK;     //?????????? ???? ????????	
DDRB&=~PORTB_MASK;     //

pause(3200*TIME);
pause(3200*TIME);
pause(3200*TIME);
pause(3200*TIME);

for (i=0;i<5;i++)
{
lcd_init();
pause(200);
}
        
while (1)
{   
 if (present_ds18b20()==1) {
                            send_ds18b20(0xcc);
                            send_ds18b20(0x44);

                             for (i=0;i<6;i++)
                              pause (32000);

                            if (present_ds18b20()==0) no_sensor();
						                        else { 
                                                      send_ds18b20(0xcc);
						                              send_ds18b20(0xbe);
                                                      receive_ds18b20();
                                                      show_temp();
                                                     }
                           }
                      else  no_sensor();               
} 

return 0;
};






