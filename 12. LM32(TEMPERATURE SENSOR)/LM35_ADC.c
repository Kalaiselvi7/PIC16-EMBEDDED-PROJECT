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
#include<stdio.h>
#define _XTAL_FREQ 20000000
#define lcd PORTB
#define RS RC0
#define RW RC1
#define EN RC2
void lcd_init();
void send_cmd(char a);
void send_data(char b);
void show_data(char *s);
void ADC_init();
void ADC_start();
float result;
int temperature;
int main()
{
    TRISB=0X00;
    TRISC=0X00;
    TRISA=0x01;
    lcd_init();
    ADC_init();
    send_cmd(0x80);
    show_data("TEMPERATURE :");
    while(1)
    {
        send_cmd(0xc0);
        ADC_start();
	}
}
void lcd_init()
{
	
	send_cmd(0x38);
	send_cmd(0x0e);
	send_cmd(0x01);
	send_cmd(0x06);
	
}
void send_cmd(char a)
{
	lcd=a;
	RS=0;
	RW=0;
	EN=1;
	__delay_ms(1);
	EN=0;
}
void send_data(char b)
{
	lcd=b;
	RS=1;
	RW=0;
	EN=1;
	__delay_ms(100);
	EN=0;
}
void show_data(char *s)
{
	while(*s)
	{
		send_data(*s++);
	}
}
void ADC_init()
{
    ADCON0=0X81;
    ADCON1=0X80;
}
void ADC_start()
{
    char d[50];
   
    GO_DONE=1;
    while(GO_DONE==1);
    result=ADRESL | (ADRESH<<8);
    temperature=(result*500)/1023;
    sprintf(d,"%02u",temperature);
    show_data(d);
    show_data(" Celsius");
}