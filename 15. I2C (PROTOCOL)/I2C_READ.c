#include <xc.h>
#define _XTAL_FREQ 20000000
#define LCD PORTB
#define RS RD0
#define RW RD1
#define EN RD2
void I2C_master_init();
void I2C_master_wait();
void I2C_master_start();
void I2C_master_repeated_start();
void I2C_master_stop();
void I2C_master_write(unsigned char a);
unsigned char I2C_master_read(unsigned char b);
void lcd_init();
void send_cmd(char c);
void send_data(char d);
void show_data(char *ptr);
unsigned char data;
int main()
{
    unsigned char str[20] = "embedded ";
    unsigned char str1[20] = "kalai";
    LCD=0x00;
    TRISB=0X00;
    TRISD=0X00;
    unsigned int i;
   lcd_init();
   I2C_master_init();
  
   I2C_master_start();    //write for slave
   I2C_master_write(0xa0);// 1010 0000  base address of device is 000 for a1,a2,a3
   I2C_master_write(0x00);
   I2C_master_write(0x28);
   for(i=0;i<=8;i++)
   {
       I2C_master_write(str[i]);
   }
   I2C_master_stop();
   
   __delay_ms(1000);
   
     I2C_master_init();
    I2C_master_start();
    I2C_master_write(0xae); //1010 1110
    I2C_master_write(0x00);
    I2C_master_write(0x38);
    
    for(i=0;i<=4;i++)
    {
        I2C_master_write(str1[i]);
    }
    I2C_master_stop();
   
   I2C_master_start();  //read from slave
   I2C_master_write(0xa0);// 1010 0000
   I2C_master_write(0x00);
   I2C_master_write(0x28);
   I2C_master_repeated_start();
   I2C_master_write(0xa1); // 1010 0001
   send_cmd(0x80);
   show_data("EEPROM1: ");
   __delay_ms(400);
   send_cmd(0xc0);
   
   for(i=0;i<9;i++)
   {
   if(i<8)
   {
       data=I2C_master_read(0);// 0 ACK
   }
   else
   {
       data=I2C_master_read(1);// 1 not ACK
   }
     send_data(data);
     __delay_ms(250);
   }
  // I2C_master_read(0);// 0 ack, 1 not ack
   I2C_master_stop();
   
    I2C_master_start();  //read from slave
   I2C_master_write(0xae);// 1010 1110
   I2C_master_write(0x00);
   I2C_master_write(0x38);
   I2C_master_repeated_start();
   I2C_master_write(0xaf); // 1010 1111
   
   send_cmd(0x01);  // clear LCD
   send_cmd(0x80);  // move to first line start
   show_data("EEPROM2:");
   __delay_ms(100);
   send_cmd(0xc0);  // move to second line
 
   for(i=0;i<5;i++)
   {
   if(i<4)
   {
       data=I2C_master_read(0);// 0 ACK
   }
   else
   {
       data=I2C_master_read(1);// 1 not ACK
   }
   
   send_data(data);
   __delay_ms(250);
   }
  // I2C_master_read(0);// 0 ack, 1 not ack
   I2C_master_stop();
   while(1);
}
void I2C_master_init()
{
SSPCON=0X28;
SSPCON2=0X00;
SSPSTAT=0X00;
SSPADD=0X19;
TRISC=0x18;
}
void I2C_master_wait()
{
    while((SSPSTAT & 0X04) || (SSPCON2 & 0X1F));
}
void I2C_master_start()
{
    I2C_master_wait();
    SEN=1;
}
void I2C_master_repeated_start()
{
    I2C_master_wait();
    RSEN=1;
}
void I2C_master_stop()
{
    I2C_master_wait();
    PEN=1;
}
void I2C_master_write(unsigned char a)
{
    I2C_master_wait();
    SSPBUF=a;
}
unsigned char I2C_master_read(unsigned char b)
{
    unsigned char temp;
    I2C_master_wait();
    RCEN=1;
    I2C_master_wait();
    temp=SSPBUF;
    I2C_master_wait();
    ACKDT=b;
    ACKEN=1;
    RCEN=0;
    return temp;
}
void lcd_init()
{  
	send_cmd(0x38);
	send_cmd(0x0e);
	send_cmd(0x01);
	send_cmd(0x06);
}
void send_cmd(char c)
{
    LCD = c;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}
void send_data(char d)
{
    LCD = d;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}
void show_data(char* ptr)
{
    while(*ptr)
    {
        send_data(*ptr++);
    }
}