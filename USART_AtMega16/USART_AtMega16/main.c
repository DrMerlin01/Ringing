#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "lcd_lib.h"

const float valueADC = 0.00073242;//ацп 12 разрядный, питание 3 В => 3/(2^12)=0.00073242 шаг измерения
const float valueTimer = 0.039; // 0.00117// 0.039
int Uo = 3000;
int Uy = 10;

int IsBlocked = 0;

void PWM_Init()
{
	OCR0 = Uy;//50% рабочий цикл (set PWM for 50% duty cycle)
	TCCR0 |= (1<<COM01);//неинвертированный режим
	TCCR0 |= (1<<WGM01)|(1<<WGM00);//режим работы Fast PWM
	TCCR0 |= (1<<CS01);//предделитель 8 и запуск PWM
}

void USART_Init()
{
	UBRRH = 0;
	UBRRL = 8;//57600
	UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);//настраиваем прием и передачу данных, разрешаем прерывание по приему
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);//прием и передача по 8 бит
}

unsigned long Usart_Receive(void)//прием символов
{
	unsigned long ReceivedByte;//переменная для хранения символов
	while((UCSRA & (1 << RXC)) == 0) {};//ждем пока появится символ
	ReceivedByte = UDR ;//записываем символ
	return ReceivedByte;//возвращаем символ
}

void Print_Float(float x)//вывод напряжения
{
	unsigned char value[5] = {'\0','\0','\0','\0','\0'}; //массив для хранения результата преобразований
	int i = 0; //счетчик
	
	int num = x*100;
	value[i++] = (unsigned char)(num/100) + '0';//записываем целую часть
	value[i++] = '.';//записываем точку
	value[i++] = (unsigned char)((num/10)%10) + '0';//десятую часть записываем
	value[i++] = (unsigned char)(num % 10) + '0';//записываем сотую часть

	lcd_dat((uint8_t*)value[0]);//выводим первый символ
	lcd_dat((uint8_t*)value[1]);//выводим второй символ
	lcd_dat((uint8_t*)value[2]);//выводим третий символ
	lcd_dat((uint8_t*)value[3]);//выводим четвертый символ
}

void main()
{
	lcd_init();//инициализация дисплея
	lcd_clr();//очистка дипслея
	
	DDRB = 0xFF;//порт B на вход

	PWM_Init();//инициализация PWM
	USART_Init();//инициализация USART

	unsigned char digitalPWM[6] = {'\0','\0','\0','\0','\0','\0'};//массив для хранения напряжения PWM
	unsigned char digitalInput[6] = {'\0','\0','\0','\0','\0','\0'};//массив для хранения выходного напряжения
	int pwmCounter = 0;
	int staticCounter = 0;
	while (1)
	{
		unsigned char sym;//переменная для хранения принятых символов
		sym = Usart_Receive();//принимаем отцифрованное значение с стм32
		if(sym)//если что-то пришло
		{
			if(IsBlocked == 0 && sym != 'v')
			{
				digitalPWM[pwmCounter++] = sym;
			}
			else
			{
				IsBlocked = 1;
				if(sym == 'v') continue;
				digitalInput[staticCounter++] = sym;
				if(sym == ' ')
				{
					IsBlocked = 0;
					int Uoc = atoi(digitalPWM);
					Uo = atoi(digitalInput);
					int diff = Uo - Uoc;
					int diffTimer = (diff * valueTimer);
					float Kp = (float)(Uy/diffTimer);
					Uy = Uy + diffTimer;
					
					if(Uy > 255) Uy = 255;
					if(Uy < 0) Uy = 1;
					OCR0 = Uy;
					
					lcd_gotoxy(0,0);//первая строка
					lcd_dat((uint8_t*)'P');//выводим P
					lcd_dat((uint8_t*)'W');//выводим W
					lcd_dat((uint8_t*)'M');//выводим M
					lcd_dat((uint8_t*)' ');//выводим ' '
					lcd_dat((uint8_t*)'=');//выводим =
					lcd_dat((uint8_t*)' ');//выводим ' '
					Print_Float(Uoc*valueADC);//выводим напряжение PWM
					lcd_dat((uint8_t*)'V');//выводим V
					lcd_gotoxy(0,1);//вторая строка
					lcd_dat((uint8_t*)'I');//выводим I
					lcd_dat((uint8_t*)'n');//выводим n
					lcd_dat((uint8_t*)'p');//выводим p
					lcd_dat((uint8_t*)'u');//выводим u
					lcd_dat((uint8_t*)'t');//выводим t
					lcd_dat((uint8_t*)' ');//выводим ' '
					lcd_dat((uint8_t*)'=');//выводим =
					lcd_dat((uint8_t*)' ');//выводим ' '
					Print_Float(Uo*valueADC);//выводим напряжение считанное АЦП
					lcd_dat((uint8_t*)'V');//выводим V
					
					pwmCounter = 0;//сбрасываем в 0
					staticCounter = 0;//сбрасываем в 0
					for (int i = 0; i < 6; i++)//заполняем массивы нулями
					{
						digitalInput[i] = '\0';
						digitalPWM[i] = '\0';
					}
				}
			}
		}
	}
}
