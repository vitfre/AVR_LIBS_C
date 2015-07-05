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

uint8_t	PWM_Step = 0;	// Текущий шаг для ШИМ
uint8_t	PWM_Data = 0;	// Данные для ШИМ
uint8_t	PWM_Mask = 0;	// Маска каналов на которых НЕ установлен ноль. 
uint8_t	PWM_ValU[PWM_COUNT] = {0}; // Используемые значения для ШИМ каналов
uint8_t	PWM_ValN[PWM_COUNT] = {0}; // Новые значения для ШИМ каналов


#define PWM_Start()	TIMSK0 |= (1 << OCIE0B)
#define PWM_Stop()	TIMSK0 &= ~(1 << OCIE0B)


// Инициализация ШИМ'а
__inline void PWM_Init()
{
	DDRB		|= PWM_CLOCK | PWM_DATA | PWM_LATCH;
	PWM_PORT	&= ~(PWM_CLOCK | PWM_DATA | PWM_LATCH);
	
	TCCR0A	|= (1 << WGM01);	// Режим таймера по достижению значения OCR0A
	TCCR0B	= (1 << CS01);		// Делитель частоты таймера = 8
	OCR0A	= 119;				// Значение регистра сравнения = (((F_CPU / 8) / 100 Гц) / 100 отсчетов) - 1
	 
}

// Установка значения для указанного канала
void PWM_SetVal(uint8_t Index, uint8_t Value)
{
	if (Index < PWM_COUNT)
	{
		PWM_ValN[Index] = Value; // Запишем новые значения для указанного канала.
		
		// Вычисление маски каналов
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

// Отсылка данных на микросхему 74HC595
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

// обработчик прерывания по совпадению таймера
ISR (TIM0_COMPB_vect)
{
	uint8_t	x;
	
	// Проверяем каждый канал на необходимость переключения в LOW
	for (x = 0; x < PWM_COUNT; x++)
	{
		if (PWM_Step == PWM_ValU[x])
		{
			PWM_Data &= ~(1 << x);
		}
	}
	
	// Если достигли максимального значения
	if (PWM_Step == 100)
	{
		// Запомним новые значения для ШИМ
		for (x = 0; x < PWM_COUNT; x++)
		{
			PWM_ValU[x] = PWM_ValN[x];
		}
		
		PWM_Step = 0;
		PWM_Data = PWM_Mask; // Установим HIGH на существующих каналах со значениями отличными от нуля.
	}
		
	PWM_Step++;
		
	PWM_SetData(); // Передадим информацию на регистер сдвига
}


int main(void)
{
	int8_t	x;	
	int8_t	i;
	
	
	PWM_Init();
	PWM_Start();
		
	sei();	// разрешаем прерывания
	
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