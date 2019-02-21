#define F_CPU 8000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DIG1_PB4 4//разряды 
#define DIG2_PB5 5
#define DIG3_PB6 6
#define DIG4_PB7 7

#define A_PD0 0//сегменты
#define B_PD1 1
#define C_PD2 2
#define D_PD3 3
#define E_PD4 4
#define F_PD5 5
#define G_PD6 6
#define H_PD7 7

int key = 20;//для клавиатуры
int result = 20;//для передачи числа на дисплей
int temp = 4;//разряды

int first=4;//начальные числа выводит
int second=5;
int third=6;
int fourth=7;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER1_OVF_vect) 
{
	TCNT1=60000;
	DDRC=0x00; //столбцы
	PORTC=0xFF;
	DDRB=0xF0;
	PORTB=0x0F;

	if ((PINC&0x01)==0) key=0;
	if ((PINC&0x02)==0) key=4;
	if ((PINB&0x04)==0) key=8;
	if ((PINB&0x08)==0) key=12;

	DDRC=0xFF; //строки
	PORTC=0x00;
	DDRB=0x0F;
	PORTB=0xF0;

	if ((PINB&0x1F)==0) key=key+0; 
	if ((PINB&0x2F)==0) key=key+1;
	if ((PINB&0x4F)==0) key=key+2;
	if ((PINB&0x8F)==0) key=key+3;


	if (key != 20)
	{
		result=key;
		temp++;
	}
	key=20;
	init();//перенастройка порта для разрядов

	if (temp > 7)
	{
		temp = 4;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init(void) { //настройка разрядов
	DDRB |= ((1<<DIG2_PB5)|(1<<DIG1_PB4)|(1<<DIG3_PB6)|(1<<DIG4_PB7));
	PORTB |= ((1<<DIG2_PB5)|(1<<DIG1_PB4)|(1<<DIG3_PB6)|(1<<DIG4_PB7));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void show(int digit) {//вывод числа
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_COMP_vect)
{				
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


		PORTD=0x00;//гасим все
		PORTB &= ~(1<<DIG1_PB4);//зажигаем разряд первый
		if(first>=0)
		{
			show(first);//выводим число
		}
		PORTB |= (1<<DIG1_PB4);//гасим разряд
		
		PORTD=0x00;//тоже что и выше и т.д.
		PORTB &= ~(1<<DIG2_PB5);
		if(second>=0)
		{
			show(second);
		}
		PORTB |= (1<<DIG2_PB5);
			
		PORTD=0x00;
		PORTB &= ~(1<<DIG3_PB6);
		if(third>=0)
		{	
			show(third);
		}
		PORTB |= (1<<DIG3_PB6);
	
		PORTD=0x00;
		PORTB &= ~(1<<DIG4_PB7);
		if(fourth>=0)
		{
			show(fourth);
		}
		PORTB |= (1<<DIG4_PB7);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {

	DDRD = 0xFF;//настройка сегментов
	PORTD = 0x00;	
	
	TIMSK|=((1<<TOIE1)|(1<<OCIE0));//по переполнению и по сравнению 
	TCCR1B=(1<<CS12);//предделитель на 256
	TCCR0 |=((1<<CS02)|(1<<WGM01));//256 и режим СТС
	
	OCR0=156;
	TCNT1=60000;
	sei();

	while(1) {}
}
