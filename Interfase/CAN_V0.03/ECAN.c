/*==============================================================================
 *            ---2014-2015---
 *         Catcatcat electronics
 * Геннадий Чернов
 * Библиотека для работы с шиной CAN шиной для PIC-контроллеров 18 серии
 * 2014 - июнь
 * Версии
 * V-0.01 передача только стандартные сообщения
 * V-0.02 - исправлена ошибка приема, загрузки данных в буфер приема
 * V-0.03 - исправлена ошибка приема, загрузки данных в буфер приема
 *==============================================================================
 */



//------------------------------------------------------------------------------
/*Включания*/
#include <xc.h>  
#include "ECAN.h"




//------------------------------------------------------------------------------
/*Определения*/
//------------------------------------------------------------------------------
/* ECAN задание скорости передачи записать параметр в константу F_ECAN
* параметр  скорость @ 64MHz
*   59        33 333
*   41        47 619
*   39        50 000
*   23        83 333
*   20        95 238
*   19       100 000
*   15       125 000
*    9       200 000
*    7       250 000
*    4       400 000
*    3       500 000
*    1      1000 000
*              параметр*/
//#define F_ECAN    15                 //






/*Функции*/
//------------------------------------------------------------------------------
/*автонастройка на скорости передачи*/
void autospeed (void)
{
    if(PIR5bits.IRXIF)
    {
        if (Speed==1)Speed=3;
        else if (Speed==3)Speed=4;
        else if (Speed==4)Speed=7;
        else if (Speed==7)Speed=15;
        else if (Speed==15)Speed=19;
        else if (Speed==19)Speed=20;
        else if (Speed==20)Speed=23;
        else if (Speed==23)Speed=39;
        else if (Speed==39)Speed=41;
        else if (Speed==41)Speed=59;
        else Speed=1;
    // настройка предделителя и ширины TQ синхронизации

        // Настроить CAN модуль в режим конфигурации
        CANCON = 0x80;                              // REQOP<2:0>=100
        while(!(CANSTATbits.OPMODE ==0x04));        // ожидать переключения в режим конфигурации

        BRGCON1bits.BRP = Speed;

//        // только чтение
//        CANCONbits.REQOP = 0b011;
//        while(!(CANSTATbits.OPMODE == 0b011));
    // Переключить CAN модуль в нормальный режим
        CANCONbits.REQOP = 0b000;
        while(!(CANSTATbits.OPMODE == 0b000));
        PIR5bits.IRXIF=0;
    }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/*Инициализация CAN, предварительно необходимо в регисре TRIS соотествующие
 * порты установить на прием
 * freq - тактовая частота шины CAN
 * filter1**filter5 - фильтер
 */
void Init_CAN(unsigned int freq, unsigned int filter1, unsigned int filter2, unsigned int filter3, unsigned int filter4, unsigned int filter5, unsigned int filter6 )
{
    // Настроить CAN модуль в режим конфигурации
//    CANCON = 0x80;                              // REQOP<2:0>=100
//    while(!(CANSTATbits.OPMODE ==0x04));        // ожидать переключения в режим конфигурации
    CANCONbits.REQOP = 0b100;
    while(!(CANSTATbits.OPMODE == 0b100));
    // Переключить CAN модуль в Режим 0
    ECANCON = 0x00;
    
    // Инициализация CAN выбор частоты шины для тактовой частоты 64 000 000 Гц
    // настройка предделителя и ширины TQ синхронизации
        switch (freq) 
        {
            case 33://59        33 333
              BRGCON1 = 59;
              break;
            case 47://41        47 619
              BRGCON1 = 41;
              break;
            case 50://39        50 000
              BRGCON1 = 39;
              break;
            case 83://23        83 333
              BRGCON1 = 23;
              break;
            case 95://20        95 238
              BRGCON1 = 20;              
              break;
            case 100://19       100 000
              BRGCON1 = 19;              
              break;
            case 125://15       125 000
              BRGCON1 = 15;
              break;
            case 200://9       200 000
              BRGCON1 = 9;
              break;
            case 250://7       250 000
              BRGCON1 = 7;
              break;
            case 400://4       400 000
              BRGCON1 = 4;
              break;
            case 500://3       500 000
              BRGCON1 = 3;
              break;
            case 1000://1      1000 000
              BRGCON1 = 1;
              break;
            default:  //1      1000 000
              BRGCON1 = 1;
              break;
        }

//        BRGCON1 = 0b00000000+freq;
/*                  10010111
 *                  ||++++++------- BRP<5:0> настройка предделителя 19 TQ(us) = (2*(19+1))/64 MHz=0.625
 *  Sync Jump Width ++------------- SJW<1:0> длительность изменения фазового сегмента 3хTQ
 */
        BRGCON2 = 0b10111000;       //0b10111000;
/*                  10111000
 * Prop_Seg         |||||+++------- PRSEG<2:0> 000 = время распространения = 1xTQ "Сегмент распространения"
 * Phase_Seg1       ||+++---------- SEG1PH<2:0> 111 = Время Фазы Сегмент1 = 8xTQ "Фазовые сегмент1"
 *                  |+------------- SAM 0 = Линия шины оцифровывается раз в точке пробы
 *                  +-------------- SEG2PHTS 1 = Свободно программируемый
 */
//        BRGCON3 = 0x05; //0000 0101     //PS2  6TQ
        BRGCON3 = 0b01000101;
/*                  00000101
 * Phase_Seg2       |||||+++------- SEG2PH<2:0>: 101 = Phase Segment2 time = 6xTQ "Фазовые сегмент2"
 *                  ||+++---------- Unimplemented:
 *                  |+------------- WAKFIL:0 = фильтр шины CAN используется для пробуждения
 *                  +-------------- WAKDIS:0 = функция пробуждения по активность шины CAN включена
 * Бит 5 WAKDIS: бит отключения пробуждения
 * 1 = Пробуждение по активности шины CAN отключена
 * 0 = Пробуждение по активности шины CAN включена
 * Бит 6 WAKFIL: Выбор CAN шины сетевой фильтр для Wake-вверх бита
 * 1 = Использовать CAN сетевой фильтр шины для пробуждения
 * 0 = сетевой фильтр CAN шины не используется для пробуждения
 */
/* Требование к программрованию временных интервалов
 * • Prop_Seg + Phase_Seg 1 >= Phase_Seg 2
 * • Phase_Seg 2 >= Sync Jump Width.
 */
 /*Nominal Bit Time = TQ * (Sync_Seg + Prop_Seg + Phase_Seg1 + Phase_Seg2)*/
 /*Nominal Bit Time = TQ * (Sync_Seg=1 + 1 + 8 + 6)=16*/
/*Регистры приемного буфера*/
    // Управляющий регистр приемного буфера 0
    // управляющий регистр приемного буфера 0
    RXB0CON = 0b00000100;
/*              |||||||+------ FILHIT0: активный приемный фильтр 0 = фильтр 0 (RXIF0)
 *              ||||||+------- JTOFF: 0 = смещение таблицы между 1 и 0
 *              |||||+-------- RX0DBEN: 1 = если буфер0 переполнен данные будут записаны в буфер1
 *              ||||+--------- RXRTRRO: Флаг принятия удаленного запроса
 *              |||+---------- Не используется: читается как ‘0’
 *              |++----------- RXM1:RXM0: Режим работы приемного буфера 00 = принимать сообщения без ошибок
 *              +------------- RXFULL:Флаг статус буфера
 *
 */
    //управляющий регистр приемного буфера 1
    RXB1CON = 0b00000000;
/*              |||||+++------ FILHIT2:FILHIT0: активный приемный фильтр 010 = фильтр 2 (RXIF2)
 *              ||||+--------- RXRTRRO: Флаг принятия удаленного запроса
 *              |||+---------- Не используется: читается как ‘0’
 *              |++----------- RXM1:RXM0: Режим работы приемного буфера 00 = принимать сообщения без ошибок
 *              +------------- RXFULL:Флаг статус буфера
 *
 */
    // Инициализация маски приемника
    // Буфер 0
    /*если бит маски 0 принимаются любые ID, если единица, то только те которые совпадают с фильтром*/
    RXM0SIDH = 0xFF;    // Маска для стандартного сообщения
    RXM0SIDL = 0xE0;    // пропускать дополнительные биты
//    RXM0SIDH = 0x00;    // Маска для стандартного сообщения
//    RXM0SIDL = 0x00;    // пропускать все сообщения

    RXM0EIDH = 0xFF;    // Маска для расширенных сообщений
    RXM0EIDL = 0xFF;    //
    
    // Буфер 1
    RXM1SIDH = 0xFF;    // Маска для стандартного сообщения
    RXM1SIDL = 0xE0;
    RXM1EIDH = 0xFF;    // Маска для расширенного соощения
    RXM1EIDL = 0xFF;
    
    // Фильтры сообщений
/* Настройка на прием только одного адреса с использование буферов 1 и 2*/
    //  Filter 0
    RXF0SIDH = filter1>>3;
    RXF0SIDL = filter1<<5;
    RXF0EIDH = 0x00;
    RXF0EIDL = 0x00;
    //  Filter 1
    RXF1SIDH = filter2>>3;
    RXF1SIDL = filter2<<5;
    RXF1EIDH = 0;
    RXF1EIDL = 0;
    //  Filter 2
    RXF2SIDH = filter3>>3;
    RXF2SIDL = filter3<<5;
    RXF2EIDH = 0x00;
    RXF2EIDL = 0x00;
    //  Filter 3
    RXF3SIDH = filter4>>3;
    RXF3SIDL = filter4<<5;
    RXF3EIDH = 0;
    RXF3EIDL = 0;
    //  Filter 4
    RXF4SIDH = filter5>>3;
    RXF4SIDL = filter5<<5;
    RXF4EIDH = 0;
    RXF4EIDL = 0;
    //  Filter 5
    RXF5SIDH = filter6>>3;
    RXF5SIDL = filter6<<5;
    RXF5EIDH = 0;
    RXF5EIDL = 0;
    
    // Переключить CAN модуль в Повторение)
//    CANCONbits.REQOP = 0b010;
//    while(!(CANSTATbits.OPMODE == 0b010));

    // Переключить CAN модуль в нормальный режим работы
    CANCONbits.REQOP = 0b000;
    while(!(CANSTATbits.OPMODE == 0b000));
    
// только слушать
//    CANCONbits.REQOP = 0b011;
//    while(!(CANSTATbits.OPMODE == 0b011));

    CIOCONbits.CANCAP=1; // разрешить прием
// разрешить прерывания по буферу 0 и 1
    PIE5bits.RXB0IE = 1;
    PIE5bits.RXB1IE = 1;

    PIE5bits.WAKIE = 1; // разрешить прерывание по активности шины
}

//------------------------------------------------------------------------------
/*Функция приема стандартных собщений. Поместить в функцию прерывания */
void ECAN_Receive(void)
{
    if(RXB0IF||RXB1IF)
    {
        if (RXB0CONbits.RXFUL) //
        {
            if(!FlagCAN.PSo)
            {
                RXBUFER[0]  = RXB0SIDH>>5;              // стандартный идендификатор (в старшем байте должно быть 3 бита)
                RXBUFER[1]  = RXB0SIDH<<3;              // младшие биты загружаем в младший байт
                RXBUFER[1]  = RXBUFER[1] | RXB0SIDL>>5; // добавим три бита из регистра RXB0SIDL
                RXBUFER[2]  = RXB0SIDL & 0b00011111;
                RXBUFER[3]  = RXB0EIDH;                 // загружаем значение расширенного идентификатора
                RXBUFER[4]  = RXB0EIDL;                 //

                RXBUFER[5]  = RXB0DLC & 0b00001111;     // загружаем значение длины принятых байт
                RXBUFER[6]  = RXB0D0;
                RXBUFER[7]  = RXB0D1;
                RXBUFER[8]  = RXB0D2;
                RXBUFER[9]  = RXB0D3;
                RXBUFER[10] = RXB0D4;
                RXBUFER[11] = RXB0D5;
                RXBUFER[12] = RXB0D6;
                RXBUFER[13] = RXB0D7;
            }

            RXB0CONbits.RXFUL = 0;      // сбросить флаг буфер полон
            RXB0IF = 0;                 // сбросить флаг прерывания

            FlagCAN.PSo = 1;            // сообщение получено
            FlagCAN.IndCAN = 1;         // флаг индикации активности шины CAN
        }
        else if (RXB1CONbits.RXFUL) //CheckRXB1
        {
            if(!FlagCAN.PSo)
            {
                RXBUFER[0]  = RXB1SIDH>>5;              // стандартный идендификатор (в старшем байте должно быть 3 бита)
                RXBUFER[1]  = RXB1SIDH<<3;              // младшие биты загружаем в младший байт
                RXBUFER[1]  = RXBUFER[1] | RXB1SIDL>>5; // добавим три бита из регистра RXB0SIDL
                RXBUFER[2]  = RXB1SIDL & 0b00011111;
                RXBUFER[3]  = RXB1EIDH;                 // загружаем значение расширенного идентификатора
                RXBUFER[4]  = RXB1EIDL;                 //

                RXBUFER[5]  = RXB1DLC & 0b00001111;     // загружаем значение принятых байт
                RXBUFER[6]  = RXB1D0;
                RXBUFER[7]  = RXB1D1;
                RXBUFER[8]  = RXB1D2;
                RXBUFER[9]  = RXB1D3;
                RXBUFER[10] = RXB1D4;
                RXBUFER[11] = RXB1D5;
                RXBUFER[12] = RXB1D6;
                RXBUFER[13] = RXB1D7;
            }

            RXB1CONbits.RXFUL = 0;      // сбросить флаг буфер полон
            RXB1IF = 0;                 // сбросить флаг прерывания

            FlagCAN.PSo = 1;            // сообщение получено
            FlagCAN.IndCAN = 1;         // флаг индикации активности шины CAN
        }
    }
}//-----------------------------------------------------------------------------







//------------------------------------------------------------------------------
// Функции передачи сообщений
//==============================================================================
/* Передача сообщения стандартного сообщения, записанного в массиве
 * через буфер 0 с 0 приоритетом
 * RTR - 0 нормальное сообщение, 1 - дистанционный запрос
 * str - массив константа сообщения IDS Con DATA
 */
void CANS_Transmit_TXB0(const char *str)
{
    while(TXB0CONbits.TXREQ); // ожидать окончания предыдущей передачи (если она была)
    //IDS загрузка
    TXB0SIDH = str[0]<<5;           // загрузка млоадших 3 бит в старшие биты
    TXB0SIDH = TXB0SIDH|str[1]>>3;  // загрука 5 бит в ладшие биты буфера и получение 8 бит интентификатора для передачи
    TXB0SIDL = str[1]<<5;           // загрузка 3 бит ID в старшие биты регистра TXB0SIDL
    // количество передаваемых байт
    TXB0DLC = str[2];
//    // флаг дистанционного запроса
//    if(RTR)
//    {
//        TXB0DLC |= 0x01000000;
//    }
    // данные
    TXB0D0 = str[3];   // байт 1
    TXB0D1 = str[4];
    TXB0D2 = str[5];
    TXB0D3 = str[6];
    TXB0D4 = str[7];
    TXB0D5 = str[8];
    TXB0D6 = str[9];
    TXB0D7 = str[10];  // байт 8

    TXB0CONbits.TXPRI = 0;      // приоритет 0
    TXB0CONbits.TXREQ = 1;      // передача буфера
}

//------------------------------------------------------------------------------
/*расширенные функции*/

// установка расширенного ID сообщения
// стандартный идентификатор - расширенный идентификатор
void CAN_EID(int SIDTX, int EIDTX)
{
    TXBUID[0] = SIDTX>>5;
    TXBUID[1] = SIDTX&0b00000011;
    TXBUID[1] |= SIDTX<<3&0b11100000;
    TXBUID[1] |= 0b00001000;

    TXBUID[2]=EIDTX>>8;
    TXBUID[3]=EIDTX;
}

// установка стандартного ID сообщения
void CAN_SID(int SIDTX)
{
    TXBUID[0] = SIDTX>>3;
    TXBUID[1] = SIDTX<<5;
}

// установка CON количество байт передачи и флаг дистанционного запроса
// RTR - флаг дистанционного запроса
// COU - количество байт передачи
void CAN_RTR_COU(char RTR, char COU)
{
    TXBUID[4] = COU;
    // флаг дистанционного запроса
    if(RTR)
    {
        TXBUID[4] |= 0x01000000;
    }
}

// передача сообщения
// TXB выбор буфера 0-2
// PRI задание приоритета передачи 0-3
void CAN_Transmit(char TXB, char PRI)
{
    if(!TXB)
    {
        while(TXB0CONbits.TXREQ);   // ожидать окончания предыдущей передачи (если она была)

        TXB0SIDH = TXBUID[0];
        TXB0SIDL = TXBUID[1];
        TXB0EIDH = TXBUID[2];
        TXB0EIDL = TXBUID[3];
        TXB0DLC = TXBUID[4];

        TXB0D0 = TXBUFER[0];
        TXB0D1 = TXBUFER[1];
        TXB0D2 = TXBUFER[2];
        TXB0D3 = TXBUFER[3];
        TXB0D4 = TXBUFER[4];
        TXB0D5 = TXBUFER[5];
        TXB0D6 = TXBUFER[6];
        TXB0D7 = TXBUFER[7];

        TXB0CONbits.TXPRI = PRI;
        TXB0CONbits.TXREQ = 1;      //передача буфера
    }
    else if(TXB==1)
    {
        while(TXB1CONbits.TXREQ);   // ожидать окончания предыдущей передачи (если она была)

        TXB1SIDH = TXBUID[0];
        TXB1SIDL = TXBUID[1];
        TXB1EIDH = TXBUID[2];
        TXB1EIDL = TXBUID[3];
        TXB1DLC = TXBUID[4];

        TXB1D0 = TXBUFER[0];
        TXB1D1 = TXBUFER[1];
        TXB1D2 = TXBUFER[2];
        TXB1D3 = TXBUFER[3];
        TXB1D4 = TXBUFER[4];
        TXB1D5 = TXBUFER[5];
        TXB1D6 = TXBUFER[6];
        TXB1D7 = TXBUFER[7];

        TXB1CONbits.TXPRI = PRI;
        TXB1CONbits.TXREQ = 1;      //передача буфера
    }
    else if(TXB==2)
    {
        while(TXB2CONbits.TXREQ);   // ожидать окончания предыдущей передачи (если она была)

        TXB2SIDH = TXBUID[0];
        TXB2SIDL = TXBUID[1];
        TXB2EIDH = TXBUID[2];
        TXB2EIDL = TXBUID[3];
        TXB2DLC = TXBUID[4];

        TXB2D0 = TXBUFER[0];
        TXB2D1 = TXBUFER[1];
        TXB2D2 = TXBUFER[2];
        TXB2D3 = TXBUFER[3];
        TXB2D4 = TXBUFER[4];
        TXB2D5 = TXBUFER[5];
        TXB2D6 = TXBUFER[6];
        TXB2D7 = TXBUFER[7];

        TXB2CONbits.TXPRI = PRI;
        TXB2CONbits.TXREQ = 1;      //передача буфера
    }

}