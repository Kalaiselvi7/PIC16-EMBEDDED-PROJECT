
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
#include <stdio.h>
#define _XTAL_FREQ 20000000
#define RSS RC0
//lcd pins
#define RS RD0
#define RW RD1
#define EN RD2
#define LCD PORTB

//SPI function
void SPI_master_init();
void SPI_master_write(unsigned char a);
unsigned char SPI_master_read();
//TC72 function
void TC72_init();
unsigned char TC72_read_temperature();


//lcd function
void lcd_init();
void send_cmd(char b);
void send_data(char c);
void display_data(char* ptr);

//function defition for SPI

void SPI_master_init()
{
    SSPCON = 0x20;
    SSPSTAT = 0x00;
}

void SPI_master_write(unsigned char a)
{
    SSPBUF = a;
    while(SSPIF == 0);
    SSPIF = 0;
}

unsigned char SPI_master_read()
{
     SSPBUF = 0x00 ; //dummy address
    while(SSPIF == 0);
    SSPIF = 0;
    return SSPBUF;
}
void TC72_init()
{
    RSS = 0;
    __delay_ms(1);
    RSS = 1;
    
    SPI_master_write(0x80); //write op
    SPI_master_write(0x00); //control byte
    
    RSS = 0;
}
unsigned char TC72_read_temperature()
{
    unsigned char temp;
    RSS = 0;
    __delay_ms(1);
    RSS = 1;
    
     SPI_master_write(0x02);
     temp = SPI_master_read();
     RSS = 0;
     return temp;
  
}

void lcd_init()
{  
	send_cmd(0x38);
	send_cmd(0x0e);
	send_cmd(0x01);
	send_cmd(0x06);
}
void send_cmd(char b)
{
    LCD = b;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}
void send_data(char c)
{
    LCD = c;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}
void display_data(char* ptr)
{
    while(*ptr)
    {
        send_data(*ptr++);
    }
}

int main()
{
    int result;
    char temp[5];
    
    TRISC = 0x10;
    TRISB =0x00;
    TRISD = 0x00;
    lcd_init();
   while(1)
   {
        
    SPI_master_init();
    TC72_init();
    
    send_cmd(0x80);
   
    //send_cmd(0x06);
    display_data("Received temp: ");
     __delay_ms(100);
    
    result = TC72_read_temperature();
    sprintf(temp, "%d",result);
    send_cmd(0xc0);
    display_data(temp);1
    }
}