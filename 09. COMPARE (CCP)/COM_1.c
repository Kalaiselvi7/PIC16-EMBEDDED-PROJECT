#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#include <xc.h>
#define _XTAL_FREQ 20000000

void compare();
void compare_set();
void compare_reset();

int main()
{
    TRISC = 0x00;
    PORTC = 0x00;
    while(1)
    {
        compare_set();
        __delay_ms(1000);

        compare_reset();
        __delay_ms(1000);
    }
}


void compare_set()
{
    CCP1CON = 0x08;// 0000 1000
    CCPR1 = 1000;
    compare();
}

void compare_reset()
{
    CCP1CON = 0x09;// 0000 1001
    CCPR1 = 6000;
    compare();
}

void compare()
{
    TMR1  = 0;
    T1CON = 0X31;
    while(CCP1IF == 0);
    CCP1IF = 0;
    while(TMR1IF == 0);
    TMR1IF=0;
    TMR1ON=0;
}