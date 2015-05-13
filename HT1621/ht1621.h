/* HT1621 LCD controller */
/* Made by Badri Bebua aka DLL32 */
#include <delay.h>
#define uchar unsigned char
#define CS PORTB.0
#define WR PORTB.1
#define DAT PORTB.2
#define nop delay_us(10);  

//To send one bit with clock use this function
//when send "1" WR goes HIGH then DAT goes HIGH and WR goes LOW
//when send "0" WR goes HIGH then DAT goes LOW and WR goes LOW
void data(char b){ nop;
if (b ==1) {WR=0; DAT=1; WR=1;}  else {WR=0; DAT = 0; WR=1;}  nop;}

//When the CS is logic high, the data and command read from or written to
//the HT1621 are disabled. Before Send any command you need to pull CS to LOW
void cs(uchar b){
if (b ==1) {CS = 1;} else {CS = 0;}} 


//////////////////////////Parallel to Serial CMD///////////////////////////////
//Example: when you send COMMAND like this 0b00000011, this function will 
//pull CS LOW then send command mode command "100" and then send your command "00000011" to DAT port
//after this CS goes HIGH
void out_cmd(uchar cmd){
                
uchar maska=0b10000000;
cs(0); // CS LOW
data(1);data(0);data(0);  // send command command
                 
do{  WR = 0; //clock LOW         
if(cmd & maska)
 {DAT=1;}
   else {DAT=0;}
maska >>= 1; 
WR = 1; //clock HIGH     
nop; 
}while(maska);

WR = 0; //clock LOW 
DAT=0; // send latest bit, no matter what..
WR = 1; //clock HIGH 

cs(1); // CS HIGH 
}       
//////////////////////////Parallel to Serial CMD///////////////////////////////

//////////////////////////Parallel to Serial DATA///////////////////////////////
void out_data(uchar temp1,uchar temp2,uchar temp3, uchar addr){
                
uchar maska=0b10000000;
cs(0); // CS LOW
data(1);data(0);data(1);  // send command write
if (addr == 1) {data(0);data(0);data(0);data(0);data(0);data(1); } //volt address from 1
                  else {data(0);data(0);data(1);data(1);data(1);data(1); }  //amper address from 15
                  
do{  WR = 0; //clock LOW         
if(temp1 & maska)
 {DAT=1;}
   else {DAT=0;}
maska >>= 1; 
WR = 1; //clock HIGH     
nop; 
}while(maska);
 maska=0b10000000;      
 
do{  WR = 0; //clock LOW         
if(temp2 & maska)
 {DAT=1;}
   else {DAT=0;}
maska >>= 1; 
WR = 1; //clock HIGH     
nop; 
}while(maska);
maska=0b10000000;

do{  WR = 0; //clock LOW         
if(temp3 & maska)
 {DAT=1;}
   else {DAT=0;}
maska >>= 1; 
WR = 1; //clock HIGH     
nop; 
}while(maska);

//data(0);data(1);data(0);data(0);     data(0);data(0);data(0);data(0);       //-
if (addr == 1) {data(0);data(0);data(1);data(1);     data(0);data(0);data(1);data(0);}       //v
    else {data(0);data(1);data(0);data(0);data(0);data(0);data(0);data(0);  data(1);data(1);data(1);data(0);data(1);data(1);data(1);data(0); }      //A
cs(1); // CS HIGH 
}       
//////////////////////////Parallel to Serial DATA///////////////////////////////



/*//(BIAS); //setup bias and working period
0b00101000  
 
//(SYSEN); //start system oscillator 
0b00000001

//SendCmd(LCDON); //switch on LCD bias generator
0b00000011

//(SYS DISABLE); //stop system oscillator 
0b00000000

//(LCDOFF); //switch OFF LCD bias generator
0b00000010

//TONE ON
0b00001001

//TONE OFF
0b000010000

//4khz TONE
0b01001001

//2khz TONE
0b01101001



data(1);data(1);data(1);data(0);     data(1);data(1);data(1);data(0);       //A
data(0);data(0);data(1);data(1);     data(0);data(0);data(1);data(0);       //v
data(0);data(1);data(0);data(0);     data(0);data(0);data(0);data(0);       //-
data(1);data(0);data(1);data(1);     data(1);data(1);data(1);data(0);       //0
data(0);data(0);data(0);data(0);     data(0);data(0);data(0);data(1);       //.
data(0);data(1);data(1);data(1);     data(0);data(0);data(1);data(0);       //o

data(0);data(0);data(0);data(0);     data(0);data(1);data(1);data(0);       //1   00000110     invert - 01100000
data(0);data(1);data(1);data(1);     data(1);data(1);data(0);data(0);       //2   01111100              00111110
data(0);data(1);data(0);data(1);     data(1);data(1);data(1);data(0);       //3   01011110              01111010
data(1);data(1);data(0);data(0);     data(0);data(1);data(1);data(0);       //4   11000110              01100011
data(1);data(1);data(0);data(1);     data(1);data(0);data(1);data(0);       //5   11011010              01011011
data(1);data(1);data(1);data(1);     data(1);data(0);data(1);data(0);       //6   11111010              01011111
data(0);data(0);data(0);data(0);     data(1);data(1);data(1);data(0);       //7   00001110              01110000
data(1);data(1);data(1);data(1);     data(1);data(1);data(1);data(0);       //8   11111110              01111111
data(1);data(1);data(0);data(1);     data(1);data(1);data(1);data(0);       //9   11011110              01111011
data(1);data(0);data(1);data(1);     data(1);data(1);data(1);data(0);       //0   10111110
*/