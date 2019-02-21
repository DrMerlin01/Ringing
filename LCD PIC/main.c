#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "lcd_hd44780_pic16.h"

#define _XTAL_FREQ 4000000
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

int second = 0;
int minute = 0;
int hours = 0;
int count=0;
int vrS=0;
int vrM=0;
int vrH=0;
int FB=0, SB=0;

void clockTime(int x)
{
	switch(x)
	{
		case 0:
		{
			LCDWriteString("0");
			break;
		}
		case 1:
		{
			LCDWriteString("1");
			break;
		}
		case 2:
		{
			LCDWriteString("2");
			break;
		}
		case 3:
		{
			LCDWriteString("3");
			break;
		}
		case 4:
		{
            LCDWriteString("4");
			break;
		}
		case 5:
		{
			LCDWriteString("5");
			break;
		}
		case 6:
		{
            LCDWriteString("6");
			break;
		}
		case 7:
		{
			LCDWriteString("7");
			break;
		}
		case 8:
		{
            LCDWriteString("8");
			break;
		}
		case 9:
		{
            LCDWriteString("9");
			break;
		}
	}
}

void interrupt isr(void)
{
    if(TMR1IF==1)
    {  
        TMR1=34286;
        TMR1IF=0;

            second++; 
            if(second>59) 
            {
                second=0;
                minute++;
                if(minute>59)
                {
                    minute=0;
                    hours++;
                    if(hours>23)
                    {
                        hours=0;
                    }
                }
            }
    }
    
    if(T0IF==1)
    {
        TMR0=0;
        T0IF=0;

        if(PORTBbits.RB0==0)
    	{
    		FB=1;
    	}
	       
    }
    
    if(TMR2IF==1)
    {
        TMR2=0;
        TMR2IF=0;

    	if(PORTBbits.RB3==0)
    	{
    		SB=1;
    	}
    }
}

void main (void)
{
    uint8_t smile[8];
    TRISB=0b00000000;
    PORTB=0xFF;

    GIE=1;
    PEIE=1;
 
    TMR0IE=1;
    OPTION_REG=0b00000111;
    TMR0=0;

    TMR1ON=1;
    TMR1IE=1;
    TMR1CS=0;
    T1CKPS1=1;
    T1CKPS0=1;
    TMR1=34286;

    TMR2ON=1;
    TMR2IE=1;
    T2CKPS1=1;
    T2CKPS0=1;
    TMR2=0;
    
    LCDInit(LS_NONE);
    LCDClear();
    
    smile[0] = 0b00100; 
    smile[1] = 0b00010;
    smile[2] = 0b10001;
    smile[3] = 0b00001;
    smile[4] = 0b10001;
    smile[5] = 0b00010;
    smile[6] = 0b00100;
    smile[7] = 0b00000;
    
//    LCDCreateUserChar(0x06, smile);
    
    LCDGotoXY(0,0);
    LCDWriteString("Kriger");
    LCDGotoXY(0,1);
    LCDWriteString("Khlyzov");
    LCDGotoXY(0,2);
    LCDWriteString("Kozlova");
    LCDGotoXY(0,3);
    LCDWriteString("Savinkov");
    LCDGotoXY(10,3);
    LCDWriteString(smile);
    
    while(1)
    {
         LCDGotoXY(8,0);
		 clockTime(hours/10);
		 LCDGotoXY(9,0);
		 clockTime(hours%10);
		 LCDGotoXY(10,0);
		 LCDWriteString(":");
		 LCDGotoXY(11,0);
		 clockTime(minute/10);
		 LCDGotoXY(12,0);
		 clockTime(minute%10);
		 LCDGotoXY(13,0);
		 LCDWriteString(":");
		 LCDGotoXY(14,0);
		 clockTime(second/10);
		 LCDGotoXY(15,0);
		 clockTime(second%10);

		 vrH=hours;
		 vrM=minute;
		 vrS=second;
		 if(FB==1)
		 {
            LCDGotoXY(8,2);
            clockTime(vrH/10);
            LCDGotoXY(9,2);
            clockTime(vrH%10);
            LCDGotoXY(10,2);
            LCDWriteString(":");
            LCDGotoXY(11,2);
            clockTime(vrM/10);
            LCDGotoXY(12,2);
            clockTime(vrM%10);
            LCDGotoXY(13,2);
            LCDWriteString(":");
            LCDGotoXY(14,2);
            clockTime(vrS/10);
            LCDGotoXY(15,2);
            clockTime(vrS%10);
            FB=0;
		 }
		 if (SB==1)
		 {
            LCDGotoXY(8,1);
            clockTime(vrH/10);
		 	LCDGotoXY(9,1);
            clockTime(vrH%10);
            LCDGotoXY(10,1);
            LCDWriteString(":");
            LCDGotoXY(11,1);
            clockTime(vrM/10);
            LCDGotoXY(12,1);
            clockTime(vrM%10);
            LCDGotoXY(13,1);
            LCDWriteString(":");
            LCDGotoXY(14,1);
            clockTime(vrS/10);
            LCDGotoXY(15,1);
            clockTime(vrS%10);
            SB=0;
		 }
    }

}
