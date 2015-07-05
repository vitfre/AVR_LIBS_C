/*
 * MultiPWM.c
 *
 * Created: 22.01.2014 22:17:05
 *  Author: Shevmax
 */ 

#define F_CPU  9600000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define PWM_COUNT	8

#define PWM_PORT	PORTB
#define	PWM_CLOCK	(1 << PINB0)
#define	PWM_DATA	(1 << PINB1)
#define	PWM_LATCH	(1 << PINB2)

uint8_t	PWM_Step = 0;	// ������� ��� ��� ���
uint8_t	PWM_Data = 0;	// ������ ��� ���
uint8_t	PWM_Mask = 0;	// ����� ������� �� ������� �� ���������� ����. 
uint8_t	PWM_ValU[PWM_COUNT] = {0}; // ������������ �������� ��� ��� �������
uint8_t	PWM_ValN[PWM_COUNT] = {0}; // ����� �������� ��� ��� �������


#define PWM_Start()	TIMSK0 |= (1 << OCIE0B)
#define PWM_Stop()	TIMSK0 &= ~(1 << OCIE0B)


// ������������� ���'�
__inline void PWM_Init()
{
	DDRB		|= PWM_CLOCK | PWM_DATA | PWM_LATCH;
	PWM_PORT	&= ~(PWM_CLOCK | PWM_DATA | PWM_LATCH);
	
	TCCR0A	|= (1 << WGM01);	// ����� ������� �� ���������� �������� OCR0A
	TCCR0B	= (1 << CS01);		// �������� ������� ������� = 8
	OCR0A	= 119;				// �������� �������� ��������� = (((F_CPU / 8) / 100 ��) / 100 ��������) - 1
	 
}

// ��������� �������� ��� ���������� ������
void PWM_SetVal(uint8_t Index, uint8_t Value)
{
	if (Index < PWM_COUNT)
	{
		PWM_ValN[Index] = Value; // ������� ����� �������� ��� ���������� ������.
		
		// ���������� ����� �������
		if (Value)
		{
			PWM_Mask |= (1 << Index);
		}
		else
		{
			PWM_Mask &= ~(1 << Index);
		}
	}
}

// ������� ������ �� ���������� 74HC595
 void PWM_SetData()
{
	int8_t		x;
	
	PWM_PORT &= ~PWM_LATCH;
		
	for (x = 7; x >= 0; x--)
	{
		if (PWM_Data & (1 << x))
		{
			PWM_PORT |= PWM_DATA;
		}
		else
		{
			PWM_PORT &= ~PWM_DATA;
		}
		
		PWM_PORT |= PWM_CLOCK;
		PWM_PORT &= ~PWM_CLOCK;
	}

	PWM_PORT |= PWM_LATCH; 
}

// ���������� ���������� �� ���������� �������
ISR (TIM0_COMPB_vect)
{
	uint8_t	x;
	
	// ��������� ������ ����� �� ������������� ������������ � LOW
	for (x = 0; x < PWM_COUNT; x++)
	{
		if (PWM_Step == PWM_ValU[x])
		{
			PWM_Data &= ~(1 << x);
		}
	}
	
	// ���� �������� ������������� ��������
	if (PWM_Step == 100)
	{
		// �������� ����� �������� ��� ���
		for (x = 0; x < PWM_COUNT; x++)
		{
			PWM_ValU[x] = PWM_ValN[x];
		}
		
		PWM_Step = 0;
		PWM_Data = PWM_Mask; // ��������� HIGH �� ������������ ������� �� ���������� ��������� �� ����.
	}
		
	PWM_Step++;
		
	PWM_SetData(); // ��������� ���������� �� �������� ������
}


int main(void)
{
	int8_t	x;	
	int8_t	i;
	
	
	PWM_Init();
	PWM_Start();
		
	sei();	// ��������� ����������
	
	for(;;)
	{
		
		for (x = 0; x < 8; x++)
		{
			for (i = 0; i <= 100; i++)
			{
				PWM_SetVal(x, i);	
				_delay_ms(5);
			}
		}
		
		for (x = 0; x < 8; x++)
		{
			for (i = 100; i >= 0; i--)
			{
				PWM_SetVal(x, i);
				_delay_ms(5);
			}
		}
	}
}