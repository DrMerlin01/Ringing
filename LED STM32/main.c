#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

int key = 20;//��� ����������
int result = 20;//��� �������� ����� �� �������
int temp = 4;//�������

int first=4;//��������� ����� �������
int second=5;
int third=6;
int fourth=7;

void show(int digit) {//����� �����
	switch(digit)
	{
		case 0:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
			break;
		}
		case 1:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_2);
			break;
		}
		case 2:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8);
			break;
		}
		case 3:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_8);
			break;
		}
		case 4:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
		case 5:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
		case 6:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
		case 7:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
			break;
		}
		case 8:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
		case 9:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
		case 10:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
		case 11:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
		case 12:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
			break;
		}
		case 13:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8);
			break;
		}
		case 14:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
		case 15:
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
			break;
		}
	}
}

void TIM4_IRQHandler(void)//����� ��������� ����������
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//������� ��� ����������
	GPIO_InitTypeDef STL, STR;//������� ���������

	GPIO_StructInit (&STL);//�������������� ���������
	STL.GPIO_Pin=(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0); //�������
	STL.GPIO_Mode=GPIO_Mode_IPU;//�� ����
	STL.GPIO_Speed=GPIO_Speed_2MHz;//��������
	GPIO_Init(GPIOA, &STL);//��������� ���������

	GPIO_StructInit (&STR);//�������������� ���������
	STR.GPIO_Pin=(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //������
	STR.GPIO_Mode=GPIO_Mode_Out_PP;//�� ����
	STR.GPIO_Speed=GPIO_Speed_2MHz;//��������
	GPIO_Init(GPIOA, &STR);//��������� ���������
	GPIO_ResetBits(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);//������ � 1

	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0) key=0;//��������� �������
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0) key=4;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0) key=8;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0) key=12;

	GPIO_StructInit (&STL);//�������������� ���������
	STL.GPIO_Pin=(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0); //������
	STL.GPIO_Mode=GPIO_Mode_Out_PP;//�� ����
	STL.GPIO_Speed=GPIO_Speed_2MHz;//��������
	GPIO_Init(GPIOA, &STL);//��������� ���������
	GPIO_ResetBits(GPIOA, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);//������ � 1

	GPIO_StructInit (&STR);//�������������� ���������
	STR.GPIO_Pin=(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //�������
	STR.GPIO_Mode=GPIO_Mode_IPU;;//�� ����
	STR.GPIO_Speed=GPIO_Speed_2MHz;//��������� ������
	GPIO_Init(GPIOA, &STR);//��������� ���������

	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0) key=key+0;//��������� ������
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0) key=key+1;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0) key=key+2;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0) key=key+3;

	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0){};//���� ���� �������� �������
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0){};
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0){};
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0){};

	if (key != 20)//���������� �����
	{
		result=key;
		temp++;//����������� ������
	}
	key=20;//������ � ��������� ���������
	if (temp > 7)//���� ������ ������ ����������
	{
		temp = 4;
	}
}

void TIM2_IRQHandler(void)//����� �� ����� ��� � �������
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//������� ��� ����������
	if (result != 20)//��������� ���� �� ������ ������� � � ����� ������� �������
	{
		if ((temp-1) == 4)//������
		{
			first = result;
		}
		if ((temp-1) == 5)//������
		{
			second = result;
		}
		if ((temp-1) == 6)//������
		{
			third = result;
		}
		if (temp == 4)//���������
		{
			fourth = result;
		}
		result = 20;
	}

	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//����� ��������
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//�������� ������
	if(first>=0)
	{
		show(first);//������� �����
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_12);//����� ������

	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//����� ��������
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);//�������� ������
	if(second>=0)
	{
		show(second);//������� �����
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_13);//����� ������

	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//����� ��������
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);//�������� ������
	if(third>=0)
	{
		show(third);//������� �����
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_14);//����� ������

	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//����� ��������
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);//�������� ������
	if(fourth>=0)
	{
		show(fourth);//������� �����
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_15);//����� ������
}

void main(void)
{
	GPIO_InitTypeDef PORTB;//�������� �������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//������������ ����� B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//������������ ����� �

	//���� B
	GPIO_StructInit (&PORTB);
	PORTB.GPIO_Mode = GPIO_Mode_Out_PP;//�� �����
	PORTB.GPIO_Pin = GPIO_Pin_All;//��� ������
	PORTB.GPIO_Speed = GPIO_Speed_2MHz;//�������� �������
	GPIO_Init (GPIOB,&PORTB);//����������� �������� ����������� ���������

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//�������� ������������ ������� 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//�������� ������������ ������� 2

	//������ 4
	TIM4->PSC = 1000;//������������
	TIM4->ARR = 1000;//������, ������ ������ 2400 ��� � �������, ������� ����� 1/ 24 000 000 / 100 / 100 = 40 ���� ��������
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//����������� ������ ��� ��������� ���������� �� ���������� (������������)
	TIM_Cmd(TIM4, ENABLE);//��������� ������
	NVIC_EnableIRQ(TIM4_IRQn);//��������� ���������� ������� 4

	//������ 2
	TIM2->PSC = 10;//������������
	TIM2->ARR = 10;//������
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//����������� ������ ��� ��������� ���������� �� ���������� (������������)
	TIM_Cmd(TIM2, ENABLE);//��������� ������
	NVIC_EnableIRQ(TIM2_IRQn);//��������� ���������� ������� 2

    while (1){}
}
