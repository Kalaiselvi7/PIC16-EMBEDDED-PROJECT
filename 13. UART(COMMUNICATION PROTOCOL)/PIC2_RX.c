// PIC16F877A Configuration Bit Settings
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#include <xc.h>
#define _XTAL_FREQ 20000000

#define lcd PORTB
#define RS RC0
#define RW RC1
#define EN RC2

// LCD Functions
void lcd_init();
void send_cmd(char a);
void send_data(char b);

// UART Functions
void USART_RX_Init();
unsigned char RX();

int main()
{
    char GET;
    TRISB = 0x00;   // LCD data lines as output
    TRISC0 = 0;     // RS as output
    TRISC1 = 0;     // RW as output
    TRISC2 = 0;     // EN as output
    TRISC7 = 1;     // RX pin as input

    USART_RX_Init();
    lcd_init();

    while(1)
    {
        GET = RX();         // wait & read received character
        send_data(GET);     // display next character (cursor auto-increments)
        __delay_ms(10);
    }
}

void USART_RX_Init()
{
    TXSTA = 0x04;   // BRGH=1, TX disabled, async mode
    RCSTA = 0x90;   // SPEN=1, CREN=1 (enable RX)
    SPBRG = 129;    // 9600 baud @ 20MHz (high speed mode)
}

// --- LCD Functions ---
void send_cmd(char a)
{
    lcd = a;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_init()
{
    send_cmd(0x38); // 8-bit mode, 2 lines
    send_cmd(0x0C); // Display ON, cursor OFF
    send_cmd(0x01); // Clear display
    send_cmd(0x06); // Auto-increment cursor
    send_cmd(0x80); // Start at first line
}

void send_data(char b)
{
    lcd = b;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

// --- UART Receive ---
unsigned char RX()
{
    while(RCIF == 0);  // wait until data received
    return RCREG;      // read & return data (reading clears RCIF)
}