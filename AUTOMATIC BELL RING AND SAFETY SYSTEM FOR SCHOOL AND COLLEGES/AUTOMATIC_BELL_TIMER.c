
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

//LCD CONNECTIONS
#define RS RD0
#define RW RD1
#define EN RD2
#define LCD PORTB

//SENSOR DEFINE
#define RAIN_SENSOR RA0
#define LED RD3
#define BUZZER RD5
#define EMERGENCY_SW RD6

//LCD FUNCTIONS
void lcd_cmd(char a);
void lcd_data(char b);
void lcd_init();
void lcd_string(const char *msg);

//GSM AND UART FUNCTIONS
void UART_INIT();
void UART_TX(char ch);
void UART_SendString(const char *msg);
void GSM_send(char *alert);

//I2C 
void I2C_master_init();
void I2C_master_start();
void I2C_master_stop();
void I2C_master_write(unsigned char a);
unsigned char I2C_master_read(unsigned char b);

//RTC FUNCTION
void DS3232_Write(unsigned char reg, unsigned char value);
unsigned char DS3232_Read(unsigned char reg);

void lcd_cmd( char a) {
    LCD = a;
    RS = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_data(char b) {
    LCD = b;
    RS = 1;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_init() 
{
    lcd_cmd(0x38);  // 8-bit mode, 2 lines
    lcd_cmd(0x0C);  // Display ON, Cursor OFF
    lcd_cmd(0x06);  // Auto-increment cursor
    lcd_cmd(0x01);  // Clear display
    __delay_ms(100);
    
}

void lcd_string(const char *msg) 
{
    while (*msg)
    {
        lcd_data(*msg++);
    }
}

void UART_INIT() 
{
    TXSTA = 0x26;  // TX enable
    RCSTA = 0x80;  // Serial Port enable
    SPBRG = 129;    // Baud rate = 9600 for 20MHz
}
void UART_TX(char ch)
{
     TXREG = ch; // Load data into TX register
    while(TXIF == 0);   // Wait until TX buffer empty
    TXIF = 0;
    __delay_ms(100);
}

void UART_SendString(const char *msg)
{
     while(*msg)
        UART_TX(*msg++);
}

void GSM_send(char *msg) 
{
    UART_SendString("AT\r");
    __delay_ms(300);
   UART_SendString("AT+CMGF=1\r");  // Text mode
    __delay_ms(300);
    UART_SendString("AT+CMGS=\"+916381518276\"\r");  // 
    __delay_ms(300);
   UART_SendString(msg);
    UART_TX(26); // CTRL+Z
    __delay_ms(500);
}

void I2C_master_init()
{
    SSPCON=0X28;
    SSPCON2=0X00;
    SSPSTAT=0X00;
    SSPADD=0X19;
    TRISC=0x18;
}
void I2C_master_start()
{
    SEN = 1;
    while(SEN);
}
void I2C_master_stop()
{
     PEN = 1;
    while(PEN);
}
void I2C_master_write(unsigned char a)
{
    SSPBUF = a;
    while(SSPIF==0);
    SSPIF = 0;
} 
unsigned char I2C_master_read(unsigned char b)
{
    unsigned char temp;
    RCEN = 1;
    while(!BF);
    temp  = SSPBUF;
    ACKDT = b & 0x01 ;// Acknowledge
    ACKEN = 1;
    RCEN = 0;
    return temp;
}
void DS3232_Write(unsigned char reg, unsigned char value)
{
    I2C_master_start();
    I2C_master_write(0xD0);   // DS3232 address + write (0x68<<1)
    I2C_master_write(reg);
    I2C_master_write(value);
    I2C_master_stop();
}
unsigned char DS3232_Read(unsigned char reg)
{
    unsigned char data;
    I2C_master_start();
    I2C_master_write(0xD0);
    I2C_master_write(reg);
    I2C_master_start();
    I2C_master_write(0xD1);   // Read mode
    data = I2C_master_read(1);
    I2C_master_stop();
    return data;
}
int main()
{
    TRISA = 0x01; // RA0 input (rain sensor)
    TRISB = 0x00; // LCD
    TRISC = 0x80; // RC7 RX input, RC6 TX output
    TRISD = 0x40; //  RB1 input (button)
    ADCON1 = 0x06; // Disable analog, make RA0 digital
    LED = 0;
    BUZZER = 0;
    
    lcd_init();
    UART_INIT();
    I2C_master_init();
    DS3232_Write(0x00, 0x00);  // sec
    DS3232_Write(0x01, 0x00);  // min
    DS3232_Write(0x02, 0x09);  // hour
    
    
    lcd_cmd(0x01);
    lcd_cmd(0x80);  // Start at first line
    lcd_string("SMART SCHOOL");
    lcd_cmd(0xC0);
    lcd_string("SAFETY SYSTEM");
    __delay_ms(2000);
    
    while(1)
    {
         
        static int rain_sent = 0;
        
        // RAIN DETECTION
        if (RAIN_SENSOR == 1 && rain_sent == 0) 
        {
            
            LED = 1;
            lcd_cmd(0x01);
            lcd_string("RAIN DETECTED");
            GSM_send("Today we have heavy rain so school will be ended on 2pm repectively.");
            __delay_ms(200);
            rain_sent = 1;
            
        } 
        else if (RAIN_SENSOR == 0)
        {
          lcd_cmd(0x01);
          lcd_cmd(0x80);
          lcd_string("RAIN NOT");
          lcd_cmd(0xC0);
          lcd_string("DETECTED");
          //lcd_cmd(0x01);
          rain_sent = 0;   // reset once rain stops
          LED = 0;
         }
        
        // EMERGENCY BUTTON
        if (EMERGENCY_SW == 0) 
        {
            BUZZER = 1;
            lcd_cmd(0x01);
            lcd_string("EMERGENCY ALERT!");
            GSM_send("Emergency in school! Immediate help required.");
            __delay_ms(1000);
            //BUZZER = 0;
            
        }
        else 
        {
            BUZZER = 0;
        }
        __delay_ms(200);
        
       //RTC working
        unsigned char hour = DS3232_Read(0x02); //hour register
        unsigned char min  = DS3232_Read(0x01); //min register
        unsigned char sec  = DS3232_Read(0x00); //sec register
        
        if(hour == 0x14 && min == 0x30 && sec == 0x00)
        { 
            lcd_cmd(0x01);
            lcd_string("9:00:00 AM");
            GSM_send("School Bell Rang at 9 AM");
            RD5 = 1;           // turn ON bell
            __delay_ms(5000);  // 5 seconds ring
            RD5 = 0;           // turn OFF bell
        }
        if(hour == 0x14 && min == 0x32 && sec == 0x00)
        {
            lcd_cmd(0x01);
            lcd_string("1:00:00 PM");
            GSM_send("LUNCH Bell Rang at 1 PM");
            BUZZER = 1;
            RD5 = 1;           // turn ON bell
            __delay_ms(5000);  // 5 seconds ring
            BUZZER = 0;  // 2:00:00 PM
            //LED 0;
        }
        if(hour == 0x14 && min == 0x35 && sec == 0x00)    // 4:00:00 PM
        {
            lcd_cmd(0x01);
            lcd_string("4:00:00 PM");
            GSM_send("SCHOLL END Bell Rang at 4 PM");
            RD5 = 1;           // turn ON bell
            __delay_ms(5000);  // 5 seconds ring
            RD5 = 0;  // 1:00:00 PM
        }
        
    }
    
}