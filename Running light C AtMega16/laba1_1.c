#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int x = 285;

int delay(int x)
{
	if (PINB==0b11111011)
	{
		_delay_ms(50);
		if (PINB==0b11111011)
		{
		x = x - 285;
		if (x<=285) x=285;
		}
	}
	if (PINB==0b11110111)
	{
		_delay_ms(50);
		if (PINB==0b11110111)
		{
			x = x + 285;
			if (x>=1425) x = 1425;
		}
	}
	
	return x;
}


int main(void)
{
DDRD=0xFF;
DDRB=0x00; 
PORTB=0xFF; 

while(1)
  { 
      PORTD = 0b11111110;
      _delay_ms(x=delay(x));
      
	  PORTD = 0b11111111;
      _delay_ms(x=delay(x));
	  
	  PORTD = 0b11111101;
      _delay_ms(x=delay(x));
      
	  PORTD = 0b11111111;
      _delay_ms(x=delay(x));

	  PORTD = 0b11111011;
      _delay_ms(x=delay(x));
	  
	  PORTD = 0b11111111;
      _delay_ms(x=delay(x));

	  PORTD = 0b11110111;
      _delay_ms(x=delay(x));

	  PORTD = 0b11111111;
      _delay_ms(x=delay(x));                
   }
	
}
