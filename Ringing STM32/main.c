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
#define use_gpio	GPIO_Pin_9  //������� ���								7
#define pin_d7		use_gpio    //������� ���								7
#define pin_d6		use_gpio>>1 //��������� ��� �� ��������					6
#define pin_d5		use_gpio>>2 //��������� ��� �� ��������					5
#define pin_d4		use_gpio>>3 //��������� ��� � 4-� ������ ����			4

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
0xC6, 0xC7 };

int second = 55;//������� �����
int minute = 44;
int hours = 8;

int Minutes;//����� �� ����� ���� ��� ��������
int Seconds;

int pisk=0;
int no_lesson=0;//����� ������� �������
int tools=1;//��������� ����� ������� ��� ������ ���������

int key=20;//����� ����������
int result=20;//����� ������� ������
int first=0;//����� ����
int second_s=0;//����� ������
int third=0;//����� �������
int set=0;//���������� ��� ��������� ��������
int count=1;//������ � �����

void Init_pin_out(void);
void Init_pin_in(void);
void Init_lcd(void);
void Lcd_write_data(uint8_t byte);
void Lcd_write_cmd(uint8_t byte);
void Lcd_clear(void);
void Return_home(void);
void Lcd_goto(uc8 x, uc8 y);
void Lcd_write_str(uc8 *STRING);
void Lcd_definechar(const uint8_t *pc,uint8_t char_code);

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
	Seconds=60-second;//������ ������ � �������� �������
	if ((hours>18 && minute>40) || hours<8 || hours>19)//���������� �� ����� ��������� �������
	{
		no_lesson=1;
	}
	else no_lesson=0;
	if (hours==8 && 0<=minute && minute<45)//������ ����� � �������� �������
	{
		Minutes=44-minute;
	}
	else if (hours==8 && 50<=minute && minute<59)
	{
		Minutes=58-minute+36;
	}
	else if (hours==9 && 0<=minute && minute<35)
	{
		Minutes=34-minute;
	}
	else if (hours==9 && 45<=minute && minute<59)
	{
		Minutes=58-minute+31;
	}
	else if (hours==10 && 0<=minute && minute<30)
	{
		Minutes=29-minute;
	}
	else if ((hours==10 && 35<=minute && minute<59) || (hours==13 && 35<=minute && minute<59))
	{
		Minutes=58-minute+21;
	}
	else if (hours==11 && 0<=minute && minute<20)
	{
		Minutes=19-minute;
	}
	else if (hours==11 && 30<=minute && minute<59)
	{
		Minutes=58-minute+16;
	}
	else if (hours==12 && 0<=minute && minute<15)
	{
		Minutes=14-minute;
	}
	else if ((hours==12 && 20<=minute && minute<59) || (hours==15 && 20<=minute && minute<59))
	{
		Minutes=58-minute+6;
	}
	else if (hours==13 && 0<=minute && minute<5)
	{
		Minutes=4-minute;
	}
	else if (hours==14 && 0<=minute && minute<20)
	{
		Minutes=19-minute;
	}
	else if (hours==14 && 25<=minute && minute<59)
	{
		Minutes=58-minute+11;
	}
	else if (hours==15 && 0<=minute && minute<10)
	{
		Minutes=9-minute;
	}
	else if (hours==16 && 0<=minute && minute<5)
	{
		Minutes=4-minute;
	}
	else if (hours==16 && 10<=minute && minute<55)
	{
		Minutes=54-minute;
	}
	else if (hours==17 && 5<=minute && minute<50)
	{
		Minutes=49-minute;
	}
	else if (hours==17 && 55<=minute && minute<59)
	{
		Minutes=58-minute+41;
	}
	else if (hours==18 && 0<=minute && minute<40)
	{
		Minutes=39-minute;
	}
	if (hours==8 && 45<=minute && minute<50)
	{
		Minutes=49-minute;
	}
	else if (hours==10 && 30<=minute && minute<35)
	{
		Minutes=34-minute;
	}
	else if (hours==12 && 15<=minute && minute<20)
	{
		Minutes=19-minute;
	}
	else if (hours==14 && 20<=minute && minute<25)
	{
		Minutes=24-minute;
	}
	else if (hours==16 && 5<=minute && minute<10)
	{
		Minutes=9-minute;
	}
	else if (hours==17 && 50<=minute && minute<55)
	{
		Minutes=54-minute;
	}
	else if (hours==9 && 35<=minute && minute<45)
	{
		Minutes=44-minute;
	}
	else if (hours==11 && 20<=minute && minute<30)
	{
		Minutes=29-minute;
	}
	else if (hours==16 && 55<=minute && minute<59)
	{
		Minutes=9-minute+5;
	}
	else if (hours==17 && 0<=minute && minute<5)
	{
		Minutes=4-minute;
	}
	else if (hours==15 && 10<=minute && minute<20)
	{
		Minutes=19-minute;
	}
	else if (hours==13 && 5<=minute && minute<35)
	{
		Minutes=34-minute;
	}
	if(second>59)//������� �������� �����
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

void Lesson()//��������� ���������� � ��������� � ��������
{
	Lcd_goto(1,0);

	if ((hours==8 && 0<=minute && minute<45) || ((hours==8 && 50<=minute && minute<=59) || (hours==9 && 0<=minute && minute<35)))
	{
		Lcd_write_str("�������");//�������
		Lcd_write_str(" ");//" "
		Lcd_write_str("1");//1
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
	}
	else if (((hours==9 && 45<=minute && minute<=59) || (hours==10 && 0<=minute && minute<30)) || ((hours==10 && 35<=minute && minute<=59) || (hours==11 && 0<=minute && minute<20)))
	{
		Lcd_write_str("�������");//�������
		Lcd_write_str(" ");//" "
		Lcd_write_str("2");//2
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
	}
	else if (((hours==11 && 30<=minute && minute<=59) || (hours==12 && 0<=minute && minute<15)) || ((hours==12 && 20<=minute && minute<=59) || (hours==13 && 0<=minute && minute<5)))
	{
		Lcd_write_str("�������");//�������
		Lcd_write_str(" ");//" "
		Lcd_write_str("3");//3
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
	}
	else if (((hours==13 && 35<=minute && minute<=59) || (hours==14 && 0<=minute && minute<20)) || ((hours==14 && 25<=minute && minute<=59) || (hours==15 && 0<=minute && minute<10)))
	{
		Lcd_write_str("�������");//�������
		Lcd_write_str(" ");//" "
		Lcd_write_str("4");//4
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
	}
	else if (((hours==15 && 20<=minute && minute<=59) || (hours==16 && 0<=minute && minute<5)) || (hours==16 && 10<=minute && minute<55))
	{
		Lcd_write_str("�������");//�������
		Lcd_write_str(" ");//" "
		Lcd_write_str("5");//5
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
	}
	else if ((hours==17 && 5<=minute && minute<50) || ((hours==17 && 55<=minute && minute<=59) || (hours==18 && 0<=minute && minute<40)))
	{
		Lcd_write_str("�������");//�������
		Lcd_write_str(" ");//" "
		Lcd_write_str("6");//6
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
	}
	else if(hours<8 || (hours>18 && minute>=40))
	{
		Lcd_write_str("�������");//�������
		Lcd_write_str(" ");//" "
		Lcd_write_str("���");//���
	}
	else
	{
		Lcd_write_str("�������");//�������
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
		Lcd_write_str(" ");//" "
	}
}

void TIM6_DAC_IRQHandler(void)//�������� ���������� � ��������� � ��������
{
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);//������� ��� ����������

	if (Minutes==0 && Seconds==0)
	{
			if (pisk==0)
			{
				pisk=1;
				GPIO_SetBits(GPIOB, GPIO_Pin_15);
			}
			else
			{
				GPIO_ResetBits(GPIOB, GPIO_Pin_15);
				pisk=0;
			}
	}
}

void show(int digit) {//���������� ����� ������� ������
	switch(digit)
	{
		case 0:
		{
			result=1;
			break;
		}
		case 1:
		{
			result=4;
			break;
		}
		case 2:
		{
			result=7;
			break;
		}
		case 3:
		{
			result=11;
			break;
		}
		case 4:
		{
			result=2;
			break;
		}
		case 5:
		{
			result=5;
			break;
		}
		case 6:
		{
			result=8;
			break;
		}
		case 7:
		{
			result=0;
			break;
		}
		case 8:
		{
			result=3;
			break;
		}
		case 9:
		{
			result=6;
			break;
		}
		case 10:
		{
			result=9;
			break;
		}
		case 11:
		{
			result=13;
			break;
		}
		case 15:
		{
			result=15;
			break;
		}
	}
	key=20;
}

void TIM4_IRQHandler(void)//����� ��������� ����������
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//������� ��� ����������
	GPIO_InitTypeDef STL;
	GPIO_InitTypeDef STR;

	GPIO_StructInit (&STL);
	STL.GPIO_Pin=(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0); //�������
	STL.GPIO_Mode=GPIO_Mode_IPU;
	STL.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &STL);

	GPIO_StructInit (&STR);
	STR.GPIO_Pin=(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //������
	STR.GPIO_Mode=GPIO_Mode_Out_PP;
	STR.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &STR);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0) key=0;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0) key=4;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0) key=8;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0) key=12;

	GPIO_StructInit (&STL);
	STL.GPIO_Pin=(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0); //������
	STL.GPIO_Mode=GPIO_Mode_Out_PP;
	STL.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &STL);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);

	GPIO_StructInit (&STR);
	STR.GPIO_Pin=(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //�������
	STR.GPIO_Mode=GPIO_Mode_IPU;;
	STR.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &STR);

	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0) key=key+0;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0) key=key+1;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0) key=key+2;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0) key=key+3;

	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0){};
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0){};
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0){};
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0){};

	show(key);
	if (result!=20)
	{
		if (result==11||result==13||result==15)
		{
			TIM_Cmd(TIM3, DISABLE);//��������� ������
			tools=0;//���������� ���� �������
			no_lesson=1;//���������� �������� ����
			set=result;//���������� ��� ��������� �������� �������
		}
		if (set==15)//�������� ���������
		{
			count=1;
			first=0;//���������� ����
			second_s=0;//���������� ������
			third=0;//���������� �������
			set=11;
			tools=0;//��������� ������ ��������� �������
			no_lesson=1;//��������� �������� ������
		}
		if (set==13)//������� ���������
		{
			count=1;
			hours=first;//������������� ����
			minute=second_s;//������������� ������
			second=third;//������������� �������
			first=0;//���������� ����
			second_s=0;//���������� ������
			third=0;//���������� �������
			set=0;
			tools=1;//��������� ������ ��������� �������
			no_lesson=0;//��������� �������� ������
			TIM_Cmd(TIM3, ENABLE);//��������� ������
		}
		//��������� �����
		if (count==1 && result <=2)
		{
			first=result*10;//������� ����� �����
			count=2;
		}
		else if ((count==2 && first<=10 && result<=9)||(count==2 && first==20 && result<=3))
		{
			count=3;
			first=first+result;//��������� ����� �����
		}
		//��������� �����
		else if (count==3 && result<=5)
		{
			count=4;
			second_s=result*10;//������� ����� �����
		}
		else if (count==4)
		{
			count=5;
			second_s=second_s+result;//��������� ����� �����
		}
		//��������� ������
		else if (count==5 && result<=5)
		{
			count=6;
			third=result*10;//������� ����� ������
		}
		else if (count==6)
		{
			count=1;
			third=third+result;//��������� ����� ������
		}
		result=20;
	}
}

void TIM2_IRQHandler(void)//����� �� ����� ��� � �������
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//������� ��� ����������
	if (tools==1)
	{
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

	Lesson();

	if (no_lesson==0)
	{
		Lcd_goto(2,11);
	    clockTime(Minutes/10);
	    Lcd_goto(2,12);
	    clockTime(Minutes%10);
	    Lcd_goto(2,13);
	    Lcd_write_str(":");
	    Lcd_goto(2,14);
	    clockTime(Seconds/10);
	    Lcd_goto(2,15);
	    clockTime(Seconds%10);
	 }
	 else
	 {
	    Lcd_goto(2,11);
	    Lcd_write_str(" ");
	    Lcd_goto(2,12);
	    Lcd_write_str(" ");
	    Lcd_goto(2,13);
	    Lcd_write_str(" ");
	    Lcd_goto(2,14);
	    Lcd_write_str(" ");
	    Lcd_goto(2,15);
	    Lcd_write_str(" ");
	  }

	  if (set==11||set==15)
	  {
	    Lcd_goto(0,8);//���������� ����� ��������� �����
	    clockTime(first/10);
	    Lcd_goto(0,9);
	    clockTime(first%10);
	    Lcd_goto(0,10);
	    Lcd_write_str(":");
	    Lcd_goto(0,11);
	    clockTime(second_s/10);
	    Lcd_goto(0,12);
	    clockTime(second_s%10);
	    Lcd_goto(0,13);
	    Lcd_write_str(":");
	    Lcd_goto(0,14);
	    clockTime(third/10);
	    Lcd_goto(0,15);
	    clockTime(third%10);
	   }
}

void main(void)
{
	GPIO_InitTypeDef PORTB;//�������� �������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//������������ ����� B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//������������ ����� �

	//���� B
	GPIO_StructInit (&PORTB);
	PORTB.GPIO_Mode = GPIO_Mode_Out_PP;//�� �����
	PORTB.GPIO_Pin = GPIO_Pin_15;//������� ������
	PORTB.GPIO_Speed = GPIO_Speed_2MHz;//�������� �������
	GPIO_Init (GPIOB,&PORTB);//����������� �������� ����������� ���������

	SystemInit();
	Init_lcd();
	Lcd_clear();

	Lcd_goto(0,0);
	Lcd_write_str("�����:");
	Lcd_goto(2,0);
	Lcd_write_str("��������:");

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//�������� ������������ ������� 6
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//�������� ������������ ������� 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//�������� ������������ ������� 3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//�������� ������������ ������� 2

	//������ 6
	TIM6->PSC = 1;//������������
	TIM6->ARR = 100;//������
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);//����������� ������ ��� ��������� ���������� �� ���������� (������������)
	TIM_Cmd(TIM6, ENABLE);//��������� ������
	NVIC_EnableIRQ(TIM6_DAC_IRQn);//��������� ���������� ������� 1

	//������ 4
	TIM4->PSC = 100;//������������
	TIM4->ARR = 100;//������
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

void Init_pin_out()
{
	RCC_APB2PeriphClockCmd(init_port, ENABLE);//������������ ����� �
	GPIO_InitTypeDef init_pin;//�������� ���������
	init_pin.GPIO_Pin  = pin_e | pin_rs | pin_rw | pin_d7 | pin_d6 | pin_d5 | pin_d4;//����������� ������
	init_pin.GPIO_Mode = GPIO_Mode_Out_PP;//�� �����
	init_pin.GPIO_Speed = GPIO_Speed_50MHz;//������� ��������
	GPIO_Init (port, &init_pin);//��������� ��������� � ���������
}
void Init_pin_in()
{
	RCC_APB2PeriphClockCmd(init_port, ENABLE);//������������ ����� �
	GPIO_InitTypeDef init_pin;//�������� ���������
	init_pin.GPIO_Pin  =  pin_d7 | pin_d6 | pin_d5 | pin_d4 ;//����������� ������
	init_pin.GPIO_Mode = GPIO_Mode_IPD;//�� ����
	init_pin.GPIO_Speed = GPIO_Speed_50MHz;//������� ��������
	GPIO_Init (port, &init_pin);//��������� ��������� � ���������
}

void Lcd_write_cmd(uc8 cmd )
{
	Init_pin_out();//��������� ������ �� �����
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
	Init_pin_out();//��������� ������ �� �����
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
	Init_pin_out();//����������� ����� �� �����
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
