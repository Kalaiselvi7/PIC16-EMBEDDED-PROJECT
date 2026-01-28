
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
#define CS1 RC0

void SPI_init();
void master_data(char a);
void page_write();
void byte_write();

void SPI_init()
{
    SSPSTAT = 0x00;
    SSPCON = 0x20;
}

void master_data(char a)
{
    SSPBUF = a;
    while(SSPIF == 0);
    SSPIF = 0 ;
    __delay_ms(10);
}

void byte_write()
{
    CS1 = 1;
    __delay_ms(10);
    CS1 = 0; 
    
    master_data(0x06); //write enable
    
    CS1 = 1;
    __delay_ms(10);
    CS1 = 0; 
    
    master_data(0x02); //instruction bit
    master_data(0x00); //address low
    master_data(0x00); //address high
    master_data('K');  //data send
    
    CS1 = 1;
    __delay_ms(10);
    CS1 = 0; 
    
    master_data(0x04);
    
    CS1 = 1;
 }

void page_write()
{
    int i;
    CS1 = 1;
    __delay_ms(1);
    CS1 = 0;
    
   master_data(0x06);

    CS1     = 1;
    __delay_ms(10);
    CS1     = 0;

    master_data(0x02);
    master_data(0x00);
    master_data(0xC8);
    
     for(i = 'a'; i <= 'z'; i++)  //string send
    {
        master_data(i);
    }
    
    CS1     = 1;
    __delay_ms(10);
    CS1     = 0;

    master_data(0x04); // disable write
    CS1     = 1;
}

int main()
{
    TRISC = 0x10;
     SPI_init();
     
     byte_write();
    __delay_ms(1000);
    
     page_write();
    __delay_ms(1000);
    while(1);
     
}


