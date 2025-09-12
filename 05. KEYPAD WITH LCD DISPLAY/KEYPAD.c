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
#define LCD PORTB
#define RS RD0
#define RW RD1
#define EN RD2

#define R1 RC0
#define R2 RC1
#define R3 RC2
#define R4 RC3
#define C1 RC4
#define C2 RC5
#define C3 RC6
#define C4 RC7

void send_cmd(char a);
void send_data(char b);
void display_string(char *msg);
void lcd_init();

void KEYPAD();
void row1();
void row2();
void row3();
void row4();

int main()
{
    LCD = 0X00;
    TRISB = 0x00;
    TRISC = 0xF0;
    TRISD = 0X00;
    lcd_init();
    while(1)
    {
        //send_cmd(0x01);
        send_cmd(0x80);
		display_string("Enter the number");
		__delay_ms(10);
		send_cmd(0xc0);
		KEYPAD();
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
    LCD = a;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(10);
    EN = 0;
}
void send_data(char b)
{
    LCD = b;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(10);
    EN = 0;
}
void display_string(char *msg)
{
    while(*msg)
    {
        send_data(*msg++);
    }
}

void KEYPAD()
{
    C1 = C2 = C3= C4 =0;
    row1();
    row2();
    row3();
    row4();
}
void row1()
{
    R1=0;
    R2=R3=R4 = 1;
    if(C1 == 0)
    {
        send_data('7');
        while(C1==0);
    }
     else if(C2 == 0)
	 {
		 send_data('8');
		 while(C2 == 0);
	 }
	 else if(C3 == 0)
	 {
		 send_data('9');
		 while(C3 == 0);
	 }
	 else if(C4 == 0)
	 {
		 send_data('/');
		 while(C4==0);
	 }
 }
	 
void row2()
{
	 R1=R3=R4 =1;
	 R2 =0;
	 if(C1 == 0)
	 {
		 send_data('4');
		 while(C1==R2);
		 }
	 else if(C2 == R2)
	 {
		 send_data('5');
		 while(C2 == R2);
	 }
	 else if(C3 == R2)
	 {
		 send_data('6');
		 while(C3 == R2);
	 }
	 else if(C4 == R2)
	 {
		 send_data('*');
		 while(C4==R2);
	 }
 }
void row3()
{
	 R2=R1=R4 =1;
	 R3 =0;
	 if(C1 == R3)
	 {
		 send_data('1');
		 while(C1==R3);
		 }
	 else if(C2 == R3)
	 {
		 send_data('2');
		 //while(C2 == R3);
	 }
	 else if(C3 == R3)
	 {
		 send_data('3');
		 while(C3 == R3);
	 }
	 else if(C4 == R3)
	 {
		 send_data('-');
		 while(C4==R3);
	 }
 }
void row4()
{
	 R2=R3=R1 =1;
	 R4 =0;
	 if(C1 == R4)
	 {
		 send_cmd(0x01);
		 while(C1==R4);
		 }
	 else if(C2 == R4)
	 {
		 send_data('0');
		 while(C2 == R4);
	 }
	 else if(C3 == R4)
	 {
		 send_data('=');
		 while(C3 == R4);
	 }
	 else if(C4 == R4)
	 {
		 send_data('+');
		 while(C4==R4);
	 }
 }
 





