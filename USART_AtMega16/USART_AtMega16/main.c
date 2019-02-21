#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "lcd_lib.h"

const float valueADC = 0.00073242;//��� 12 ���������, ������� 3 � => 3/(2^12)=0.00073242 ��� ���������
const float valueTimer = 0.039; // 0.00117// 0.039
int Uo = 3000;
int Uy = 10;

int IsBlocked = 0;

void PWM_Init()
{
	OCR0 = Uy;//50% ������� ���� (set PWM for 50% duty cycle)
	TCCR0 |= (1<<COM01);//����������������� �����
	TCCR0 |= (1<<WGM01)|(1<<WGM00);//����� ������ Fast PWM
	TCCR0 |= (1<<CS01);//������������ 8 � ������ PWM
}

void USART_Init()
{
	UBRRH = 0;
	UBRRL = 8;//57600
	UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);//����������� ����� � �������� ������, ��������� ���������� �� ������
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);//����� � �������� �� 8 ���
}

unsigned long Usart_Receive(void)//����� ��������
{
	unsigned long ReceivedByte;//���������� ��� �������� ��������
	while((UCSRA & (1 << RXC)) == 0) {};//���� ���� �������� ������
	ReceivedByte = UDR ;//���������� ������
	return ReceivedByte;//���������� ������
}

void Print_Float(float x)//����� ����������
{
	unsigned char value[5] = {'\0','\0','\0','\0','\0'}; //������ ��� �������� ���������� ��������������
	int i = 0; //�������
	
	int num = x*100;
	value[i++] = (unsigned char)(num/100) + '0';//���������� ����� �����
	value[i++] = '.';//���������� �����
	value[i++] = (unsigned char)((num/10)%10) + '0';//������� ����� ����������
	value[i++] = (unsigned char)(num % 10) + '0';//���������� ����� �����

	lcd_dat((uint8_t*)value[0]);//������� ������ ������
	lcd_dat((uint8_t*)value[1]);//������� ������ ������
	lcd_dat((uint8_t*)value[2]);//������� ������ ������
	lcd_dat((uint8_t*)value[3]);//������� ��������� ������
}

void main()
{
	lcd_init();//������������� �������
	lcd_clr();//������� �������
	
	DDRB = 0xFF;//���� B �� ����

	PWM_Init();//������������� PWM
	USART_Init();//������������� USART

	unsigned char digitalPWM[6] = {'\0','\0','\0','\0','\0','\0'};//������ ��� �������� ���������� PWM
	unsigned char digitalInput[6] = {'\0','\0','\0','\0','\0','\0'};//������ ��� �������� ��������� ����������
	int pwmCounter = 0;
	int staticCounter = 0;
	while (1)
	{
		unsigned char sym;//���������� ��� �������� �������� ��������
		sym = Usart_Receive();//��������� ������������� �������� � ���32
		if(sym)//���� ���-�� ������
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
					
					lcd_gotoxy(0,0);//������ ������
					lcd_dat((uint8_t*)'P');//������� P
					lcd_dat((uint8_t*)'W');//������� W
					lcd_dat((uint8_t*)'M');//������� M
					lcd_dat((uint8_t*)' ');//������� ' '
					lcd_dat((uint8_t*)'=');//������� =
					lcd_dat((uint8_t*)' ');//������� ' '
					Print_Float(Uoc*valueADC);//������� ���������� PWM
					lcd_dat((uint8_t*)'V');//������� V
					lcd_gotoxy(0,1);//������ ������
					lcd_dat((uint8_t*)'I');//������� I
					lcd_dat((uint8_t*)'n');//������� n
					lcd_dat((uint8_t*)'p');//������� p
					lcd_dat((uint8_t*)'u');//������� u
					lcd_dat((uint8_t*)'t');//������� t
					lcd_dat((uint8_t*)' ');//������� ' '
					lcd_dat((uint8_t*)'=');//������� =
					lcd_dat((uint8_t*)' ');//������� ' '
					Print_Float(Uo*valueADC);//������� ���������� ��������� ���
					lcd_dat((uint8_t*)'V');//������� V
					
					pwmCounter = 0;//���������� � 0
					staticCounter = 0;//���������� � 0
					for (int i = 0; i < 6; i++)//��������� ������� ������
					{
						digitalInput[i] = '\0';
						digitalPWM[i] = '\0';
					}
				}
			}
		}
	}
}
