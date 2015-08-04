#include <avr/io.h>
#include <avrlibtypes.h>
#include <avrlibdefs.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <LCD_wg128.h>

int main()
{
LCD_INIT();

DEBUG_UP;
LCD_putc(2,20,"Di Halt");
LCD_putc(0,1,"EasyElectronics.ru");
DEBUG_DN;

return 0;
}
