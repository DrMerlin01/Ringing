#define F_CPU 8000000UL 
#define LCD_4bit
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd_lib.h"
#include "avr/pgmspace.h"

int second = 0;
int minute = 0;
int hours = 0;
int vrS=0;
int vrM=0;
int vrH=0;
int FB=0, SB=0;
const uint8_t  smile[] PROGMEM = { 0b00100, 0b00010, 0b10001, 0b00001, 0b10001, 0b00010, 0b00100, 0b0 };//пользовательский символ

void clockTime(int x)
{
	switch(x)
	{
		case 0:
		{
			lcd_dat(0x30);
			break;
		}
		case 1:
		{
			lcd_dat(0x31);
			break;
		}
		case 2:
		{
			lcd_dat(0x32);
			break;
		}
		case 3:
		{
			lcd_dat(0x33);
			break;
		}
		case 4:
		{
			lcd_dat(0x34);
			break;
		}
		case 5:
		{
			lcd_dat(0x35);
			break;
		}
		case 6:
		{
			lcd_dat(0x36);
			break;
		}
		case 7:
		{
			lcd_dat(0x37);
			break;
		}
		case 8:
		{
			lcd_dat(0x38);
			break;
		}
		case 9:
		{
			lcd_dat(0x39);
			break;
		}
	}
}

ISR(TIMER1_OVF_vect)
{
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

 TCNT1=34286;
}

ISR(INT0_vect)
{
	_delay_ms(50);

	if((PIND&0x04)==0)
	{
		FB=1;
	}
	else
	{
		return;
	}
}

ISR(INT1_vect)
{
	_delay_ms(50);

	if((PIND&0x08)==0)
	{
		SB=1;
	}
	else
	{
		return;
	}
}

int main(void)
{
DDRD=0x00;
PORTD=0xFF;

GICR |=(1<<INT0);
GICR |=(1<<INT1);
MCUCR=0b00001010;

TCCR1B =(1 << CS12);
TIMSK |= (1 << TOIE1);
TCNT1  = 34286;

sei();

lcd_init();
lcd_clr();
lcd_definechar(smile, 0x06);

lcd_gotoxy(0,0);//Саня
lcd_dat(0x58);//Х
lcd_dat(0xBB);//Л
lcd_dat(0xC3);//Ы
lcd_dat(0xB7);//З
lcd_dat(0x6F);//О
lcd_dat(0xB3);//В

lcd_gotoxy(0,1);//Вася
lcd_dat(0x4B);//К
lcd_dat(0x70);//Р
lcd_dat(0xB8);//И
lcd_dat(0xB4);//Г
lcd_dat(0x65);//Е
lcd_dat(0x70);//Р

lcd_gotoxy(-4,2); //Настя
lcd_dat(0x4B);//код буквы К и т.д.
lcd_dat(0x6F);//О
lcd_dat(0xB7);//З
lcd_dat(0xBB);//Л
lcd_dat(0x6F);//О
lcd_dat(0xB3);//В
lcd_dat(0x61);//А

lcd_gotoxy(-4,3);//Игорь
lcd_dat(0x43);//С
lcd_dat(0x61);//А
lcd_dat(0xB3);//В
lcd_dat(0xB8);//И
lcd_dat(0xBD);//Н
lcd_dat(0xBA);//К
lcd_dat(0x6F);//О
lcd_dat(0xB3);//В
lcd_gotoxy(5,3);
lcd_dat(0x06);
 
    while (1) 
    {
		 lcd_gotoxy(8,0);
		 clockTime(hours/10);
		 lcd_gotoxy(9,0);
		 clockTime(hours%10);
		 lcd_gotoxy(10,0);
		 lcd_dat(0x3A);
		 lcd_gotoxy(11,0);
		 clockTime(minute/10);
		 lcd_gotoxy(12,0);
		 clockTime(minute%10);
		 lcd_gotoxy(13,0);
		 lcd_dat(0x3A);
		 lcd_gotoxy(14,0);
		 clockTime(second/10);
		 lcd_gotoxy(15,0);
		 clockTime(second%10);

		 vrH=hours;
		 vrM=minute;
		 vrS=second;
		 if(FB==1)
		 {
		 lcd_gotoxy(4,2);
		 clockTime(vrH/10);
		 lcd_gotoxy(5,2);
		 clockTime(vrH%10);
		 lcd_gotoxy(6,2);
		 lcd_dat(0x3A);
		 lcd_gotoxy(7,2);
		 clockTime(vrM/10);
		 lcd_gotoxy(8,2);
		 clockTime(vrM%10);
		 lcd_gotoxy(9,2);
		 lcd_dat(0x3A);
		 lcd_gotoxy(10,2);
		 clockTime(vrS/10);
		 lcd_gotoxy(11,2);
		 clockTime(vrS%10);
		 FB=0;
		 }
		 if (SB==1)
		 {
		 lcd_gotoxy(8,1);
		 clockTime(vrH/10);
		 lcd_gotoxy(9,1);
		 clockTime(vrH%10);
		 lcd_gotoxy(10,1);
		 lcd_dat(0x3A);
		 lcd_gotoxy(11,1);
		 clockTime(vrM/10);
		 lcd_gotoxy(12,1);
		 clockTime(vrM%10);
		 lcd_gotoxy(13,1);
		 lcd_dat(0x3A);
		 lcd_gotoxy(14,1);
		 clockTime(vrS/10);
		 lcd_gotoxy(15,1);
		 clockTime(vrS%10);
		 SB=0;
		 }
    }
}

