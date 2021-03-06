#include <mega8.h>
#include <math.h>

#define SIN_FREQ 1000 //������� ������
#define MAX_INT 16 //���������� ������, �������� 4-256

typedef unsigned char byte;

//����� ����� ������� ������
byte sin_table[MAX_INT];  

byte i;

//��� ������ ���������� ������� ����� ���������
//������� ��������� OCR2 ������� �������
interrupt [TIM1_COMPA] void timer1_compa_isr(void) {
OCR2=sin_table[i];
i++;
#if MAX_INT<256
if (i>MAX_INT-1) i=0;
#endif
}

//����� ���������� ������������� ������� ������
void sin_init() {
//float g=(PI/180.0)*(360.0/(float)MAX_INT);
float g=2*PI/(float)MAX_INT;
byte i=0;
do {
sin_table[i]=(byte)((sin((float)i*g)+1.0)*127.0);
i++;
#if MAX_INT<256
} while(i!=MAX_INT);
#else
} while (i!=0);
#endif
}

void main(void) {
//����������� �����
PORTB=0;
DDRB=8;
PORTC=0;
DDRC=0;
PORTD=0;
DDRD=0;

//����������� ������ ������ �� ������� 8MHz
//top=OCR1A
TCCR1A=0;
TCCR1B=8;
TCNT1=0;
ICR1=0;
//������������� ������� �������
//����� � ������������ ������� ������
//������� ������ [������� �������]/[���������� ������]/[������� ������]
OCR1A=16000000/MAX_INT/SIN_FREQ;
OCR1B=0;

//����������� ������ ������ �� ������� 8MHz
//������ ������ � ��� ���
ASSR=0;
TCCR2=0x68;
TCNT2=0;
OCR2=0x80;

//��������� ���������� ������� �������
//OCR1A
TIMSK=0x10;

//��������� ����������, �� ��� �� �����
ACSR=0x80;

//��������� ����������
#asm("sei")

//���������� �������������� �������
sin_init();

//�������� �������
TCCR2|=1;
TCCR1B|=1;
while (1) 
{



}
} 