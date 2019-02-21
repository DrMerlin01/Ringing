#include <xc.h>
#define _XTAL_FREQ 4000000
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

int n=0; 

void reset(void) 
{ 
    TRISB=0b00000000; 
    PORTB=0b11111111; 
    TRISD=0b11111111; 
    PORTD=0b11111100;
}

void interrupt isr()
{ 
    if((TMR1IF==1)&&(TMR1IE==1))
    {
        TMR1IF=0;
        TMR1=60000;
        
        if(PORTDbits.RD0==0)
        {
            n++;
            if (n==10) 
            { 
                PORTBbits.RB0=0; 
            } 
            if (n==20) 
            { 
                PORTBbits.RB1=0; 
            } 
            if (n==30)
            { 
                PORTBbits.RB2=0; 
            } 
            if (n==40) 
            { 
                PORTBbits.RB3=0; 
            } 
            if (n>40) 
            { 
                PORTB=0x00; 
            }
            while(PORTDbits.RD0==0){};   
        }
    }
    else if ((T0IF==1)&&(T0IE==1)) 
    {
        T0IF=0;
        TMR0=0;
        
        if(PORTDbits.RD1==0)
        { 
            n=0;
            reset();
            while (PORTDbits.RD1==0){}; 
        }
    }
} 

int main(void) 
{ 
    INTCON=0b11100000;
    OPTION_REG=0b00000111;
    TMR0=0;
    T0IF=0;

    TMR1ON=1;
    T1OSCEN=1;
    TMR1IE=1;
    T1CKPS1=1;
    T1CKPS0=1;
    TMR1IF=0;
    TMR1=60000;
    
    reset(); 
    while(1){} 
}
