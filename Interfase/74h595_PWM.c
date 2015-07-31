#define F_CPU 12000000
#include <util/delay.h>
#include <avr/io.h>


// ANIMATE 0 - no animation
// ANIMATE 1 - fade rotate
// ANIMATE 2 - change sd1 and sd2
#define ANIMATE 1

// Show slowly how frame is drawn
//#define DEMO

void  inline SendByteHorizontal(uint8_t byte);
void  inline SendByteVertical(uint8_t byte);
void  inline OutVert();
void  inline OutHoriz();
void  inline OutOff();
void  inline OutOn();


// 254 - line start
// 255 - screen end

/*
static uint8_t sd1[]={
	254,  4, 10, 12,  6,  2, 10,  9,  1,
	254,  0,  8,  8,  4,  8,  4,  1,  5,
	254,  7,  3,  1,  8, 15,  0, 12, 15,
	254,  2,  1, 11,  1, 14,  7,  6,  9,
	254,  3, 14, 12, 14,  2,  8,  8,  4,
	254,  4,  9,  2,  1,  0, 15,  1,  8,
	254,  2, 15,  6,  6, 12,  5, 14,  6,
	254, 13, 12,  7,  9, 13,  7,  1,  4, 255
};



static uint8_t sd1[]={
	254,  1,  3,  1,  3,  1,  3,  1,  3,
	254,  3,  1,  3,  1,  3,  1,  3,  1,
	254,  1,  3,  1,  3,  1,  3,  1,  3,
	254,  3,  1,  3,  1,  3,  1,  3,  1,
	254,  1,  3,  1,  3,  1,  3,  1,  3,
	254,  3,  1,  3,  1,  3,  1,  3,  1,
	254,  1,  3,  1,  3,  1,  3,  1,  3,
	254,  3,  1,  3,  1,  3,  1,  3,  1,   255
};

*/

static uint8_t sd1[]={
	254,  1,  3,  5,  7,  9, 11, 13, 15,
	254,  3,  5,  7,  9, 11, 13, 15, 13,
	254,  5,  7,  9, 11, 13, 15, 13, 11,
	254,  7,  9, 11, 13, 15, 13, 11,  9,
	254,  9, 11, 13, 15, 13, 11,  9,  7,
	254, 11, 13, 15, 13, 11,  9,  7,  5,
	254, 13, 15, 13, 11,  9,  7,  5,  3,
	254, 15, 13, 11,  9,  7,  5,  3,  1,   255
};


/*





static uint8_t sd1[]={
	254,  0,  1,  2,  3,  4,  5,  6,  7,
	254,  0,  1,  2,  3,  4,  5,  6,  7,
	254,  0,  0,  0,  0,  0,  0,  0,  0,
	254,  8,  9, 10, 11, 12, 13, 14, 15,
	254,  8,  9, 10, 11, 12, 13, 14, 15,
	254,  0,  0,  0,  0,  0,  0,  0,  0,
	254,  0,  0,  0,  0,  0,  0,  0,  0,
	254,  5,  6,  7,  8,  9, 10, 11, 12,   255
};




static uint8_t sd1[]={
	254,  0,  0,  0,  0,  0,  0,  0,  0,
	254,  0,  0, 10, 15, 10,  0,  0,  0,
	254, 13,  0, 15, 15, 15,  0,  0,  0,
	254,  0, 15,  0, 15,  0,  0, 15,  0,
	254,  0,  0, 15, 15, 15, 15,  0,  0,
	254,  0,  0,  0, 15,  0,  0,  0,  0,
	254,  0,  0, 15,  0, 15,  0,  0,  0,
	254,  0,  0, 15,  0, 15,  0,  0,  0,   255
};

*/

static uint8_t sd2[]={
	254,  0,  0,  0,  0,  0,  0,  0,  0,	
	254,  0,  0,  0,  0,  0,  0,  0,  0,
	254,  0,  0, 10, 15, 10,  0,  0,  0,
	254,  0, 13, 15, 15, 15,  0,  0,  0,
	254,  0, 15,  0, 15,  0, 13,  0,  0,
	254,  0,  0, 15, 15, 15, 15,  0,  0,
	254,  0,  0,  0, 15,  0,  0,  0,  0,
	254,  0, 12, 15,  0, 15, 12,  0,  0,   255
};


int main (){
	DDRD=0xff;
	uint8_t b=0b11001101;
	uint8_t c=0;
	#ifdef DEMO
	uint16_t i=0;
	#endif
	unsigned int k=1;
	uint8_t t;
	uint8_t l;
	uint8_t r;
	uint8_t *p;

	

	OutOn();
	SendByteVertical(0);
	SendByteHorizontal(0);
	OutVert();
	OutHoriz();
	_delay_ms(1000);


 	while (1) {
		// simple frame change animation (maybe used as double buffer in the future)
		if(ANIMATE & 2){
			if(c>100) p=sd1;
			else p=sd2;
		}
		else {
			p=sd1;
		}
		
		// main output loop
		t=*p;
		l=0;
		k=1;
		while(t!=255){
			OutOff();			
			if(t==254){	
				if(l)k<<=1;
				p++;
				t=*p;
				l++;
				b=0b10000000;
				SendByteVertical(~k);
				OutVert();
			}
			SendByteHorizontal(b);
			OutHoriz();	
			t&=0b00001111;
			#ifdef DEMO
			for(i=0;i<10000;i++){
			#endif
			for(r=0;r<15;r++){
				if (t>r){
					OutOn();
				}
				else {
					OutOff();
				}
			}
			#ifdef DEMO
			}
			#endif
			p++;
			t=*p;
			b>>=1;
		}

		// animation 
		if (!c && (ANIMATE & 1)){
			p=sd1;
			t=*p;
			while(t!=255){
				if (t!=254){
					if (t&0b10000000){
						t&=0b01111111;
						t--;
						if (t<2){
							t&=0b01111111;
						}
						else {
							t|=0b10000000;
						}
					}
					else {
						t++;
						if (t>14){
							t|=0b10000000;
						}
					}
					*p=t;
				}
				p++;
				t=*p;
			} 
		}

		c++;
		if (ANIMATE==1){
			if (c>20)c=0;
		}
	}




	
}

void inline SendByteHorizontal(uint8_t byte){
	uint8_t mask=0b00000001;
	while (mask){
		PORTD &= (~(1<<PIND3));
		if (mask & byte){
			PORTD |=(1<<PIND1);
		}
		else {
			PORTD &= (~(1<<PIND1));
		}
		PORTD  |= (1<<PIND3);
		mask<<=1;
	}
}

void inline SendByteVertical(uint8_t byte){
	uint8_t mask=0b10000000;
	while (mask){
		PORTD &= (~(1<<PIND3));
		if (mask & byte){
			PORTD |=(1<<PIND1);
		}
		else {
			PORTD &= (~(1<<PIND1));
		}
		PORTD  |= (1<<PIND3);
		mask>>=1;
	}
}

void  inline OutHoriz(){
	PORTD  &= (~(1<<PIND2));
   	PORTD  |= (1<<PIND2);
}
void inline OutVert(){
	PORTD  &= (~(1<<PIND4));
	PORTD  |= (1<<PIND4);


}
void inline OutOn(){
	PORTD &=(~(1<<PIND6));
}
void inline OutOff(){
	PORTD |= (1<<PIND6);
}
