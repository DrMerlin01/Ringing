#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

int key = 20;//для клавиатуры
int result = 20;//для передачи числа на дисплей
int temp = 4;//разряды

int first=4;//начальные числа выводит
int second=5;
int third=6;
int fourth=7;

void show(int digit) {//вывод числа
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

void TIM4_IRQHandler(void)//опрос матричной клавиатуры
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//Очищаем бит прерывания
	GPIO_InitTypeDef STL, STR;//создаем структуры

	GPIO_StructInit (&STL);//инициализируем структуру
	STL.GPIO_Pin=(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0); //столбцы
	STL.GPIO_Mode=GPIO_Mode_IPU;//на вход
	STL.GPIO_Speed=GPIO_Speed_2MHz;//скорость
	GPIO_Init(GPIOA, &STL);//загружаем настройки

	GPIO_StructInit (&STR);//инициализируем структуру
	STR.GPIO_Pin=(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //строки
	STR.GPIO_Mode=GPIO_Mode_Out_PP;//на вход
	STR.GPIO_Speed=GPIO_Speed_2MHz;//скорость
	GPIO_Init(GPIOA, &STR);//загружаем настройки
	GPIO_ResetBits(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);//ставим в 1

	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0) key=0;//проверяем столбцы
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0) key=4;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0) key=8;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0) key=12;

	GPIO_StructInit (&STL);//инициализируем структуру
	STL.GPIO_Pin=(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0); //строки
	STL.GPIO_Mode=GPIO_Mode_Out_PP;//на вход
	STL.GPIO_Speed=GPIO_Speed_2MHz;//скорость
	GPIO_Init(GPIOA, &STL);//загружаем настройки
	GPIO_ResetBits(GPIOA, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);//ставим в 1

	GPIO_StructInit (&STR);//инициализируем структуру
	STR.GPIO_Pin=(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //столбцы
	STR.GPIO_Mode=GPIO_Mode_IPU;;//на вход
	STR.GPIO_Speed=GPIO_Speed_2MHz;//скокрость обмена
	GPIO_Init(GPIOA, &STR);//загружаем настройки

	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0) key=key+0;//проверяем строки
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0) key=key+1;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0) key=key+2;
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0) key=key+3;

	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0){};//ждем пока отпустят клавишу
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0){};
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0){};
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0){};

	if (key != 20)//записываем число
	{
		result=key;
		temp++;//увеличиваем разряд
	}
	key=20;//ставим в начальное состояние
	if (temp > 7)//если разряд больше сбрасываем
	{
		temp = 4;
	}
}

void TIM2_IRQHandler(void)//Вывод на экран раз в секунду
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//Очищаем бит прерывания
	if (result != 20)//проверяем была ли зажата клавиша и в каком разряде вывести
	{
		if ((temp-1) == 4)//первый
		{
			first = result;
		}
		if ((temp-1) == 5)//второй
		{
			second = result;
		}
		if ((temp-1) == 6)//третий
		{
			third = result;
		}
		if (temp == 4)//четвертый
		{
			fourth = result;
		}
		result = 20;
	}

	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//гасим сегменты
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//зажигаем разряд
	if(first>=0)
	{
		show(first);//выводим число
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_12);//гасим разряд

	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//гасим сегменты
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);//зажигаем разряд
	if(second>=0)
	{
		show(second);//выводим число
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_13);//гасим разряд

	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//гасим сегменты
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);//зажигаем разряд
	if(third>=0)
	{
		show(third);//выводим число
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_14);//гаисм разряд

	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//гасим сегменты
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);//зажигаем разряд
	if(fourth>=0)
	{
		show(fourth);//выводим число
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_15);//гасим разряд
}

void main(void)
{
	GPIO_InitTypeDef PORTB;//создание структр

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//тактирование порта B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//тактирование порта А

	//Порт B
	GPIO_StructInit (&PORTB);
	PORTB.GPIO_Mode = GPIO_Mode_Out_PP;//на выход
	PORTB.GPIO_Pin = GPIO_Pin_All;//все выводы
	PORTB.GPIO_Speed = GPIO_Speed_2MHz;//скорость средняя
	GPIO_Init (GPIOB,&PORTB);//настраиваем согласно заполненной структуре

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//включаем тактирование таймера 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//включаем тактирование таймера 2

	//Таймер 4
	TIM4->PSC = 1000;//предделитель
	TIM4->ARR = 1000;//период, таймер тикнет 2400 раз в секунду, расчеты такие 1/ 24 000 000 / 100 / 100 = 40 мсек примерно
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//Настраиваем таймер для генерации прерывания по обновлению (переполнению)
	TIM_Cmd(TIM4, ENABLE);//Запускаем таймер
	NVIC_EnableIRQ(TIM4_IRQn);//разрешаем прерывание таймера 4

	//Таймер 2
	TIM2->PSC = 10;//предделитель
	TIM2->ARR = 10;//период
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//Настраиваем таймер для генерации прерывания по обновлению (переполнению)
	TIM_Cmd(TIM2, ENABLE);//Запускаем таймер
	NVIC_EnableIRQ(TIM2_IRQn);//разрешаем прерывание таймера 2

    while (1){}
}
