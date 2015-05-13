#include <tiny25.h>

#include <delay.h>


//#define                                sel PINB.0
#define                                pls PINB.3
#define                                mns PINB.4

#define speaker                  PORTB.0  //17 Пискун

#define ADC_VREF_TYPE 0x00

// Declare your global variables here


unsigned int PWM0=0,PWM=0,fl=0,fl1=0,temp=0;
eeprom unsigned char PWM_eeprom=100;



// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=(adc_input & 0x0f) | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}


//**********************************************************************
void beep (unsigned char squeak)  //Робимо пік
{
for (;squeak>0; squeak--)
{
for (temp=50; temp; temp--)
{
speaker=!speaker;
delay_us(300);
}
//delay_us(900);
}
}




void main(void)
{
// Declare your local variables here

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Port B initialization
// Func5=In Func4=In Func3=In Func2=In Func1=Out Func0=Out
// State5=T State4=P State3=P State2=P State1=0 State0=0
PORTB=0x1C;
DDRB=0x03;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 125,000 kHz
// Mode: Fast PWM top=0xFF
// OC0A output: Disconnected
// OC0B output: Non-Inverted PWM
TCCR0A=0x23;
TCCR0B=0x03;
TCNT0=0x00;
OCR0A=0x00;
OCR0B=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Timer1 Overflow Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
PLLCSR=0x00;

TCCR1=0x00;
GTCCR=0x00;
TCNT1=0x00;
OCR1A=0x00;
OCR1B=0x00;
OCR1C=0x00;

// External Interrupt(s) initialization
// INT0: Off
// Interrupt on any change on pins PCINT0-5: Off
GIMSK=0x00;
MCUCR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// Universal Serial Interface initialization
// Mode: Disabled
// Clock source: Register & Counter=no clk.
// USI Counter Overflow Interrupt: Off
USICR=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
ACSR=0x80;
ADCSRB=0x00;
DIDR0=0x00;


// ADC initialization
// ADC Clock frequency: 125,000 kHz
// ADC Voltage Reference: AVCC pin
// ADC Bipolar Input Mode: Off
// ADC Reverse Input Polarity: Off
// ADC Auto Trigger Source: ADC Stopped
// Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
DIDR0&=0x03;
DIDR0|=0x00;
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x86;
ADCSRB&=0x5F;


beep (15);
PWM=PWM_eeprom;

while (PWM0<PWM) 
{
    PWM0++;
    OCR0B=PWM0;
    delay_ms(10);
}
beep (4);

while (1)
      {
//Опитування кнопок + і - і s
while (pls==0) 
{
    PWM0++;  //якщо кнопка нажата то +1
    if (pls==0&mns==0) 
    {
        beep (15);
        PWM=254;
        while (PWM0<PWM) 
        {
            PWM0++;
            if (PWM0>254) {PWM0=254;}
            OCR0B=PWM0;
            delay_ms(40);
        }
        beep (4);
    };
    if (PWM0>=254) 
    {
        PWM0=254;
        beep (1);
        delay_ms(200);
    };
    delay_ms(10);
    OCR0B=PWM0;
};

while (mns==0) 
{
PWM0--;   //якщо кнопка нажата то -1
if (pls==0&mns==0) 
{
    beep (15);
    PWM=1;
    while (PWM0>PWM) 
    {
        PWM0--;
        if (PWM<1) 
        {
            PWM=1;
        }
        OCR0B=PWM0;
        delay_ms(40);
    }
    beep (4);
}


if (PWM0<1) 
{
    PWM0=1;
    beep (1);
    delay_ms(200);
}
delay_ms(10);
OCR0B=PWM0;
}


if (PWM0==PWM_eeprom) {;}
else  {PWM_eeprom=PWM0;}

}}