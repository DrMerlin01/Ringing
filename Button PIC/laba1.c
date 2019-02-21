#include <htc.h>
#include "pic16f877a.h"
#define _XTAL_FREQ 4000000

int n=0; 

void reset(void) 
{ 
    TRISB=0b00000000; 
    PORTB=0b11111111; 
    TRISD=0b11111111; 
    PORTD=0b11111111;
}

void interrupt isr()
{ 
    if(TMR1IF&&TMR1IE)
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
    else if (T0IF&&T0IE) 
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
