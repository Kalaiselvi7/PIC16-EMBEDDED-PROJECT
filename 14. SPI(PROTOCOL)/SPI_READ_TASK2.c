
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
#define RSS1 RC0
#define RSS2 RC1
#define RSS3 RC2
#define RS RD0
#define RW RD1
#define EN RD2
#define LCD PORTB

//SPI function
void SPI_master_init();
void SPI_master_data(char a);
void SPI_page_write1(unsigned char MSB,unsigned char LSB);
void SPI_page_write2(unsigned char MSB,unsigned char LSB);
void SPI_page_write3(unsigned char MSB,unsigned char LSB);
void SPI_read_byte1(unsigned char MSB,unsigned char LSB);
void SPI_read_byte2(unsigned char MSB,unsigned char LSB);
void SPI_read_byte3(unsigned char MSB,unsigned char LSB);

//lcd function
void lcd_init();
void send_cmd(char b);
void send_data(char c);

//function defition for SPI

void SPI_master_init()
{
    SSPCON = 0x20;
    SSPSTAT = 0x00;
}

void SPI_master_data(char a)
{
    SSPBUF = a;
    while(SSPIF == 0);
    SSPIF = 0;
}

void SPI_page_write1(unsigned char MSB,unsigned char LSB)
{
    char str1[20] ="embedded";
    int i;
    RSS1 = 1;
    __delay_ms(1);
    RSS1 = 0;
    
    SPI_master_data(0x06);
    
    RSS1 = 1;
    __delay_ms(1);
    RSS1 = 0;
    
    SPI_master_data(0x02);
    SPI_master_data(MSB);
    SPI_master_data(LSB);
    
    for(i=0;str1[i]!= '\0';i++)
    {
        SPI_master_data(str1[i]);
    }
    
    RSS1 = 1;
    __delay_ms(1);
    RSS1 = 0;
    SPI_master_data(0x04);
    RSS1 = 1;
}

void SPI_page_write2(unsigned char MSB,unsigned char LSB)
{
    char str2[20] ="system";
    int i;
    RSS2 = 1;
    __delay_ms(1);
    RSS2 = 0;
    
    SPI_master_data(0x06);
    
    RSS2 = 1;
    __delay_ms(1);
    RSS2 = 0;
    
    SPI_master_data(0x02);
    SPI_master_data(MSB);
    SPI_master_data(LSB);
    
    for(i=0;str2[i]!= '\0';i++)
    {
        SPI_master_data(str2[i]);
    }
    
    RSS2 = 1;
    __delay_ms(1);
    RSS2 = 0;
    SPI_master_data(0x04);
    RSS2 = 1;
}

void SPI_page_write3(unsigned char MSB,unsigned char LSB)
{
    char str3[30] ="kalaiselvi";
    int i;
    RSS3 = 1;
    __delay_ms(1);
    RSS3 = 0;
    
    SPI_master_data(0x06);
    
    RSS3 = 1;
    __delay_ms(1);
    RSS3 = 0;
    
    SPI_master_data(0x02);
    SPI_master_data(MSB);
    SPI_master_data(LSB);
    
    for(i=0;str3[i]!= '\0';i++)
    {
        SPI_master_data(str3[i]);
    }
    
    RSS3 = 1;
    __delay_ms(1);
    RSS3 = 0;
    SPI_master_data(0x04);
    RSS3 = 1;
}
void SPI_read_byte1(unsigned char MSB,unsigned char LSB)
{
    int i;
    RSS1 = 1;
    __delay_ms(1);
    RSS1 = 0;
    
    SPI_master_data(0x03);
    SPI_master_data(MSB);
    SPI_master_data(LSB);
    
    for(i=0;i<=7;i++)
    {
        char data;
        SPI_master_data(0x00);
        data = SSPBUF;
        send_data(data);
    }
    
    RSS1 = 1;
}
void SPI_read_byte2(unsigned char MSB,unsigned char LSB)
{
    int i;
    RSS2 = 1;
    __delay_ms(1);
    RSS2 = 0;
    
    SPI_master_data(0x03);
    SPI_master_data(MSB);
    SPI_master_data(LSB);
    
    for(i=0;i<=6;i++)
    {
        char data;
        SPI_master_data(0x00);
        data = SSPBUF;
        send_data(data);
    }
    
    RSS2 = 1;
}
void SPI_read_byte3(unsigned char MSB,unsigned char LSB)
{
    int i;
    RSS3 = 1;
    __delay_ms(1);
    RSS3 = 0;
    
    SPI_master_data(0x03);
    SPI_master_data(MSB);
    SPI_master_data(LSB);
    
    for(i=0;i<=9;i++)
    {
        char data;
        SPI_master_data(0x00);
        data = SSPBUF;
        send_data(data);
    }
    
    RSS3 = 1;
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
int main()
{
    TRISC = 0x10;
    TRISB = 0x00;
    TRISD = 0x00;
    
    
    SPI_master_init();
    lcd_init();
   
    SPI_page_write1(0x00,0x28);
    SPI_page_write2(0x00,0x38);
    SPI_page_write3(0x00,0x48);
    __delay_ms(1000);
    
    send_cmd(0x80);
    SPI_read_byte1(0x00,0x28);
    __delay_ms(1000);
    SPI_read_byte2(0x00,0x38);
    __delay_ms(1000);
    
    send_cmd(0xc0);
    SPI_read_byte3(0x00,0x48);
    __delay_ms(1000);
    while(1);
}