#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "delay.h"

int n=0;

void resets(void)//сброс диодов
{
	GPIO_SetBits(GPIOA, GPIO_Pin_All);//гасим диоды все
}

void TIM3_IRQHandler(void)//проверяем нажата ли кнопка
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//Очищаем бит прерывания
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0)//если нажали на кнопку установки
	{
		Delay_ms(50);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0)//если нажали на кнопку установки
			{
		n++;//увеличиваем счет
		if (n==2)//зажигаем 1 диод
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);//зажигаем диод
		}
		if (n==4)//зажигаем 2 диод
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		}
		if (n==6)//зажигаем 3 диод
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		}
		if (n==8)//зажигаем 4 диод
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		}
		if (n>8)//зажигаем все диоды
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_All);
		}
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0){};//ждем пока отпустят кнопку
			}
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0)//если нажали на кнопку сброса
	{
		n=0;//обнуляем переменную
		resets();//гасим диоды
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0){}//ждем пока отпустят кнопку
	}
}

void main(void)
{
	GPIO_InitTypeDef PORTB, PORTA;//создание структр

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//тактирование порта B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//тактирование порта А

	//Порт A
	GPIO_StructInit (&PORTA);
	PORTA.GPIO_Mode = GPIO_Mode_Out_PP;//на выход
	PORTA.GPIO_Pin = GPIO_Pin_All;//все выводы
	PORTA.GPIO_Speed = GPIO_Speed_2MHz;//скорость средняя
	GPIO_Init (GPIOA,&PORTA);//настраиваем согласно заполненной структуре
	GPIO_SetBits(GPIOA, GPIO_Pin_All);

	//Порт B
	GPIO_StructInit (&PORTB);
	PORTB.GPIO_Mode = GPIO_Mode_IPU;//на вход
	PORTB.GPIO_Pin = GPIO_Pin_All;//все выводы
	PORTB.GPIO_Speed = GPIO_Speed_2MHz;//скорость средняя
	GPIO_Init (GPIOB,&PORTB);//настраиваем согласно заполненной структуре

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//включаем тактирование таймера 3

	//Таймер 3
	TIM3->PSC = 100;//предделитель
	TIM3->ARR = 100;//период равный 40мс 1/24МHz/100/100=40
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//Настраиваем таймер для генерации прерывания по обновлению (переполнению)
	TIM_Cmd(TIM3, ENABLE);//Запускаем таймер
	NVIC_EnableIRQ(TIM3_IRQn);//разрешаем прерывание таймера 3

    while (1){}
}
