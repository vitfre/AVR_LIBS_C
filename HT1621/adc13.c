#include <tiny13a.h>
#include <ht1621.h>
#define ADC_VREF_TYPE 0x00

unsigned int volt_[6], volt;
uchar i;

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}


void main(void)
{
#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

PORTB=0x00;
DDRB.0=1;
DDRB.1=1;
DDRB.2=1;


// ADC initialization
// ADC Clock frequency: 125.000 kHz
// ADC Bandgap Voltage Reference: Off
// ADC Auto Trigger Source: Free Running
// Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
DIDR0&=0x03;
DIDR0|=0x00;
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0xA3;
ADCSRB&=0xF8;


cs(1); //pottiagivaem na plius
  
 out_cmd(0b00101000); //BIAS & COM  
 out_cmd(0b00000001); //SYSTEM ENABLE
 out_cmd(0b00000011);    //TURN ON LCD     
 //out_cmd(0b10100000);
/*==================================================================================*/ 
/*
//Send Data
cs(0);  //set CS LOW
data(1);data(0);data(1);   //send command WRITE = "101"
data(0);data(0);data(0);data(0);data(0);data(1);     data(1);data(1);data(1);data(1);  delay_ms(200);
cs(1); //set CS HIGH
*/

  
while (1)
      { 

read_adc(2); //shitivaem napriajenie
//vmesto plavaiushei zapiatoi uvelichivaiu vse na 10, toest vmnesto 5.0 pishu 50, tak mi ekonomim pamiat :) 
       volt = ADCW;//  >>2;
       volt_[0] = (volt*5)/1023;          //pervaia cifra
       volt_[1] = ((volt*50)/1023) %10;   //vtoraia cifra
       volt_[2] = ((volt*500)/1023) %10;  //tretaia cifra      
 
read_adc(3);  //shitivaem tok     
       volt = ADCW;//  >>2;
       volt_[3] = ((volt*500)/1023)*16/10;             //pervaia cifra
       volt_[4] = (((volt*5000)/1023) %10)*16/10;      //vtoraia cifra
       volt_[5] = (((volt*50000)/1023) %10)*16/10;     //tretaia cifra 
  

/*
if (volt_[3] > 3) {

     out_cmd(0b00001001);  //TONE ON
     delay_ms(1000);
     out_cmd(0b00001000);} //TONE OFF 
     delay_ms(10000);  
*/   
       
i=0;   
while (i != 6) { // cikl gde k chislam prisvaivaem sootvetsvuiushie znachenia dlia LCD
       if (volt_[i] == 6) {volt_[i] = 0b11111010;}  
       if (volt_[i] == 1) {volt_[i] = 0b00000110;}
       if (volt_[i] == 2) {volt_[i] = 0b01111100;}
       if (volt_[i] == 3) {volt_[i] = 0b01011110;}
       if (volt_[i] == 4) {volt_[i] = 0b11000110;}
       if (volt_[i] == 5) {volt_[i] = 0b11011010;}     
       if (volt_[i] == 7) {volt_[i] = 0b00001110;}
       if (volt_[i] == 8) {volt_[i] = 0b11111110;}
       if (volt_[i] == 9) {volt_[i] = 0b11011110;}
       if (volt_[i] == 0) {volt_[i] = 0b10111110;}    
i++;

    };     
    
       
    
      delay_ms(500); // dalshe vivodim vse na LCD
       out_data(volt_[0], volt_[1] | 1, volt_[2], 1); 
       out_data(volt_[3] | 1, volt_[4], volt_[5], 2);
       }
}


/*
data(0);data(0);data(0);data(0);     data(0);data(1);data(1);data(0);       //1   00000110     
data(0);data(1);data(1);data(1);     data(1);data(1);data(0);data(0);       //2   01111100              
data(0);data(1);data(0);data(1);     data(1);data(1);data(1);data(0);       //3   01011110              
data(1);data(1);data(0);data(0);     data(0);data(1);data(1);data(0);       //4   11000110              
data(1);data(1);data(0);data(1);     data(1);data(0);data(1);data(0);       //5   11011010              
data(1);data(1);data(1);data(1);     data(1);data(0);data(1);data(0);       //6   11111010              
data(0);data(0);data(0);data(0);     data(1);data(1);data(1);data(0);       //7   00001110              
data(1);data(1);data(1);data(1);     data(1);data(1);data(1);data(0);       //8   11111110              
data(1);data(1);data(0);data(1);     data(1);data(1);data(1);data(0);       //9   11011110              
data(1);data(0);data(1);data(1);     data(1);data(1);data(1);data(0);       //0   10111110