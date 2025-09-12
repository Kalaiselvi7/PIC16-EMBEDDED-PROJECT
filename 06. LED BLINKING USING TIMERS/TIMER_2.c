// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000
#define LED PORTB
void delay(int ms);
int main()
{
    TRISB = 0x00;
    LED = 0x00;
    while(1)
    {
        LED = 0xff;
        delay(1000);
        LED = 0x00;
        delay(1000);
    }
}
void delay(int ms)
{
    int i;
    TMR2 = 0;
    T2CON = 0x7F;
    PR2 = 40;
    for(i=0;i<ms;i++)
    {
        while(TMR2IF == 0);
        TMR2IF = 0;
    }
    TMR2ON = 0;
}