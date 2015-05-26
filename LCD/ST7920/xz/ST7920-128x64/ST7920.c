/****************************************************/
/* ������ ��� ������ � LCD                          */
/* �����:       SypH3r                              */
/* Website:     MCUS.RU                             */
/* �����������: HI-TECH PICC18                      */
/****************************************************/

#include "ST7920.h"
#include <htc.h>
#include "font.h"
#include "string.h"




void lcd_wr_byte(unsigned char command) //��������  �����
{
        DB4_TRIS = 0;
        DB5_TRIS = 0;
        DB6_TRIS = 0;
        DB7_TRIS = 0;
        
        LCD_RW = 0;
	LCD_EN = 1;
	// ������ ������� ��� �����
	LCD_DB4 = (command & (1 << 4)) >> 4;
        LCD_DB5 = (command & (1 << 5)) >> 5;
        LCD_DB6 = (command & (1 << 6)) >> 6;
        LCD_DB7 = (command & (1 << 7)) >> 7;
	
        DelayUs(10);
        LCD_EN = 0;
        DelayUs(10);

	
	LCD_EN = 1;
	// ������ ������� ��� �����
        LCD_DB4 = (command & (1 << 0)) >> 0;
        LCD_DB5 = (command & (1 << 1)) >> 1;
        LCD_DB6 = (command & (1 << 2)) >> 2;
        LCD_DB7 = (command & (1 << 3)) >> 3;
	
        DelayUs(10);
        LCD_EN = 0;

	DelayUs(10);					// ���� ����������� � ��� �� ���������� - ������������ �������� � ���� ���� ���������
}

void lcd_wr_ni_byte(unsigned char command) //�������� �������� �����
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

void lcd_data(unsigned char data) //������ ������
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

void lcd_comm(unsigned char command) //������ �������
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

        lcd_wr_ni_byte(0b00110000); // 4 ������ �����
        DelayMs(5);


        lcd_wr_ni_byte(0b00100000); // 4 ������ �����
        DelayUs(200);


	lcd_comm(0b00100000); // 4 ������ �����
	DelayUs(100);

	lcd_comm(0b00001100); // �������� �������, ��������� ������ � ��� �������
	DelayUs(200);

	lcd_comm(0b00000001); 	  // ������� ������
	DelayMs(15);

	lcd_comm(0x06); 	  // ������ ���������� ������, ��� ������ ������
	DelayUs(200);

	lcd_comm(0b00000010); // ������ ������ �����

        DelayUs(200);

        //�������� ����������� �����
        lcd_comm(0b00100000);//(0x20);
	DelayMs(1);
	lcd_comm(0b00100100);//(0x24);			// ������������� � ����� ����������� ����������
	DelayMs(1);
	lcd_comm(0b00100110);//(0x26);			// �������� ����������� �����
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
			for(x = 0; x < 8; x++)			 //������� ����� ������
			{					 //� ������ ����������� ����������, � � � ���������� ������ ���� ������� ����� ��������� ������
				lcd_comm(0x80 | y);		 // ������������ ����������
                                lcd_comm(0x80 | x);		 // �������������� �� 0 �� 8
				lcd_data(graphic[2*x + 16*y]);	 // ������� ���� ������
				lcd_data(graphic[2*x+1 + 16*y]); // ������� ���� ������
			}
		}
		else
		{
			for(x = 0; x < 8; x++)			 // ��������� ������ ����� ������
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
	// ����� ������ �� ����� ��������� ����� 5x7 �� 16 �������� �� 8 �������,
	// ��� ��� ������ ������ 16 ��� - ��������� 2 ������� �� 1 ������ �����
	unsigned char row;
	unsigned char colInd;
	unsigned char count = 0;
        unsigned char next = 1;

	while(*str && next)
	{
		unsigned char letterA = *str++; //���� ����� ������ �� ������ ��������� ������ ��������
		if (letterA == 0) {letterA=32; next=0;}

		unsigned char letterB = *str++;
		if (letterB == 0) {letterB=32; next=0;}


		unsigned char indA = letterA - 0x20; //�������� ������������ ������ � ������� chars
		unsigned char indB = letterB - 0x20;
		unsigned char colListA[5];
		unsigned char colListB[5];

                //������ ������ � ������� chars � ��������� ������ colListA
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
	
                //���������� ����� 2 �������� ������������
		for (row=0;row<8;row++)
		{
			if (line < 4)	//������� ����� ������
			{
				lcd_comm(0x80 | (line * 8 + row)); //���������� �����
				lcd_comm(0x80 | count);
			}
			else            //����� ����� ������
			{
				lcd_comm(0x80 | ( (line-4) * 8 + row));
				lcd_comm(0x88 | count);
			}

			unsigned char dataA = 0x00;
			for (colInd=0;colInd<5;colInd++)    //�������������� ������ �� 90 �������� ������ ������� �������
			{
				if (colListA[colInd] & (1 << row))          
				{
					dataA = dataA | (1 << (colInd+3));  //��������� ���� ��� ������ n-�� ������ 1 �������
				}                                           //��������� 3 ������ ���� ��� ������������ ���������� ����� ���������
			}

			unsigned char dataB = 0x00;
			for (colInd=0;colInd<5;colInd++)
			{
				if (colListB[colInd] & (1 << row))
				{
					dataB = dataB | (1 << (colInd+3));
				}
			}

			lcd_data(dataA);    //������� n-�� ������ 1 �������
			lcd_data(dataB);    //������� n-�� ������ 2 �������
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
		for (row=0;row<8;row++) //���� ������ 8 ����� � ������ 8�8
		{
			if (k < 4)	//������� ����� ������
			{
				lcd_comm(0x80 | (k * 8 + row)); //���������� �����
				lcd_comm(0x80 | column);
			}
			else            //����� ����� ������
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
