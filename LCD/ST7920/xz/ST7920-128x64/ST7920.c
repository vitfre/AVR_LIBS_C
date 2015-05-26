/****************************************************/
/* Модуль для работы с LCD                          */
/* Автор:       SypH3r                              */
/* Website:     MCUS.RU                             */
/* Компиляторы: HI-TECH PICC18                      */
/****************************************************/

#include "ST7920.h"
#include <htc.h>
#include "font.h"
#include "string.h"




void lcd_wr_byte(unsigned char command) //Отправка  байта
{
        DB4_TRIS = 0;
        DB5_TRIS = 0;
        DB6_TRIS = 0;
        DB7_TRIS = 0;
        
        LCD_RW = 0;
	LCD_EN = 1;
	// Пуляем старшие пол байта
	LCD_DB4 = (command & (1 << 4)) >> 4;
        LCD_DB5 = (command & (1 << 5)) >> 5;
        LCD_DB6 = (command & (1 << 6)) >> 6;
        LCD_DB7 = (command & (1 << 7)) >> 7;
	
        DelayUs(10);
        LCD_EN = 0;
        DelayUs(10);

	
	LCD_EN = 1;
	// Пуляем младшие пол байта
        LCD_DB4 = (command & (1 << 0)) >> 0;
        LCD_DB5 = (command & (1 << 1)) >> 1;
        LCD_DB6 = (command & (1 << 2)) >> 2;
        LCD_DB7 = (command & (1 << 3)) >> 3;
	
        DelayUs(10);
        LCD_EN = 0;

	DelayUs(10);					// Если отображение к вас не правильное - увеличивайте задержки в этой всей процедуре
}

void lcd_wr_ni_byte(unsigned char command) //Отправка половины байта
{
        DB4_TRIS = 0;
        DB5_TRIS = 0;
        DB6_TRIS = 0;
        DB7_TRIS = 0;
        
        LCD_RW = 0;
	LCD_EN = 1;

	LCD_DB4 = (command & (1 << 4)) >> 4;
        LCD_DB5 = (command & (1 << 5)) >> 5;
        LCD_DB6 = (command & (1 << 6)) >> 6;
        LCD_DB7 = (command & (1 << 7)) >> 7;

        DelayUs(50);
        LCD_EN = 0;
	DelayUs(280);					
}

void lcd_data(unsigned char data) //Запись данных
{
        DB4_TRIS = 0;
        DB5_TRIS = 0;
        DB6_TRIS = 0;
        DB7_TRIS = 0;
        
        LCD_RW = 0;
        LCD_RS=1;
        DelayUs(10);
	lcd_wr_byte(data);
}

void lcd_comm(unsigned char command) //Запись комманд
{
        DB4_TRIS = 0;
        DB5_TRIS = 0;
        DB6_TRIS = 0;
        DB7_TRIS = 0;
        
        LCD_RW = 0;
        LCD_RS=0;
        DelayUs(10);
	lcd_wr_byte(command);
}

void lcd_init(void)
{
        LCD_REST=1;
        LCD_REST=0;
        DelayMs(5);
        LCD_REST=1;

	DelayUs(200);

        lcd_wr_ni_byte(0b00110000); // 4 битный режим
        DelayMs(5);


        lcd_wr_ni_byte(0b00100000); // 4 битный режим
        DelayUs(200);


	lcd_comm(0b00100000); // 4 битный режим
	DelayUs(100);

	lcd_comm(0b00001100); // Включаем дисплей, выключаем курсор и его мигание
	DelayUs(200);

	lcd_comm(0b00000001); 	  // Очистка экрана
	DelayMs(15);

	lcd_comm(0x06); 	  // Курсор двимгается вправо, без сдвига экрана
	DelayUs(200);

	lcd_comm(0b00000010); // Курсой пинаем домой

        DelayUs(200);

        //Включаем графический режим
        lcd_comm(0b00100000);//(0x20);
	DelayMs(1);
	lcd_comm(0b00100100);//(0x24);			// Переключаемся в режим расширенных инструкций
	DelayMs(1);
	lcd_comm(0b00100110);//(0x26);			// Включаем графический режим
	DelayMs(1);
	
}

void lcd_clear(void)
{
	unsigned char x, y;
	for(y = 0; y < 64; y++)
	{
		if(y < 32)
		{
			lcd_comm(0x80 | y);
                        lcd_comm(0x80);
		}
		else
		{
			lcd_comm(0x80 | (y-32));
                        lcd_comm(0x88);
		}
		for(x = 0; x < 8; x++)
		{
			lcd_data(0x00);
			lcd_data(0x00);
		}
	}
}

void lcd_image(const unsigned char* graphic)
{
	unsigned char x, y;
	for(y = 0; y < 64; y++)
	{
		if(y < 32)
		{
			for(x = 0; x < 8; x++)			 //Верхняя часть экрана
			{					 //В режиме расширенных инструкций, х и у координаты должны быть указаны перед отправкой данных
				lcd_comm(0x80 | y);		 // Вертикальные координаты
                                lcd_comm(0x80 | x);		 // Горизонтальные от 0 до 8
				lcd_data(graphic[2*x + 16*y]);	 // Старший байт данных
				lcd_data(graphic[2*x+1 + 16*y]); // Младший байт данных
			}
		}
		else
		{
			for(x = 0; x < 8; x++)			 // Отрисовка нижней части экрана
			{							
				lcd_comm(0x80 | (y-32));			
                                lcd_comm(0x88 | x);
				lcd_data(graphic[2*x + 16*y]);
				lcd_data(graphic[2*x+1 + 16*y]);
			}
		}
		
	}
}


void lcd_puts(unsigned char line, char* str)
{
	// Вывод текста на экран используя шрифт 5x7 по 16 символов на 8 строках,
	// так как ячейка памяти 16 бит - выводится 2 символа за 1 проход цикла
	unsigned char row;
	unsigned char colInd;
	unsigned char count = 0;
        unsigned char next = 1;

	while(*str && next)
	{
		unsigned char letterA = *str++; //Если конец строки то меняем последний символ пробелом
		if (letterA == 0) {letterA=32; next=0;}

		unsigned char letterB = *str++;
		if (letterB == 0) {letterB=32; next=0;}


		unsigned char indA = letterA - 0x20; //Получаем расположение строки в матрице chars
		unsigned char indB = letterB - 0x20;
		unsigned char colListA[5];
		unsigned char colListB[5];

                //Читаем строку в матрице chars и формируем строку colListA
                //0x42,0x61,0x51,0x49,0x46,  -  2

                colListA[4] = chars[(indA*5)];      // ?    ?  0x42
                colListA[3] = chars[(indA*5)+1];    // ??    ? 0x61
                colListA[2] = chars[(indA*5)+2];    // ? ?   ? 0x51
                colListA[1] = chars[(indA*5)+3];    // ?  ?  ? 0x49
                colListA[0] = chars[(indA*5)+4];    // ?   ??  0x46

                colListB[4] = chars[(indB*5)];
                colListB[3] = chars[(indB*5)+1];
                colListB[2] = chars[(indB*5)+2];
                colListB[1] = chars[(indB*5)+3];
                colListB[0] = chars[(indB*5)+4];
	
                //Построчный вывод 2 символов одновременно
		for (row=0;row<8;row++)
		{
			if (line < 4)	//Верхняя часть экрана
			{
				lcd_comm(0x80 | (line * 8 + row)); //Отправляем адрес
				lcd_comm(0x80 | count);
			}
			else            //Нижня часть экрана
			{
				lcd_comm(0x80 | ( (line-4) * 8 + row));
				lcd_comm(0x88 | count);
			}

			unsigned char dataA = 0x00;
			for (colInd=0;colInd<5;colInd++)    //Переворачиваем символ на 90 градусов против часовой стрелки
			{
				if (colListA[colInd] & (1 << row))          
				{
					dataA = dataA | (1 << (colInd+3));  //Формируем байт для вывода n-ой строки 1 символа
				}                                           //Добавляем 3 пустых бита для формирования расстояния между символами
			}

			unsigned char dataB = 0x00;
			for (colInd=0;colInd<5;colInd++)
			{
				if (colListB[colInd] & (1 << row))
				{
					dataB = dataB | (1 << (colInd+3));
				}
			}

			lcd_data(dataA);    //Выводим n-ую строку 1 символа
			lcd_data(dataB);    //Выводим n-ую строку 2 символа
		}

		count++;
	}
}

void lcd_putc_big(unsigned char line, unsigned char column, unsigned char ch)
{
        unsigned char row,i,k;

        i=0;
        for (k=line;k<line+4;k++)
        {         
		for (row=0;row<8;row++) //Цикл вывода 8 строк в ячейке 8х8
		{
			if (k < 4)	//Верхняя часть экрана
			{
				lcd_comm(0x80 | (k * 8 + row)); //Отправляем адрес
				lcd_comm(0x80 | column);
			}
			else            //Нижня часть экрана
			{
				lcd_comm(0x80 | ( (k-4) * 8 + row));
				lcd_comm(0x88 | column);
			}


                        switch (ch)
                        {
                            case '0': lcd_data(big0[i]);lcd_data(big0[i+1]); break;
                            case '1': lcd_data(big1[i]);lcd_data(big1[i+1]); break;
                            case '2': lcd_data(big2[i]);lcd_data(big2[i+1]); break;
                            case '3': lcd_data(big3[i]);lcd_data(big3[i+1]); break;
                            case '4': lcd_data(big4[i]);lcd_data(big4[i+1]); break;
                            case '5': lcd_data(big5[i]);lcd_data(big5[i+1]); break;
                            case '6': lcd_data(big6[i]);lcd_data(big6[i+1]); break;
                            case '7': lcd_data(big7[i]);lcd_data(big7[i+1]); break;
                            case '8': lcd_data(big8[i]);lcd_data(big8[i+1]); break;
                            case '9': lcd_data(big9[i]);lcd_data(big9[i+1]); break;
                            case ':': lcd_data(dw[i]);lcd_data(dw[i+1]); break;
                        }

                        i=i+2;
		
                }

	}
}
