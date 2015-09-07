//.............................................................................
//                         GLOBAL INCLUDES 
//.............................................................................
#include <mega128.h>
#include <delay.h>
//.............................................................................
//                         GLOBAL DEFINES 
//.............................................................................
#define setbit(adress,bit) (adress |=(1<<bit))        //function is set bit
#define clearbit(adress,bit) (adress &=~(1<<bit))    //function is clear bit
//*****************************************************************************
void channel_selection(unsigned char channel)
{
clearbit(PORTF,1);
delay_ms(2);
    switch(channel){
        case 0:{clearbit(PORTF,0);clearbit(PORTF,2);};break;
        case 1:{setbit(PORTF,0);clearbit(PORTF,2);};break;
        case 2:{clearbit(PORTF,0);setbit(PORTF,2);};break;
        case 3:{setbit(PORTF,0);setbit(PORTF,2);};break;                            
    };
setbit(PORTF,1);
delay_ms(18);
}