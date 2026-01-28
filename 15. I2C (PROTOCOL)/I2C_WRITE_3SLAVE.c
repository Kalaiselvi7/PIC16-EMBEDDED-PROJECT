
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
//function for I2C
void I2C_master_init();
void I2C_master_wait(); //this is for every operation in I2C master needs to wait until next operation.
void I2C_master_start();
void I2C_master_write(unsigned char a);
void I2C_master_stop();

int main()
{
    char str[20] = "embedded" ;
    char str2[20] ="kalaiselvi";
    int i;
    I2C_master_init();
    I2C_master_start();
    I2C_master_write(0xa0);
    I2C_master_write(0x00);
    I2C_master_write(0x28);
    I2C_master_write('k');
    I2C_master_stop();
    
    __delay_ms(1000);
    
    I2C_master_init();
    I2C_master_start();
    I2C_master_write(0xae);
    I2C_master_write(0x00);
    I2C_master_write(0x38);
    
    for(i=0;i<=7;i++)
    {
        I2C_master_write(str[i]);
    }
    I2C_master_stop();
    
    __delay_ms(1000);
    
    I2C_master_init();
    I2C_master_start();
    I2C_master_write(0xaa);
    I2C_master_write(0x00);
    I2C_master_write(0x48);
    
    for(i=0;i<=9;i++)
    {
        I2C_master_write(str2[i]);
    }
    I2C_master_stop();
             
}
void I2C_master_init()
{
    SSPCON = 0x28;
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = 0x19;
    TRISC=0X18;
}
void I2C_master_wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  //Waits until the I2C module is ready for the next operation.
  //     0X00 & 0X04     ||    0X00 & 0X1F
  //     ( 0000 0000 & 0000 0100 ) || ( 0000 0000 & 0001 1111 )
}
void I2C_master_start()
{
    I2C_master_wait();
    SEN = 1;
}
void I2C_master_write(unsigned char a)
{
    I2C_master_wait();
    SSPBUF = a;
}
void I2C_master_stop()
{
    I2C_master_wait();
    PEN = 1;
}
