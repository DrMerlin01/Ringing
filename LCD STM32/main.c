#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#define port    	GPIOB
#define init_port 	RCC_APB2Periph_GPIOB
#define pin_e 		GPIO_Pin_10
#define pin_rw		GPIO_Pin_11
#define pin_rs		GPIO_Pin_12

#define lcd_shift	6			///����� ����������� ���� � 4-� ������ ����
#define use_gpio	GPIO_Pin_9  //������� ���								9
#define pin_d7		use_gpio    //������� ���								9
#define pin_d6		use_gpio>>1 //��������� ��� �� ��������					8
#define pin_d5		use_gpio>>2 //��������� ��� �� ��������					7
#define pin_d4		use_gpio>>3 //��������� ��� � 4-� ������ ����			6

#define Function_set 				0b00100000//4-bit,2 - line mode, 5*8 dots
#define Display_on_off_control		0b00001111/// display on,cursor off,blink off
#define Display_clear				0b00000001
#define Entry_mode_set				0b00000100//

#define rs_1	port->ODR |=  pin_rs
#define rs_0	port->ODR &=~ pin_rs
#define e_1 	port->ODR |=  pin_e
#define e_0		port->ODR &=~ pin_e
#define rw_1	port->ODR |=  pin_rw
#define rw_0	port->ODR &=~ pin_rw
u32 del;
const unsigned char russian[]={ 0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45,
0xA3, 0xA4, 0xA5,0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 0x50,0x43,
0x54, 0xA9, 0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD,0xAE, 0x62,
0xAF, 0xB0, 0xB1, 0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7,
0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0x6F, 0xBE, 0x70, 0x63,0xBF,
0x79, 0xE4, 0x78, 0xE5, 0xC0, 0xC1, 0xE6, 0xC2,0xC3, 0xC4, 0xC5,
0xC6, 0xC7 };//��� ������� �������

void Init_pin_out(void);//��������� ������ �� �����
void Init_pin_in(void);//��������� ������ �� ����
void Init_lcd(void);//������������� �������
void Lcd_write_data(uint8_t byte);//������ ������
void Lcd_write_cmd(uint8_t byte);//������ ������
void Lcd_clear(void);//������� ������
void Return_home(void);//��������� � ������
void Lcd_goto(uc8 x, uc8 y);//��������� �������
void Lcd_write_str(uc8 *STRING);//������ ������

void lcd_definechar(const uint8_t *pc,uint8_t char_code);//�������� ����������������� �������


int second = 55;//������� �����
int minute = 44;
int hours = 8;
const uint8_t smile[]= { 0b00100, 0b00010, 0b10001, 0b00001, 0b10001, 0b00010, 0b00100, 0b0 };//���������������� ������

void clockTime(int x)//���������� ����� ����� �������
{
	switch(x)
	{
		case 0:
		{
			Lcd_write_str("0");
			break;
		}
		case 1:
		{
			Lcd_write_str("1");
			break;
		}
		case 2:
		{
			Lcd_write_str("2");
			break;
		}
		case 3:
		{
			Lcd_write_str("3");
			break;
		}
		case 4:
		{
			Lcd_write_str("4");
			break;
		}
		case 5:
		{
			Lcd_write_str("5");
			break;
		}
		case 6:
		{
			Lcd_write_str("6");
			break;
		}
		case 7:
		{
			Lcd_write_str("7");
			break;
		}
		case 8:
		{
			Lcd_write_str("8");
			break;
		}
		case 9:
		{
			Lcd_write_str("9");
			break;
		}
	}
}

void TIM3_IRQHandler(void)//����
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//������� ��� ����������
	second++;//����������� �������
	if(second>59)//������� �������� �����
	{
		second=0;
		minute++;//����������� ������
		if(minute>59)
		{
			minute=0;
			hours++;//����������� ����
			if(hours>23)
			{
				hours=0;
			}
		}
	}
}

void TIM4_IRQHandler(void)//������� ����� ������� �� ������ ������
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//������� ��� ����������

	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)//���� ������ ������
	{
		int Hour=hours;//������������ ������� �����
		int Min=minute;
		int Sec=second;

		Lcd_goto(1,8);//������� ����� �������
		clockTime(Hour/10);
		Lcd_goto(1,9);
		clockTime(Hour%10);
		Lcd_goto(1,10);
		Lcd_write_str(":");
		Lcd_goto(1,11);
		clockTime(Min/10);
		Lcd_goto(1,12);
		clockTime(Min%10);
		Lcd_goto(1,13);
		Lcd_write_str(":");
		Lcd_goto(1,14);
		clockTime(Sec/10);
		Lcd_goto(1,15);
		clockTime(Sec%10);
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)//���� ������ ������
	{
		int Hour=hours;//������������ ������� �����
		int Min=minute;
		int Sec=second;

		Lcd_goto(2,8);//������� ����� �������
		clockTime(Hour/10);
		Lcd_goto(2,9);
		clockTime(Hour%10);
		Lcd_goto(2,10);
		Lcd_write_str(":");
		Lcd_goto(2,11);
		clockTime(Min/10);
		Lcd_goto(2,12);
		clockTime(Min%10);
		Lcd_goto(2,13);
		Lcd_write_str(":");
		Lcd_goto(2,14);
		clockTime(Sec/10);
		Lcd_goto(2,15);
		clockTime(Sec%10);
	}
}

void TIM2_IRQHandler(void)//����� �� ����� ��� � ������� �������
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//������� ��� ����������

	Lcd_goto(0,8);
	clockTime(hours/10);
	Lcd_goto(0,9);
	clockTime(hours%10);
	Lcd_goto(0,10);
	Lcd_write_str(":");
	Lcd_goto(0,11);
	clockTime(minute/10);
	Lcd_goto(0,12);
	clockTime(minute%10);
	Lcd_goto(0,13);
	Lcd_write_str(":");
	Lcd_goto(0,14);
	clockTime(second/10);
	Lcd_goto(0,15);
	clockTime(second%10);
}

void main(void)
{
	SystemInit();
	Init_lcd();//������������� ������
	Lcd_clear();//������� ������
	Lcd_definechar(smile,0x06);//����������� ���������������� ������ �� ���������� ������
	Lcd_write_cmd(0b00001100);

	GPIO_InitTypeDef PORTA;//�������� �������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//������������ ����� B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//������������ ����� �
	//���� B
	GPIO_StructInit (&PORTA);
	PORTA.GPIO_Mode = GPIO_Mode_IPU;//�� �����
	PORTA.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//0 � 1 �����
	PORTA.GPIO_Speed = GPIO_Speed_2MHz;//�������� �������
	GPIO_Init (GPIOA,&PORTA);//����������� �������� ����������� ���������

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//�������� ������������ ������� 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//�������� ������������ ������� 3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//�������� ������������ ������� 2

	//������ 4
	TIM4->PSC = 2400;//������������
	TIM4->ARR = 10000;//������, �������� ������ 40 ����
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//����������� ������ ��� ��������� ���������� �� ���������� (������������)
	TIM_Cmd(TIM4, ENABLE);//��������� ������
	NVIC_EnableIRQ(TIM4_IRQn);//��������� ���������� ������� 4

	//������ 3
	TIM3->PSC = 2400;//������������
	TIM3->ARR = 10000;//������ ������ 1 �������
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//����������� ������ ��� ��������� ���������� �� ���������� (������������)
	TIM_Cmd(TIM3, ENABLE);//��������� ������
	NVIC_EnableIRQ(TIM3_IRQn);//��������� ���������� ������� 3

	//������ 2
	TIM2->PSC = 2400;//������������
	TIM2->ARR = 10000;//������ ������ 1 �������
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//����������� ������ ��� ��������� ���������� �� ���������� (������������)
	TIM_Cmd(TIM2, ENABLE);//��������� ������
	NVIC_EnableIRQ(TIM2_IRQn);//��������� ���������� ������� 2

	Lcd_goto(0,0);//� ������ ������
	Lcd_write_str("������");//������ �������
	Lcd_goto(1,0);//�� ������ ������
	Lcd_write_str("������");//������ �������
	Lcd_goto(2,0);//� ������ ������
	Lcd_write_str("�������");//������ �������
	Lcd_goto(3,0);// � ��������� ������
	Lcd_write_str("��������");//��������� �������
	Lcd_goto(3,9);//� � ��������� ������ ������
	Lcd_write_data(0x06);//������� ������ ���

    while (1){}
}

void Lcd_write_str(uc8 *STRING)//������ ������
{
	char c; //������ �� ������
	while (c=*STRING++){//���������� �� ���� ������
	if(c>=192) Lcd_write_data(russian[c-192]);//���� � ��� ������� �������
	else Lcd_write_data(c);//����� ������
	}
}

void Lcd_goto(uc8 x,uc8 y)//������� �������
{
	switch(x)
		{
			case 0://���������� ������ ������
				Lcd_write_cmd(y|0x80);//�������
				break;
			case 1://������
				Lcd_write_cmd(0x40+y|0x80);//������� �� �����
				break;
			case 2://������
				Lcd_write_cmd(0x10+y|0x80);//������� �� ����� �������� ��������
				break;
			case 3://�� � ���������
				Lcd_write_cmd(0x50+y|0x80);//������� �� �����
				break;
		}
}

void Init_pin_out()//��������� ������ �� �����
{
	RCC_APB2PeriphClockCmd(init_port, ENABLE);//������������ ����� B
	GPIO_InitTypeDef init_pin;//�������� ���������
	init_pin.GPIO_Pin  = pin_e | pin_rs | pin_rw | pin_d7 | pin_d6 | pin_d5 | pin_d4;//4, 5, 6, 7 , 12, 11, 10 ������
	init_pin.GPIO_Mode = GPIO_Mode_Out_PP;//�� �����
	init_pin.GPIO_Speed = GPIO_Speed_50MHz;//��������
	GPIO_Init (port, &init_pin);//������ � ��������� ������� ��������
}
void Init_pin_in()//��������� ������ �� ����
{
	RCC_APB2PeriphClockCmd(init_port, ENABLE);//������������ ����� B
	GPIO_InitTypeDef init_pin;//�������� ���������
	init_pin.GPIO_Pin  =  pin_d7 | pin_d6 | pin_d5 | pin_d4 ;//4, 5, 6 � 7 ������
	init_pin.GPIO_Mode = GPIO_Mode_IPD;//�� ����
	init_pin.GPIO_Speed = GPIO_Speed_50MHz;//��������
	GPIO_Init (port, &init_pin);//������ � ��������� ������� ��������
}

void Lcd_write_cmd(uc8 cmd )
{
	Init_pin_out();//��������� ����� �� �����
	del=24000; while (del--){}//��������
	rs_0;//���������� ��� ����� ������������ �������
	GPIO_Write(port,((cmd>>4)<<lcd_shift));//�������� ������� �����
	e_1;//�������� ��������
	del=10; while (del--){}//��������
	e_0;//����������� ��������
	GPIO_Write(port,(0x00)<<lcd_shift);//
	GPIO_Write(port,((cmd&0x0F)<<lcd_shift));//�������� ������� �����
	del=10; while (del--){}//��������
	e_1;//�������� ��������
	del=10; while (del--){}//��������
	e_0;rs_0;rw_0;//����������� ��������
}

void Lcd_write_data(uint8_t data)
{
	Init_pin_out();//��������� ����� �� �����
	GPIO_Write(port,((data>>4)<<lcd_shift));//�������� ������� �����
	e_1;rs_1;//�������� �������� ������
	del=10; while (del--){}//��������
	e_0;//����������� ��������
	GPIO_Write(port,(0x00)<<lcd_shift);
	GPIO_Write(port,((data&0x0F)<<lcd_shift));//�������� ������� �����
	del=10; while (del--){}//��������
	e_1;rs_1;//�������� ��������
	del=10; while (del--){}//��������
	e_0;rs_0;rw_0;//�����������
	GPIO_Write(port,(0x00)<<lcd_shift);
}

void Init_lcd()//������������� �������
{
	Init_pin_out();//��������� ����� �� �����

	del=72000; while (del--){}//��������
	Lcd_write_cmd(Function_set);//�������� 4 ������ �����
	del=72000; while (del--){}//��������

	Lcd_write_cmd(Display_on_off_control);//�������� �������, ��������� ������ � �������
	del=72000; while (del--){}//��������

	Lcd_write_cmd(Display_clear);//������� ������
	del=72000; while (del--){}//��������

    Lcd_write_cmd(Entry_mode_set);//������� ����� ���������
}

void Lcd_clear()
{
	Lcd_write_cmd(Display_clear);//�������� ������� ������� �������
}

void Return_home()
{
	Lcd_write_cmd(0b0000001);//������� ��������� � ������ �������
}

void Lcd_definechar(const uint8_t *pc,uint8_t char_code)//�������� ������������ �������
{
	uint8_t a;//������ ����� ��� �������
	uint16_t i;//���������� �����
	a=(char_code<<3)|0x40;//���������� ��� �������
	for (i=0; i<8; i++)
	{
		Lcd_write_cmd(a++);//�������� ��� � ������
		Lcd_write_data(pc[i]);//���������� ������
	}
}
