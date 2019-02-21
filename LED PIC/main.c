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

#define DIG1_PA4 0
#define DIG2_PA5 1
#define DIG3_PA6 2
#define DIG4_PA7 3

#define A_PD0 0
#define B_PD1 1
#define C_PD2 2
#define D_PD3 3
#define E_PD4 4
#define F_PD5 5
#define G_PD6 6
#define H_PD7 7

int key = 20;
int result = 20;
int temp = 4;

int first=4;
int second=5;
int third=6;
int fourth=7;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void show(int digit) {
	switch(digit)
	{
		case 0:
		{
			PORTD |= ((1<<A_PD0)|(1<<B_PD1)|(1<<C_PD2)|(1<<D_PD3)|(1<<E_PD4)|(1<<F_PD5));	
			break;
		}
		case 1:
		{
			PORTD |= ((1<<B_PD1)|(1<<C_PD2));	
			break;
		}
		case 2:
		{
			PORTD |= ((1<<A_PD0)|(1<<B_PD1)|(1<<D_PD3)|(1<<E_PD4)|(1<<G_PD6));	
			break;
		}
		case 3:
		{
			PORTD |= ((1<<A_PD0)|(1<<B_PD1)|(1<<C_PD2)|(1<<D_PD3)|(1<<G_PD6));	
			break;
		}
		case 4:
		{
			PORTD |= ((1<<B_PD1)|(1<<C_PD2)|(1<<F_PD5)|(1<<G_PD6));	
			break;
		}
		case 5:
		{
			PORTD |= ((1<<A_PD0)|(1<<C_PD2)|(1<<D_PD3)|(1<<G_PD6)|(1<<F_PD5));	
			break;
		}
		case 6:
		{
			PORTD |= ((1<<A_PD0)|(1<<C_PD2)|(1<<D_PD3)|(1<<E_PD4)|(1<<G_PD6)|(1<<F_PD5));	
			break;
		}
		case 7:
		{
			PORTD |= ((1<<A_PD0)|(1<<C_PD2)|(1<<B_PD1));	
			break;
		}
		case 8:
		{
			PORTD |= ((1<<A_PD0)|(1<<B_PD1)|(1<<C_PD2)|(1<<D_PD3)|(1<<E_PD4)|(1<<G_PD6)|(1<<F_PD5));	
			break;
		}
		case 9:
		{
			PORTD |= ((1<<A_PD0)|(1<<B_PD1)|(1<<C_PD2)|(1<<D_PD3)|(1<<G_PD6)|(1<<F_PD5));	
			break;
		}
		case 10:
		{
			PORTD |= ((1<<A_PD0)|(1<<B_PD1)|(1<<C_PD2)|(1<<E_PD4)|(1<<G_PD6)|(1<<F_PD5));	
			break;
		}
		case 11:
		{
			PORTD |= ((1<<C_PD2)|(1<<D_PD3)|(1<<E_PD4)|(1<<G_PD6)|(1<<F_PD5));	
			break;
		}
		case 12:
		{
			PORTD |= ((1<<A_PD0)|(1<<D_PD3)|(1<<E_PD4)|(1<<F_PD5));	
			break;
		}
		case 13:
		{
			PORTD |= ((1<<B_PD1)|(1<<C_PD2)|(1<<D_PD3)|(1<<E_PD4)|(1<<G_PD6));	
			break;
		}
		case 14:
		{
			PORTD |= ((1<<A_PD0)|(1<<D_PD3)|(1<<E_PD4)|(1<<G_PD6)|(1<<F_PD5));	
			break;
		}
		case 15:
		{
			PORTD |= ((1<<A_PD0)|(1<<E_PD4)|(1<<G_PD6)|(1<<F_PD5));	
			break;
		}
	}	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void interrupt isr()
{
    if(TMR1IF==1)
    {
        TMR1IF=0;
        TMR1=60000;
	
        TRISB=0xF0;
        PORTB=0x0F;
        if ((PORTBbits.RB0&0x01)==0) key=0;
        if ((PORTBbits.RB1&0x02)==0) key=4;
        if ((PORTBbits.RB2&0x03)==0) key=8;
        if ((PORTBbits.RB3&0x04)==0) key=12;

        TRISB=0x0F;
        PORTB=0xF0;
        if ((PORTBbits.RB4&0x1F)==1) key=key+0;
        if ((PORTBbits.RB5&0x2F)==1) key=key+1;
        if ((PORTBbits.RB6&0x3F)==1) key=key+2;
        if ((PORTBbits.RB7&0x4F)==1) key=key+3;


        if (key != 20)
    	{
    		result=key;
    		temp++;
    	}
    	key=20;
        
        if (temp > 7)
        {
    		temp = 4;
    	}
    }
    
    if(T0IF==1)
    {
        T0IF=0;
		TMR0=0;
		if (result != 20)
		{
			if ((temp-1) == 4)
			{
				first = result;
			}			
			if ((temp-1) == 5)
			{
				second = result;
			}			
			if ((temp-1) == 6)
			{
				third = result;
			}			
			if (temp == 4)
			{
				fourth = result;
			}
			result = 20;			
		}


		PORTD=0x00;
		PORTA |= (1<<DIG1_PA4);
		if(first>=0)
		{
			show(first);
		}
		PORTA &= ~(1<<DIG1_PA4);
		
		PORTD=0x00;
		PORTA |= (1<<DIG2_PA5);
		if(second>=0)
		{
			show(second);
		}
		PORTA &= ~(1<<DIG2_PA5);
			
		PORTD=0x00;
		PORTA |= (1<<DIG3_PA6);
		if(third>=0)
		{	
			show(third);
		}
		PORTA &= ~(1<<DIG3_PA6);
	
		PORTD=0x00;
		PORTA |= (1<<DIG4_PA7);
		if(fourth>=0)
		{
			show(fourth);
		}
		PORTA &= ~(1<<DIG4_PA7);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {

	TRISD = 0x00;
	PORTD = 0x00;
    TRISA=0x00;
    PORTA=0xFF;
	
    INTCON=0b11100000;
    OPTION_REG=0b00000111;
    TMR0=0;

    TMR1ON=1;
    TMR1IE=1;
    T1CKPS1=1;
    T1CKPS0=1;
    TMR1=60000;
    
	while(1) {}
}
