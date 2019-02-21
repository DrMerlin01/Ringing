#define F_CPU 8000000UL 
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h> 

int n=0; 

ISR(INT0_vect) 
{ 
_delay_ms(50);

if(PIND==0b11111011)
{
n++;
if (n==10) 
{ 
PORTB&=~(1<<PB2); 
} 
if (n==20) 
{ 
PORTB&=~(1<<PB3); 
} 
if (n==30) 
{ 
PORTB&=~(1<<PB4); 
} 
if (n==40) 
{ 
PORTB&=~(1<<PB5); 
} 
if (n>40) 
{ 
PORTB=0x00; 
}
while (PIND==0b11111011){};   
}
else
{
return 0;
}
} 

ISR(INT1_vect) 
{
_delay_ms(50);

if(PIND==0b11110111)
{ 
n=0;
reset();
while (PIND==0b11110111){}; 
}
else
{
return 0;
} 
} 

void reset() 
{ 
DDRB=0xFF; 
PORTB=0b00111100; 
DDRD=0x00; 
PORTD=0xFF;

GICR |=(1<<INT0); 
GICR |=(1<<INT1); 
MCUCR=0b00001010;
sei(); 
} 

int main(void) 
{ 
reset(); 
while(1){} 
}
