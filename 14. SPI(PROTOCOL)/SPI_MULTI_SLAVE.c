
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
#define SS1 RC0
#define SS2 RC1
#define SS3 RC2

void SPI_INIT();
void master_data(char a);
void page_write1();
void page_write2();
void page_write3();

void SPI_INIT()
{
    SSPCON = 0x20;
    SSPSTAT = 0x00;
}

void master_data(char a)
{
    SSPBUF = a;
    while(SSPIF == 0);
    SSPIF = 0;
    __delay_ms(10);
}

void page_write1()
{
    int i;
    char str1[20] = "embedded" ;
    
    SS1 = 1;
    __delay_ms(1);
    SS1 = 0;
    
    master_data(0x06);
    
    SS1 = 1;
    __delay_ms(1);
    SS1 = 0;
    
    master_data(0x02);
    master_data(0x00);
    master_data(0x00);
    
    for(i=0;str1[i]!='\0';i++)
    {
        master_data(str1[i]);
    }
    
     SS1     = 1;
    __delay_ms(2);
    SS1     = 0;
    
    master_data(0x04); // disable write
    SS1   = 1;
}

void page_write2()
{
    int i;
    char str2[20] = "system";
    
    SS2 = 1;
    __delay_ms(1);
    SS2 = 0;
    
    master_data(0x06);
    
    SS2 = 1;
    __delay_ms(1);
    SS2 = 0;
    
    master_data(0x02);
    master_data(0x00);
    master_data(0x28);
    
    for(i=0;str2[i]!='\0';i++)
    {
        master_data(str2[i]);
    }
    
     SS2     = 1;
    __delay_ms(2);
    SS2     = 0;
    
    master_data(0x04); // disable write
    SS2   = 1;
    
}

void page_write3()
{
    int i;
    char str3[20] = "kalai"; 
    
    SS3 = 1;
    __delay_ms(1);
    SS3 = 0;
    
    master_data(0x06);
    
    SS3 = 1;
    __delay_ms(1);
    SS3 = 0;
    
    master_data(0x02);
    master_data(0x00);
    master_data(0x38);
    
    for(i=0;str3[i]!='\0';i++)
    {
        master_data(str3[i]);
    }
    
     SS3     = 1;
    __delay_ms(2);
    SS3     = 0;
    
    master_data(0x04); // disable write
    SS3   = 1;
}

int main()
{
    TRISC = 0x10;
    SPI_INIT();
    
    page_write1();
    __delay_ms(10);
    
    page_write2();
     __delay_ms(10);
     
    page_write3();
     __delay_ms(10);
     
     while(1);
    
}
