#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "delay.h"

int n=0;

void resets(void)//����� ������
{
	GPIO_SetBits(GPIOA, GPIO_Pin_All);//����� ����� ���
}

void TIM3_IRQHandler(void)//��������� ������ �� ������
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//������� ��� ����������
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0)//���� ������ �� ������ ���������
	{
		Delay_ms(50);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0)//���� ������ �� ������ ���������
			{
		n++;//����������� ����
		if (n==2)//�������� 1 ����
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);//�������� ����
		}
		if (n==4)//�������� 2 ����
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		}
		if (n==6)//�������� 3 ����
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		}
		if (n==8)//�������� 4 ����
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		}
		if (n>8)//�������� ��� �����
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_All);
		}
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0){};//���� ���� �������� ������
			}
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0)//���� ������ �� ������ ������
	{
		n=0;//�������� ����������
		resets();//����� �����
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0){}//���� ���� �������� ������
	}
}

void main(void)
{
	GPIO_InitTypeDef PORTB, PORTA;//�������� �������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//������������ ����� B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//������������ ����� �

	//���� A
	GPIO_StructInit (&PORTA);
	PORTA.GPIO_Mode = GPIO_Mode_Out_PP;//�� �����
	PORTA.GPIO_Pin = GPIO_Pin_All;//��� ������
	PORTA.GPIO_Speed = GPIO_Speed_2MHz;//�������� �������
	GPIO_Init (GPIOA,&PORTA);//����������� �������� ����������� ���������
	GPIO_SetBits(GPIOA, GPIO_Pin_All);

	//���� B
	GPIO_StructInit (&PORTB);
	PORTB.GPIO_Mode = GPIO_Mode_IPU;//�� ����
	PORTB.GPIO_Pin = GPIO_Pin_All;//��� ������
	PORTB.GPIO_Speed = GPIO_Speed_2MHz;//�������� �������
	GPIO_Init (GPIOB,&PORTB);//����������� �������� ����������� ���������

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//�������� ������������ ������� 3

	//������ 3
	TIM3->PSC = 100;//������������
	TIM3->ARR = 100;//������ ������ 40�� 1/24�Hz/100/100=40
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//����������� ������ ��� ��������� ���������� �� ���������� (������������)
	TIM_Cmd(TIM3, ENABLE);//��������� ������
	NVIC_EnableIRQ(TIM3_IRQn);//��������� ���������� ������� 3

    while (1){}
}
