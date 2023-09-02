/*
    UART LOOP back example for Microchip 16F877A
 */


#include <xc.h>

/* Configuration registers select external clock 

    FOSC - clock
    CPD - EEPROM read protection
    CP - Program mempory Read protection

    https://microchipdeveloper.com/mcu1102:configuration-bits

*/
#pragma config  FOSC    = HS  
#pragma config  CPD     = ON   
#pragma config  CP      = ON   

/*
    serial number of device: 16F877A0
 */
 __EEPROM_DATA('1','6','F','8','7','7','A','0');

/*
    Global macros
 */
#define BAUDRATE    115200
#define CLOCK       20000000UL
#define LOOP        while(1)
#define BAUDRATE_REG    (CLOCK/(long)(64UL*BAUDRATE))-1
#define RESET_CHIP  __asm__ volatile ("reset");

/*Global variables */
unsigned char rxCh ;

/* Function prototypes */
void setup(void);

/* Main entry point to the code */
void main(void) 
{
    
    setup();
    LOOP
    {
        while(0 == RCIF);
        RCIF = 0;
        rxCh = RCREG;

        while(0 == TXIF);
        TXIF = 0;
        TXREG = rxCh;

    }

    // Reset the chip if it reaches here
    RESET_CHIP;
}

/*
    Configure UART for 8 bit TxD and Rxd for continous
 */
void setup(void)
{
   unsigned char ch = 0x00;
   unsigned char index = 0x00;

    TRISC = 0b10000000;
    TXSTA = (1<<5);
    RCSTA = (1<<7) | (1<<4);
    SPBRG =  BAUDRATE_REG;


    /* Print serial number of device from the EEPROM */
    while(index != 8)
    {
          ch = eeprom_read(index);
          index++;
          while(0 == TXIF);
          TXIF = 0;
          TXREG = ch;
    }
}



