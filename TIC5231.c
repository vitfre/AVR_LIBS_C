/*
 * TIC5231.c
 *
 * Created: 16.09.2014 10:51:14
 *  Author: Администратор
 */ 

#include "TIC5231.h"

//............................................................................
//                      FUNCTIONS DECLARATIONS  
//............................................................................
//*****************************************************************************
unsigned int MAT(float data,unsigned char t_adr)
{
	switch(t_adr)
	{
		case 0:{data=data*100;data=data/1128;data=data-104;};break;      //112.1
		case 1:{data=data*100;data=data/1116;data=data-104;};break;      //112.8
		case 2:{data=data*100;data=data/1124;data=data-104;};break;
		case 3:{data=data*100;data=data/1120;data=data-104;};break;      //112.4                         
    };
	return data;
};

void BCDconv(unsigned int a,unsigned char *digit5,unsigned char *digit4,unsigned char *digit3,unsigned char *digit2,unsigned char *digit1,unsigned char *digit0)
{
	unsigned char i=0;
	//while (a>=100000){ a-=100000; i++;};*digit5=i; i=0;
	while (a>=10000){ a-=10000; i++;};*digit4=i; i=0;
	while (a>=1000){ a-=1000; i++;};*digit3=i; i=0;
	while (a>=100){ a-=100; i++;};*digit2=i; i=0;
	while (a>=10){ a-=10; i++;};*digit1=i; i=0;
	while (a>=1){ a-=1; i++;};*digit0=i;
	*digit5=10;
}

unsigned char seg_conv(unsigned char data)
{
	unsigned char code=0;    
    switch(data)
	{
        case 0:{code=0b01011111;};break;
        case 1:{code=0b01010000;};break;
        case 2:{code=0b01101101;};break;
        case 3:{code=0b01111001;};break;
        case 4:{code=0b01110010;};break;
        case 5:{code=0b00111011;};break;
        case 6:{code=0b00111111;};break;
        case 7:{code=0b01010001;};break;
        case 8:{code=0b01111111;};break;
        case 9:{code=0b01111011;};break;
        case 10:{code=0b00000000;};break;                                
    };
	return code;
};

void lcdsnd(unsigned char lcd_bt,unsigned char sym,unsigned char dot)
{
	unsigned char a=0;
	unsigned char temp_a;
    switch(sym)
	{
        case 0:
		{
            for (a=0;a<=7;a++)
                {
					_delay_us(20);
					setbit(PORTD,5);
					temp_a=lcd_bt<<7;
					temp_a=temp_a>>7;     
					if (temp_a==1)
					{
						setbit(PORTD,6);
					}else
					{
						clearbit(PORTD,6);
					};	
					_delay_us(20);
					clearbit(PORTD,5);
					lcd_bt=lcd_bt>>1;
	            };
        };break;
		case 1:{
            
            for (a=0;a<=6;a++)
	            {
					_delay_us(20);
					setbit(PORTD,5);
					temp_a=lcd_bt<<7;
					temp_a=temp_a>>7;     
					if (temp_a==1)
					{
						setbit(PORTD,6);
					}else
					{
						clearbit(PORTD,6);
					};	
					_delay_us(20);
					clearbit(PORTD,5);
					lcd_bt=lcd_bt>>1;
	            };

        };break;
		case 2:{           
            for (a=0;a<=6;a++)
	            {
					_delay_us(20);
					setbit(PORTD,5);
					temp_a=lcd_bt<<7;
					temp_a=temp_a>>7;     
					if (temp_a==1)
					{
						setbit(PORTD,6);
					}else
					{
						clearbit(PORTD,6);
					};	
					_delay_us(20);
					clearbit(PORTD,5);
					lcd_bt=lcd_bt>>1;
	            };
                _delay_us(20);
	            setbit(PORTD,5);
                if (dot==1)
				{
					setbit(PORTD,6);
				}else
				{
					clearbit(PORTD,6);
				};
	            _delay_us(20);
	            clearbit(PORTD,5);
        };break;
		case 3:{
            for (a=0;a<=6;a++)
	            {
					_delay_us(20);
					setbit(PORTD,5);
					temp_a=lcd_bt<<7;
					temp_a=temp_a>>7;     
					if (temp_a==1)
					{
						setbit(PORTD,6);
					}else
					{
						clearbit(PORTD,6);
					};	
					_delay_us(20);
					clearbit(PORTD,5);
					lcd_bt=lcd_bt>>1;
	            };
        };break;
		case 4:{
            for (a=0;a<=6;a++)
	            {
					_delay_us(20);
					setbit(PORTD,5);
					temp_a=lcd_bt<<7;
					temp_a=temp_a>>7;     
					if (temp_a==1)
					{
						setbit(PORTD,6);
					}else
					{
						clearbit(PORTD,6);
					};	
					_delay_us(20);
					clearbit(PORTD,5);
					lcd_bt=lcd_bt>>1;
	            };
                _delay_us(20);
	            setbit(PORTD,5);
                if (dot==2)
				{
					setbit(PORTD,6);
				}else
				{
					clearbit(PORTD,6);
				};
	            _delay_us(20);
	            clearbit(PORTD,5);
         };break;
		case 5:{
            for (a=0;a<=3;a++)
	            {
					_delay_us(20);
					setbit(PORTD,5);
					temp_a=lcd_bt<<7;
					temp_a=temp_a>>7;     
					if (temp_a==1)
					{
						setbit(PORTD,6);
					}else
					{
						clearbit(PORTD,6);
					};	
					_delay_us(20);
					clearbit(PORTD,5);
					lcd_bt=lcd_bt>>1;
	            };
        };break;
								
	};
};

void lcdclr(unsigned char lcd_adr,unsigned char a)
{
	unsigned char i=0;
	for (i=1;i<=41;i++)
	{
		_delay_us(20);
		setbit(PORTD,5);
		if (a)
		{
			setbit(PORTD,6);
		}
		else
		{
			clearbit(PORTD,6);
		};  
		_delay_us(20);
		clearbit(PORTD,5);
	}; 
	lcd_viv
	//lcd_viv(lcd_adr);
};

void viv_data(unsigned int a,unsigned char lcd_adr,unsigned char rec)
{
	unsigned char seg0=0,seg1=0,seg2=0,seg3=0,seg4=0,seg5=0;
	if (a<5) {a=5;};
	//====================================================================
	BCDconv(a,&seg5,&seg4,&seg3,&seg2,&seg1,&seg0);
	//====================================================================
	if (seg4==0) {seg4=seg5;};
	if (seg3==0) {seg3=seg5;};
	if (seg2==0) {seg2=seg5;};
	//if (seg1==0) {seg1=seg5;};
	seg5=seg_conv(seg5);
	seg4=seg_conv(rec);
	seg3=seg_conv(seg3);
	seg2=seg_conv(seg2);
	seg1=seg_conv(seg1);
	seg0=seg_conv(seg0);
	//====================================================================
	lcdsnd(seg5,5,0);
	lcdsnd(seg4,4,0);
	lcdsnd(seg5,3,0);
	lcdsnd(seg2,2,0);     
	lcdsnd(seg1,1,0); //lcdsnd(Charo,1,0);
	lcdsnd(seg0,0,0); //lcdsnd(CharC,0,0);
	lcd_viv
	//lcd_viv(lcd_adr);
};

void viv_data_l(unsigned int a,unsigned char lcd_adr)
{
	unsigned char seg0=0,seg1=0,seg2=0,seg3=0,seg4=0,seg5=0;
	//====================================================================
	BCDconv(a,&seg5,&seg4,&seg3,&seg2,&seg1,&seg0);
	//====================================================================
	seg5=seg_conv(seg5);
	seg4=seg_conv(seg4);
	seg3=seg_conv(seg3);
	seg2=seg_conv(seg2);
	seg1=seg_conv(seg1);
	seg0=seg_conv(seg0);
	//====================================================================
	lcdsnd(seg5,5,0);
	lcdsnd(seg5,4,0);
	lcdsnd(seg5,3,0);
	lcdsnd(seg5,2,0);     
	lcdsnd(seg5,1,0); //lcdsnd(Charo,1,0);
	lcdsnd(seg0,0,0); //lcdsnd(CharC,0,0);
	
	
	lcd_viv
	//lcd_viv(lcd_adr);
};
